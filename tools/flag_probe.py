#!/usr/bin/env python3
"""Test a compiler-flag change on a function AND its neighbours.

A draft that matches only with a changed flag (say `-O2 -fno-gcse`) is not
done. The original game set flags per source FILE, so the flag is only a
plausible explanation if the functions that shared that file also match
with it. This compiles every function near the target with the change and
shows where the results turn:

    python tools/flag_probe.py sub_0805D438 --cflags-add=-fno-gcse
    python tools/flag_probe.py sub_0808AAF4 --cflags-remove=-O2,-fforce-addr \\
        --cflags-add=-O1 --around 20

Each function is compiled with its own configured flags (including any entry
in data/compiler-overrides.json) plus the change. Promoted functions are
known to match without it; for the others both columns are measured. The
report names the nearest promoted function on each side that the change
BREAKS: if the game compiled a file with this flag, that file lay between
them. Everything compiles into build/drafts/; nothing in work/ or src/ is
touched. A match here is evidence, not a verdict: record an override in
data/compiler-overrides.json and verify with trymatch.py.
"""
import argparse
import json
import os
import sys

import agbenv
import awlib
import drafts


def probe(fn, profile):
    src = drafts.draft_rel(fn)
    if not os.path.exists(drafts.absp(src)):
        return {"state": "NO-DRAFT"}
    b = drafts.build(fn, src, "flagprobe-" + profile, profile=profile)
    return drafts.score(fn, b)


def short(r):
    st = r["state"]
    if st == "MATCH":
        return "MATCH"
    if st == "MISMATCH":
        return "%.2f%% size%+d" % (r["pct"], r["size_delta"])
    return st


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("fn")
    ap.add_argument("--cflags-add", action="append", default=[], metavar="=FLAG")
    ap.add_argument("--cflags-remove", action="append", default=[], metavar="=FLAG")
    ap.add_argument("--around", type=int, default=12,
                    help="functions to test on each side (default 12)")
    a = ap.parse_args(argv)
    add = [x for v in a.cflags_add for x in v.split(",") if x]
    remove = [x for v in a.cflags_remove for x in v.split(",") if x]
    if not add and not remove:
        ap.error("give --cflags-add=FLAG and/or --cflags-remove=FLAG")

    recs = json.load(open(os.path.join(awlib.DATA_DIR, "functions.json"),
                          encoding="utf-8"))
    recs.sort(key=lambda r: r["addr"])
    k = next((i for i, r in enumerate(recs) if r["name"] == a.fn), None)
    if k is None:
        print("error: %s is not in data/functions.json" % a.fn)
        return 2
    window = recs[max(0, k - a.around):k + a.around + 1]

    change = "; ".join((["adding " + " ".join(add)] if add else [])
                       + (["removing " + " ".join(remove)] if remove else []))
    print("flag probe: %s; %d function(s) each side of %s\n"
          % (change, a.around, a.fn))
    print("  %-10s  %-14s %-12s  %-20s %s" % ("address", "function", "status",
                                               "configured", "with the change"))
    rows = []
    for r in window:
        fn, status = r["name"], r["status"]
        if status == "asm-resident":
            res = (None, None)
        else:
            try:
                prof = agbenv.custom_profile(add, remove, base="configured", fn=fn)
            except ValueError as exc:
                ap.error(str(exc))
            now = probe(fn, prof)
            base = ({"state": "MATCH"} if status == "matched"
                    else probe(fn, "configured"))
            res = (base, now)
        rows.append((r, res))
        base, now = res
        print("  %-10s  %-14s %-12s  %-20s %s%s"
              % (r["addr_hex"], fn, status,
                 "(assembly only)" if base is None else short(base),
                 "" if now is None else short(now),
                 "   <- target" if fn == a.fn else ""), flush=True)

    def breaks(row):
        r, (base, now) = row
        return (r["status"] == "matched" and now is not None
                and now["state"] != "MATCH")

    t = next(i for i, (r, _) in enumerate(rows) if r["name"] == a.fn)
    left = next((i for i in range(t - 1, -1, -1) if breaks(rows[i])), None)
    right = next((i for i in range(t + 1, len(rows)) if breaks(rows[i])), None)
    tgt_now = rows[t][1][1]

    print("\nReading it:")
    if tgt_now is None:
        print("  %s is assembly-only; nothing to test." % a.fn)
        return 0
    print("  %s with the change: %s" % (a.fn, short(tgt_now)))
    for side, i in (("before", left), ("after", right)):
        if i is None:
            print("  No promoted function %s it in this window breaks under the "
                  "change (widen --around to look further)." % side)
        else:
            print("  Nearest promoted function %s it that the change breaks: %s "
                  "(%s)." % (side, rows[i][0]["name"], short(rows[i][1][1])))
    lo = 0 if left is None else left + 1
    hi = len(rows) - 1 if right is None else right - 1
    inside = rows[lo:hi + 1]
    fixed = [r["name"] for r, (b, n) in inside
             if b is not None and b["state"] != "MATCH" and n["state"] == "MATCH"]
    print("  If the game built one file with this change, it lay within %s .. %s "
          "(%d functions)." % (rows[lo][0]["name"], rows[hi][0]["name"], len(inside)))
    if left == t - 1 and right == t + 1:
        print("  Both immediate neighbours break, so that file would hold %s "
              "alone. That is weak evidence for the flag." % a.fn)
    if fixed:
        print("  Unmatched functions in that range that the change makes match: %s"
              % ", ".join(fixed))
    still = ["%s (%s)" % (r["name"], short(n)) for r, (b, n) in inside
             if b is not None and b["state"] != "MATCH" and n["state"] != "MATCH"]
    if still:
        print("  Unmatched functions in that range that still do not match with "
              "it: %s. If they shared the file, they need the change too."
              % ", ".join(still))
    print("  To use it: record the override for each function in "
          "data/compiler-overrides.json, promote them into files of their own "
          "(flags apply per object file), and verify with trymatch.py.")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
