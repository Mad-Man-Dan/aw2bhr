#!/usr/bin/env python3
"""Give a RAM address a linker symbol, so C code can name it.

C can only name a RAM address that has a symbol. aw2bhr.lds defines them in
its EWRAM and IWRAM blocks, one per line, in address order:

    . = 0x00C528; gUnknown_0200C528 = .;

A function that reads an address with no symbol cannot be written in C
without going through a pool word, and that changes the code it compiles
to (sub_0801A718 and 0x0200C618). This adds the line in the right place:

    python tools/add_ram_symbol.py 0x0200C618
    python tools/add_ram_symbol.py 0x0200C618 --name gFoo --dry-run

It refuses when the address sits next to a line it does not understand
(libgcc placements, the SPLIT_RAM_OBJ blocks gen_lds.py replaces); edit
those by hand. The sections are NOLOAD, so a symbol changes no ROM byte.
Afterwards it re-runs gen_lds.py, which copies the line into
aw2bhr.split.lds. Declare the symbol in include/unknown-globals.h yourself;
its type is a decision about the code that uses it.
"""
import argparse
import os
import re
import subprocess
import sys

import awlib

LDS = os.path.join(awlib.REPO, "aw2bhr.lds")
SECTIONS = {"EWRAM": (0x02000000, 0x02040000), "IWRAM": (0x03000000, 0x03008000)}
SYM_LINE = re.compile(r'^\t\t\. = 0x([0-9A-F]{6});((?: [A-Za-z_]\w* = \.;)+)$')
NAME = re.compile(r'^[A-Za-z_]\w*$')


def section_span(lines, section):
    """(first, last) line index of the section's body, exclusive of braces."""
    head = next(i for i, l in enumerate(lines) if l.strip().startswith(section + " "))
    open_ = next(i for i in range(head, len(lines)) if lines[i].strip() == "{")
    close = next(i for i in range(open_, len(lines)) if lines[i] == "\t}")
    return open_ + 1, close


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument("address")
    ap.add_argument("--name", help="default gUnknown_<ADDRESS>")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args(argv)

    addr = int(a.address, 16)
    name = a.name or "gUnknown_%08X" % addr
    if not NAME.match(name):
        print("error: %r is not a C identifier" % name)
        return 2
    section = next((s for s, (lo, hi) in SECTIONS.items() if lo <= addr < hi), None)
    if section is None:
        print("error: 0x%08X is not in EWRAM or IWRAM" % addr)
        return 2
    base = SECTIONS[section][0]

    with open(LDS, encoding="utf-8", newline="") as fh:
        text = fh.read()
    if re.search(r'(^|[\s;])%s = ' % re.escape(name), text):
        print("%s is already defined in aw2bhr.lds; nothing to do" % name)
        return 0
    lines = text.split("\n")
    first, last = section_span(lines, section)

    # Symbol lines in the section, with their offsets, and a monotonic check.
    syms, prev, in_split = [], -1, False
    for i in range(first, last):
        # gen_lds.py replaces SPLIT_RAM_OBJ blocks in the split build, so a
        # symbol inside one would exist in only one of the two builds.
        if "END_SPLIT_RAM_OBJ" in lines[i]:
            in_split = False
            continue
        if "SPLIT_RAM_OBJ" in lines[i]:
            in_split = True
        m = None if in_split else SYM_LINE.match(lines[i])
        if m:
            off = int(m.group(1), 16)
            if off < prev:
                print("error: aw2bhr.lds line %d is out of address order; fix "
                      "that first" % (i + 1))
                return 2
            prev = off
            syms.append((i, off))
    off = addr - base
    same = [i for i, o in syms if o == off]
    if same:
        i = same[0]
        new_line = lines[i] + " %s = .;" % name
        where = "added to line %d, which already defines 0x%08X" % (i + 1, addr)
    else:
        before = [i for i, o in syms if o < off]
        after = [i for i, o in syms if o > off]
        if not before or not after:
            print("error: 0x%08X is outside the range of the section's symbol "
                  "lines; add it by hand" % addr)
            return 2
        i, j = before[-1], after[0]
        if j != i + 1:
            print("error: lines %d-%d between the neighbouring symbols are not "
                  "plain symbol lines (a placement or a SPLIT_RAM_OBJ block); "
                  "add 0x%08X by hand" % (i + 2, j, addr))
            return 2
        new_line = "\t\t. = 0x%06X; %s = .;" % (off, name)
        where = "inserted after line %d (%s)" % (i + 1, lines[i].strip())
    print("%s: %s" % (name, where))
    print("  " + new_line.strip())
    if a.dry_run:
        return 0
    if same:
        lines[i] = new_line
    else:
        lines.insert(i + 1, new_line)
    with open(LDS, "w", encoding="utf-8", newline="") as fh:
        fh.write("\n".join(lines))

    r = subprocess.run([sys.executable, os.path.join(awlib.REPO, "tools", "gen_lds.py")],
                       capture_output=True, text=True, cwd=awlib.REPO)
    split = os.path.join(awlib.REPO, "aw2bhr.split.lds")
    ok = r.returncode == 0 and name in open(split, encoding="utf-8").read()
    print("gen_lds.py: %s" % ("aw2bhr.split.lds regenerated with the symbol" if ok
                              else "FAILED\n" + (r.stdout + r.stderr)[-1500:]))
    print("next: declare %s in include/unknown-globals.h, then run both "
          "builds" % name)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
