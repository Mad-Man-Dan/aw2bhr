#!/usr/bin/env python3
"""Classify a function's data_refs into real globals and agbcc `.LC` pool words.

Wave 15 established that the number of `-fforce-addr` literal-pool words a
large function carries is what predicts the cost of matching it -- monotone
across seven functions, where size, callee count, new-symbol count and the
index's own `difficulty` field all order it wrong, two of them backwards:

    .LC words | outcome
    0         | matched (three of four on the first try)
    1         | +8 bytes
    2         | -8 bytes
    2, needing 3 distinct read-site spellings | -28 bytes

The catch this tool exists for: `data_refs` in data/functions.json cannot tell a
shared global from a shared pool word, because the splitter invents a
`gUnknown_<addr>` symbol for both. Wave 15's large batch was chosen believing it
was `.LC`-free and three of its four functions carried pool words for globals
nobody had looked at.

The screen is fan-in == 1 AND run-density >= 4 on the 4-byte grid. NEITHER HALF
IS SUFFICIENT and this is never a verdict, only a prioritisation:

  * density alone misclassifies real tables -- gUnknown_08499590 sits at
    density 12 with fan-in 327, because genuinely popular globals cluster too.
  * fan-in alone would park the proc-script wrappers, the cheapest family in
    the tree.

Validated against 14 hand-checked controls (8 known pool words, 6 known
globals); known blocks include 0x08091350-0x0809138C, 0x0816D938-0x0816D9BC and
0x0816D9E0+. Run the screen rather than trusting any address list, this one
included.

    python tools/lc_screen.py                 # summarise the straight-line band
    python tools/lc_screen.py sub_0800AA30    # classify one function's refs
"""
import bisect
import json
import os
import re
import sys
import collections

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_ADDR = re.compile(r"^gUnknown_0([0-9A-F]{7})$")

# A pool word is referenced by exactly one function...
FANIN_MAX = 1
# ...and sits in a run of consecutive 4-byte slots that are also referenced.
DENSITY_MIN = 4
# Half-width of the window swept over the 4-byte grid, in slots.
WINDOW = 8


def load(path=None):
    path = path or os.path.join(ROOT, "data", "functions.json")
    with open(path, encoding="utf-8") as fh:
        return json.load(fh)


def sym_addr(sym):
    """Address encoded in a splitter-invented symbol name, or None."""
    m = _ADDR.match(sym)
    return int(m.group(1), 16) if m else None


class Screen:
    def __init__(self, functions):
        self.fanin = collections.Counter()
        self.refs = set()
        for f in functions:
            for r in set(f.get("data_refs", ())):
                self.fanin[r] += 1
                a = sym_addr(r)
                if a is not None:
                    self.refs.add(a)

    def density(self, addr):
        return sum(1 for k in range(-WINDOW, WINDOW + 1)
                   if (addr + 4 * k) in self.refs)

    def is_pool_word(self, sym):
        addr = sym_addr(sym)
        if addr is None:
            return False
        return (self.fanin[sym] <= FANIN_MAX
                and self.density(addr) >= DENSITY_MIN)

    def split(self, fn):
        """(pool_words, real_globals) for one function record."""
        pool, real = [], []
        for r in sorted(set(fn.get("data_refs", ()))):
            (pool if self.is_pool_word(r) else real).append(r)
        return pool, real


_ROM_SYM = re.compile(r"\bgUnknown_(08[0-9A-F]{6})\b")


def _is_address(word):
    return (0x02000000 <= word < 0x02040000 or 0x03000000 <= word < 0x03008000
            or 0x08000000 <= word < 0x08800000)


