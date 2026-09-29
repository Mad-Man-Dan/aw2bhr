#!/usr/bin/env python3
"""Compare several spellings of one function in a single table.

Wave 96 matched sub_080611D8 by compiling five tail spellings side by side and
reading size, frame and push list off the assembly; the scripts were throwaway
(build/probe/w96*.py). This is that, for any function:

    python tools/spellings.py sub_080611D8 a.c b.c c.c
    python tools/spellings.py sub_080611D8 variants.c --variants 0,1,2,3

Each file is a complete draft of <fn> (with its includes). With --variants the
ONE file holds all the spellings as `#if VARIANT == 2 ... #endif` blocks, and
each listed number is compiled with `#define VARIANT <n>` put in front.

Every variant is compiled with the project's own path (drafts.build, i.e. the
Makefile recipe with the function's compiler override) into build/drafts/ and
scored against the ROM the way `drafts.py score` does; work/ is never written.
Per variant the table shows

    bytes    the function's size and the difference from the ROM's
    pct      byte match, as tools/trymatch.py counts it
    first    offset of the first differing byte (larger is better)
    frame    the stack adjustment (`sub sp`) and the push list(s), read off
             the compiler's own assembly, next to the ROM's
    at       the first differing instruction: ROM word vs this variant's

The first two lines (`ROM`, and each variant's `frame`) are what usually decides
whether a spelling is worth chasing: a wrong frame or push list means a wrong
local count or a wrong live range, and no register nudge fixes that.

    --one-unit   compile all variants as V_0, V_1, ... in ONE translation unit
                 (one compile; frame and size only, no ROM score). Same idea as
                 the wave-96 probes; use it to screen many spellings quickly.
    --keep       leave the generated files under build/spellings/<fn>/

Exit status: 0 if any variant MATCHes, 1 otherwise, 2 on a usage error.
"""

import argparse
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import agbenv  # noqa: E402
import awlib  # noqa: E402
import drafts  # noqa: E402

REPO = awlib.REPO
OUT = os.path.join("build", "spellings")


# ------------------------------------------------------------ assembly reading

def _regs(text):
    return "{" + ",".join(r.strip() for r in text.split(",")) + "}"


def frame_of(asm):
    """(frame bytes or None, [push lists]) from one function's assembly.

    Reads both the compiler's spelling (`add sp, #-32`) and the disassembler's
    (`sub sp, #0x20`). The frame is the sum of the stack adjustments before the
    first `bl`/branch-free body -- in practice the prologue's single one.
    """
    frame, pushes = None, []
    for ln in asm.splitlines():
        s = ln.strip()
        m = re.match(r"push\s*\{([^}]*)\}", s)
        if m:
            pushes.append(_regs(m.group(1)))
            continue
        m = re.match(r"sub\s+sp,\s*(?:sp,\s*)?#(-?(?:0x[0-9a-fA-F]+|\d+))", s)
        if m and frame is None:
            frame = int(m.group(1), 0)
            continue
        m = re.match(r"add\s+sp,\s*(?:sp,\s*)?#(-(?:0x[0-9a-fA-F]+|\d+))", s)
        if m and frame is None:
            frame = -int(m.group(1), 0)
    return frame, pushes


def function_asm(asm, name):
    """The text of `name`'s body inside a whole-unit .s (up to .size or the next label)."""
    m = re.search(r"^%s:\s*(?:@.*)?$(.*?)(?=^\s*\.size\s+%s|\Z)" % (re.escape(name), re.escape(name)),
                  asm, re.S | re.M)
    return m.group(1) if m else ""


def describe_frame(frame, pushes):
    return "sub sp %s, push %s" % (
        "-" if frame is None else "#0x%X" % frame,
        " + ".join(pushes) if pushes else "-")


# ------------------------------------------------------------------ variants

