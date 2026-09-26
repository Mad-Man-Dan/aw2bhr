"""Compile a draft with agbcc's RTL dump flags and keep the dumps.

    python tools/rtldump.py <fn> [--flags=-dg] [--out DIR]

Runs the same cpp and cc1 command line as trymatch.py (per-function overrides
included) with extra flags, default `-dg`, and writes the dumps to
work/<fn>/rtl/ as dump.<pass> next to out.s. `-dg` is the post-reload (.greg)
dump: it lists the spill registers per insn and shows which register each
reload got. `-dl` (local-alloc), `-dc` (combine), `-dJ` (jump2) and friends
work the same way. See "A RELOAD register is chosen ROUND-ROBIN" in
docs/agbcc-codegen.md.
"""

import argparse
import os
import shlex
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import agbenv  # noqa: E402
import awlib  # noqa: E402


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("name")
    ap.add_argument("--flags", default="-dg",
                    help="cc1 dump flags; write as --flags=-dg")
    ap.add_argument("--out")
    ap.add_argument("--src", help="source file (default work/<fn>/<fn>.c)")
    a = ap.parse_args()
    src = a.src or os.path.join("work", a.name, a.name + ".c")
    out = a.out or os.path.join(awlib.REPO, "work", a.name, "rtl")
    os.makedirs(out, exist_ok=True)
    # Remove the previous run's files first, so a failed compile cannot leave
    # them behind to be read as this draft's dumps.
    for name in os.listdir(out):
        if name.startswith("dump.") or name in ("in.i", "out.s"):
            os.remove(os.path.join(out, name))
    f = agbenv.flags(a.name)
    od = shlex.quote(agbenv.wsl_path(out))
    script = ('set -eo pipefail\n'
              '{cpp} {cppflags} {src} | iconv -f UTF-8 -t CP932 > {od}/in.i\n'
              '{cc1} {cflags} {extra} -dumpbase {od}/dump {od}/in.i -o {od}/out.s\n'
              ).format(od=od, cpp=f["CPP"], cppflags=f["CPPFLAGS"],
                       src=shlex.quote(src.replace("\\", "/")), cc1=f["CC1"],
                       cflags=f["CFLAGS"], extra=a.flags)
    rc, _, err = agbenv.run(script)
    if rc != 0:
        sys.stderr.write(err or "rtldump: the compile failed (exit %d)\n" % rc)
        return 1
    for name in sorted(os.listdir(out)):
        print(os.path.join(out, name))
    return 0


if __name__ == "__main__":
    sys.exit(main())