def pool_word_drafts(fns):
    """[(fn, symbol, word)]: unmatched drafts that name a compiler-made pool
    word instead of the global it points at.

    agbcc sometimes puts a global's address in a word of .rodata and loads
    it from there. The splitter names that word gUnknown_08XXXXXX, and a
    draft that reads the word as a pointer variable compiles to different
    code than one that names the real global. Wave 91 changed four drafts
    from the first spelling to the second and all four moved; one matched.

    A symbol is flagged when its ROM word is itself an address AND no other
    function references it. Both halves matter: without the fan-in test,
    shared pointer globals such as gUnknown_08499594 (167 readers) are
    flagged too.
    """
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    from comment_check import strip_c
    with open(os.path.join(ROOT, "baserom.gba"), "rb") as fh:
        rom = fh.read()
    scr = Screen(fns)
    code_spans = sorted((f["addr"], f["addr"] + f["size"]) for f in fns)
    starts = [s for s, _ in code_spans]

    def inside_code(word):
        """A word pointing into the middle of a function is data that happens
        to look like an address (two halfwords, say), not a pointer."""
        k = bisect.bisect_right(starts, word) - 1
        return k >= 0 and code_spans[k][0] < (word & ~1) < code_spans[k][1]

    out = []
    for f in fns:
        if f["status"] != "parked":
            continue
        path = os.path.join(ROOT, "work", f["name"], f["name"] + ".c")
        if not os.path.isfile(path):
            continue
        with open(path, encoding="utf-8", errors="replace") as fh:
            code = strip_c(fh.read())
        for hexaddr in sorted(set(_ROM_SYM.findall(code))):
            off = int(hexaddr, 16) - 0x08000000
            if off % 4 or off + 4 > len(rom):
                continue
            word = int.from_bytes(rom[off:off + 4], "little")
            sym = "gUnknown_" + hexaddr
            if (_is_address(word) and not inside_code(word)
                    and scr.fanin[sym] <= FANIN_MAX):
                out.append((f["name"], sym, word))
    return out


def main(argv):
    fns = load()
    if argv[:1] == ["--drafts"]:
        hits = pool_word_drafts(fns)
        by_fn = collections.defaultdict(list)
        for fn, sym, word in hits:
            by_fn[fn].append((sym, word))
        print("%d unmatched draft(s) name a compiler-made pool word:" % len(by_fn))
        for fn in sorted(by_fn):
            print("  %s" % fn)
            for sym, word in by_fn[fn]:
                print("      %s holds 0x%08X -- name the global at 0x%08X, "
                      "not %s" % (sym, word, word, sym))
        return 0
    scr = Screen(fns)
    by_name = {f["name"]: f for f in fns}

    if argv:
        for name in argv:
            f = by_name.get(name)
            if f is None:
                print("%-16s ?? not in index" % name)
                continue
            pool, real = scr.split(f)
            print("%s  %dB  %d .LC word(s)"
                  % (name, f["size"], len(pool)))
            for r in pool:
                print("   .LC   %-22s fanin=%-4d density=%d"
                      % (r, scr.fanin[r], scr.density(sym_addr(r))))
            for r in real:
                a = sym_addr(r)
                print("   glob  %-22s fanin=%-4d density=%s"
                      % (r, scr.fanin[r],
                         scr.density(a) if a is not None else "-"))
        return 0

    band = [f for f in fns
            if f["status"] == "asm" and f["mode"] == "THUMB"
            and f["backward_branches"] == 0 and f["size"] >= 256]
    clean = [f for f in band if not scr.split(f)[0]]
    dirty = [f for f in band if scr.split(f)[0]]
    fmt = "  %-9s %4d funcs %7d bytes"
    print("straight-line band >=256B: %d funcs, %d bytes"
          % (len(band), sum(f["size"] for f in band)))
    print(fmt % (".LC-free", len(clean), sum(f["size"] for f in clean)))
    print(fmt % ("has .LC", len(dirty), sum(f["size"] for f in dirty)))
    print("\ncheapest-first (.LC-free, largest first):")
    for f in sorted(clean, key=lambda x: -x["size"])[:25]:
        print("  %-16s %4dB calls=%-2d globals=%-2d %s"
              % (f["name"], f["size"], len(f["calls"]),
                 len(scr.split(f)[1]), f["src"]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