def make_sources(fn, args, tmpdir):
    """[(label, repo-relative path)] for the variants named on the command line."""
    out = []
    if args.variants:
        if len(args.files) != 1:
            raise SystemExit("error: --variants takes exactly one file")
        src = open(args.files[0], encoding="utf-8", errors="replace").read()
        for v in args.variants.split(","):
            v = v.strip()
            path = os.path.join(tmpdir, "v%s.c" % v)
            awlib.write_text(path, "#define VARIANT %s\n%s" % (v, src))
            out.append(("VARIANT=" + v, drafts.rel(path)))
    else:
        for f in args.files:
            out.append((os.path.basename(f), drafts.rel(os.path.abspath(f))))
    return out


def one_unit(fn, sources, tmpdir):
    """Compile every variant's function, renamed V_<i>, into one unit.

    Returns [(label, size or None, frame, pushes)]. The unit is the first file's
    text with the function's definition replaced by all the variants' definitions,
    so everything above the function (includes, statics, helpers) is shared.
    """
    defs = []
    head = None
    for i, (label, rel) in enumerate(sources):
        text = open(os.path.join(REPO, rel.replace("/", os.sep)), encoding="utf-8",
                    errors="replace").read()
        m = re.search(r"^[A-Za-z_][^\n;{]*\b%s\s*\(" % re.escape(fn), text, re.M)
        if not m:
            raise SystemExit("error: %s not defined in %s" % (fn, label))
        if head is None:
            head = text[:m.start()]
        body = text[m.start():]
        defs.append(re.sub(r"\b%s\b" % re.escape(fn), "V_%d" % i, body))
    unit = os.path.join(tmpdir, "unit.c")
    awlib.write_text(unit, head + "\n".join(defs))
    o = os.path.join(OUT, fn, "unit.o").replace(os.sep, "/")
    rc, so, se = agbenv.compile_c(drafts.rel(unit), o, fn=fn)
    if rc != 0:
        raise SystemExit("error: the unit does not compile:\n" + (se or so)[-1200:])
    asm = open(os.path.join(REPO, o[:-2].replace("/", os.sep) + ".s"), encoding="utf-8",
               errors="replace").read()
    rc, so, se = agbenv.run("arm-none-eabi-nm -S %s" % o)
    sizes = {}
    for ln in so.splitlines():
        p = ln.split()
        if len(p) == 4:
            sizes[p[3]] = int(p[1], 16)
    rows = []
    for i, (label, rel) in enumerate(sources):
        fr, pu = frame_of(function_asm(asm, "V_%d" % i))
        rows.append((label, sizes.get("V_%d" % i), fr, pu))
    return rows


def rom_facts(fn):
    """(size, frame, pushes) of the ROM function from work/<fn>/target.s, or None."""
    t = drafts.target(fn)
    if t is None:
        return None
    size = t[0]["size"]
    p = os.path.join(REPO, "work", fn, "target.s")
    if not os.path.exists(p):
        return size, None, []
    fr, pu = frame_of(open(p, encoding="utf-8", errors="replace").read())
    return size, fr, pu


def first_insn(fn, obj_rel, off):
    """The 16-bit word at `off` of the variant's .text, as hex, or ''."""
    try:
        data = drafts._read(drafts.absp(obj_rel[:-2] + ".text.bin"))
        return "%04X" % (data[off] | (data[off + 1] << 8))
    except (IndexError, TypeError):
        return ""


def rom_word(fn, off):
    t = drafts.target(fn)
    if t is None:
        return ""
    data = t[1]
    try:
        return "%04X" % (data[off] | (data[off + 1] << 8))
    except IndexError:
        return ""


# --------------------------------------------------------------------- main

def run(args):
    fn = args.fn
    facts = rom_facts(fn)
    if facts is None:
        print("error: %s is not in data/functions.json" % fn)
        return 2
    rsize, rframe, rpush = facts
    tmpdir = os.path.join(REPO, OUT, fn)
    os.makedirs(tmpdir, exist_ok=True)
    sources = make_sources(fn, args, tmpdir)
    if not sources:
        print("error: nothing to compare")
        return 2
    print("%-28s %5d bytes   %s" % ("ROM", rsize, describe_frame(rframe, rpush)))

    if args.one_unit:
        for label, size, fr, pu in one_unit(fn, sources, tmpdir):
            d = "" if size is None else "%+d" % (size - rsize)
            print("%-28s %5s bytes   %s%s%s" % (
                label[:28], size if size is not None else "?", describe_frame(fr, pu),
                ("   size %s" % d) if d else "",
                "   FRAME DIFFERS" if fr != rframe or pu != rpush else ""))
        return 1

    matched = False
    for i, (label, rel) in enumerate(sources):
        b = drafts.build(fn, rel, "spell-%d" % i)
        r = drafts.score(fn, b)
        if r["state"] in ("COMPILE-FAIL", "ERROR"):
            print("%-28s %s" % (label[:28], drafts.fmt(r)))
            continue
        asm_path = os.path.join(REPO, b.obj[:-2].replace("/", os.sep) + ".s")
        asm = open(asm_path, encoding="utf-8", errors="replace").read()
        fr, pu = frame_of(function_asm(asm, fn) or asm)
        size = len(b.text)
        if r["state"] == "MATCH":
            matched = True
            print("%-28s %5d bytes   MATCH   %s" % (label[:28], size, describe_frame(fr, pu)))
            continue
        at = ""
        if r.get("first") is not None:
            at = "%s vs %s" % (rom_word(fn, r["first"] & ~1),
                               first_insn(fn, b.obj, r["first"] & ~1))
        print("%-28s %5d bytes   %6.2f%%  size%+d  first+0x%X   %s%s%s" % (
            label[:28], size, r["pct"], r["size_delta"], r["first"],
            describe_frame(fr, pu),
            "   FRAME DIFFERS" if fr != rframe or pu != rpush else "",
            ("   at " + at) if at else ""))
    return 0 if matched else 1


def self_test():
    ok = True

    def check(name, cond):
        nonlocal ok
        print("[self-test] %s: %s" % (name, "PASS" if cond else "FAIL"))
        ok = ok and cond

    asm_c = "sub_X:\n\tpush\t{r4, r5, lr}\n\tadd\tsp, #-12\n\tmov\tr0, #0\n\t.size\tsub_X, .-sub_X\n"
    asm_t = "sub_X: @ 0x08000000\n\tpush {r4, r5, lr}\n\tsub sp, #0xC\n\tmovs r0, #0\n"
    check("compiler spelling", frame_of(function_asm(asm_c, "sub_X")) == (12, ["{r4,r5,lr}"]))
    check("disassembler spelling", frame_of(asm_t) == (12, ["{r4,r5,lr}"]))
    check("two push lists", frame_of("push {r4, lr}\nmov r7, sl\npush {r5, r6, r7}\nsub sp, #4")[1]
          == ["{r4,lr}", "{r5,r6,r7}"])
    # one real compile: a promoted function must MATCH against itself
    import json
    prom = json.load(open(os.path.join(REPO, "data", "promoted.json")))
    pick = next((r for r in prom if r["functions"] and 24 <= r["size"] <= 60
                 and os.path.exists(os.path.join(REPO, r["file"]))), None)
    if pick is None:
        print("[self-test] no small promoted function to compile; skipped")
    else:
        fn = pick["functions"][0]
        ns = argparse.Namespace(fn=fn, files=[os.path.join(REPO, pick["file"])] * 2,
                                variants=None, one_unit=False, keep=False)
        import io
        import contextlib
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            rc = run(ns)
        out = buf.getvalue()
        check("a promoted function matches itself in the table (%s)" % fn,
              rc == 0 and out.count("MATCH") == 2)
    return 0 if ok else 1


def main():
    doc = __doc__.split("\n\n", 1)
    ap = argparse.ArgumentParser(description=doc[0], epilog=doc[1],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("fn", nargs="?")
    ap.add_argument("files", nargs="*", help="variant files (complete drafts of <fn>)")
    ap.add_argument("--variants", help="comma list of VARIANT numbers for a single #if file")
    ap.add_argument("--one-unit", action="store_true",
                    help="compile all variants as V_<i> in one unit (size and frame only)")
    ap.add_argument("--keep", action="store_true")
    ap.add_argument("--self-test", action="store_true")
    a = ap.parse_args()
    if a.self_test:
        return self_test()
    if not a.fn or not a.files:
        ap.error("need <fn> and at least one variant file")
    return run(a)


if __name__ == "__main__":
    sys.exit(main())
