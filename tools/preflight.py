#!/usr/bin/env python3
"""Wave pre-flight: every standing check, one command, one compact summary.

Added in wave 20 after the token-usage review: the checks below used to be ~10
separate orchestrator turns of heredocs re-pasted from SKILL.md each wave, and
each turn replays the whole session context. Run this instead:

    python tools/preflight.py            # everything (parked re-test included)
    python tools/preflight.py --skip-parked   # fast run, skip the re-test
    python tools/preflight.py --retest sub_A sub_B   # only the re-test, on these

What it does, in SKILL.md section-1 order:
  1. regenerates data/families.json (families.py, with its self-validation)
  2. prints the three progress metrics and which one titles the commit
  3. screens the straight-line band (>= 256 B, backward_branches == 0)
  4. sweeps work/*/best.json for unpromoted near-misses (nothing else reads it)
  5. re-tests every data/parked.json entry BY EXIT CODE, and prints a draft
     that does not COMPILE as COMPILE-FAIL with its first error, never as an
     ordinary `still-fails` (wave 90: 46 of 101 drafts after PR #3's renames)
  5b. lists parked drafts that read a compiler-made pool word instead of
     naming the global it points at (tools/lc_screen.py --drafts)
  6. prints the current draft-residual queue (including unmeasured drafts)
  7. reminds you to inspect overlap_screen.py's full locality/shape report too

Exit status is 0 unless families.py's self-validation fails, which is the one
condition under which batching must not proceed.
"""

import argparse
import glob
import hashlib
import json
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
os.chdir(REPO)
sys.path.insert(0, HERE)

PY = sys.executable


def run(args, **kw):
    return subprocess.run([PY] + args, capture_output=True, text=True, **kw)


def sect(title):
    print("\n=== %s ===" % title)


def better_base(name, v):
    """A note when best.c (or a recovered.c) beats the draft just scored.

    The re-test scores the draft only, and 2026-09-26 found 27 parked
    functions whose draft sat below another file in the same folder --
    sub_0801D390 at 8.76% beside a 62.97% size-exact best.c. An agent handed
    the draft starts from the worse one unless something says so.
    """
    notes = []
    try:
        # A best.json outlives a best.c renamed *.wrongc after review; its
        # score then describes a file judged wrong, so say nothing.
        if not os.path.exists(os.path.join("work", name, "best.c")):
            raise OSError("no best.c")
        with open(os.path.join("work", name, "best.json"), encoding="utf-8") as fh:
            b = json.load(fh)
        sd = b.get("size_delta")
        if sd is not None and (abs(sd) < abs(v["size"])
                               or (abs(sd) == abs(v["size"])
                                   and b.get("percent", 0) > v["pct"] + 0.5)):
            notes.append("best.c %.2f%% size%+d" % (b["percent"], sd))
    except (OSError, ValueError, KeyError):
        pass
    if os.path.exists(os.path.join("work", name, "recovered.c")):
        notes.append("recovered.c")
    if not notes:
        return ""
    return "   <- better: %s (drafts.py bases %s)" % (", ".join(notes), name)


def retest_parked(names):
    """Re-test each name by EXIT CODE, telling a compile failure from a miss.

    Wave 90: after PR #3's renames 46 of 101 drafts did not COMPILE and this
    printed `still-fails` for them exactly as for a 99% near-miss. trymatch's
    exit 1 covers both; its final TRYMATCH line tells them apart.
    """
    import trymatch
    compile_fail, no_verdict = [], []
    for name in names:
        r = run(["tools/trymatch.py", name])
        v = trymatch.parse_result(r.stdout) or {"state": "ERROR"}
        if r.returncode == 0:
            print("  UNPARKED %s -- matches now, promote it" % name)
        elif v["state"] == "COMPILE-FAIL":
            err = next((ln.strip() for ln in r.stdout.splitlines()
                        if re.search(r':\d+: ', ln) and "In function" not in ln
                        and "treated as errors" not in ln), "")
            print("  COMPILE-FAIL %s :: %s" % (name, err[:150]))
            compile_fail.append(name)
        elif v["state"] == "MISMATCH":
            print("  still-fails %-16s %6.2f%% size%+d%s"
                  % (name, v["pct"], v["size"], better_base(name, v)))
        else:
            print("  %s %s -- no verdict (exit %d)" % (v["state"], name, r.returncode))
            no_verdict.append(name)
    if compile_fail:
        print("  !! %d PARKED DRAFT(S) DO NOT COMPILE. They are not near-misses and"
              % len(compile_fail))
        print("  !! have no score. After a header rename, port them by offset:")
        print("  !!   python tools/port_rename.py <fn> --old-rev <pre-rename commit>")
        print("  !! %s" % " ".join(compile_fail))
    if no_verdict:
        print("  !! %d function(s) gave NO verdict: %s"
              % (len(no_verdict), " ".join(no_verdict)))
    return compile_fail, no_verdict


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--skip-parked", action="store_true",
                    help="skip the parked-queue re-test (the slow step)")
    ap.add_argument("--skip-queue", action="store_true",
                    help="skip the residual queue generated by overlap_screen")
    ap.add_argument("--retest", nargs="+", metavar="FN",
                    help="run ONLY the parked-style re-test on these functions")
    args = ap.parse_args()
    if args.retest:
        sect("re-test (exit code, compile failures separated)")
        retest_parked(args.retest)
        return 0

    # 0. The wave number. Nothing else prints it, it is not derivable from the
    # metrics, and a wrong one lands in every comment the wave's agents write
    # -- a session once numbered itself 25 against an existing wave-25 commit
    # and 40+ annotations had to be renumbered surgically (SKILL.md rule 5).
    sect("last commits (the wave number comes from HERE, nowhere else)")
    g = subprocess.run(["git", "log", "--oneline", "-3"],
                       capture_output=True, text=True)
    print(g.stdout.strip() or "(git log unavailable)")
    m = re.search(r"Wave (\d+):", g.stdout)
    if m:
        print("=> last wave committed: %s -- this wave is %d"
              % (m.group(1), int(m.group(1)) + 1))

    # 1. families.py -- regenerate FIRST; stale clusters waste the wave.
    sect("families.py (regenerated, self-validated)")
    r = run(["tools/families.py"])
    tail = (r.stdout + r.stderr).strip().splitlines()
    for line in tail[-6:]:
        print(line)
    families_ok = r.returncode == 0 and "[FAIL]" not in r.stdout + r.stderr
    if not families_ok:
        print("!! families.py validation FAILED -- do not batch on this")

    # 2. The three progress metrics.
    sect("progress metrics")
    data = json.load(open("data/functions.json", encoding="utf-8"))
    from collections import Counter
    counts = Counter(f["status"] for f in data)
    matched = [f for f in data if f["status"] == "matched"]
    total_b = sum(f["size"] for f in data)
    match_b = sum(f["size"] for f in matched)
    print("status:", dict(counts))
    print("index metric  : %d / %d functions, %d / %d bytes (%.2f%%)"
          % (len(matched), len(data), match_b, total_b, 100.0 * match_b / total_b))
    import index_functions as ix
    cdef = len(ix.decompiled_symbols())
    print("C-def metric  : %d   <- commit TITLES use this one" % cdef)
    print("(progress_map.py prints a third, larger byte figure -- it counts")
    print(" src/proc.c / src/title-screen.c too. Never quote it as progress.)")

    # 3. Straight-line band.
    sect("straight-line band (>= 256 B, backward_branches == 0)")
    band = [f for f in data
            if f["status"] == "asm" and f["mode"] == "THUMB"
            and f["size"] >= 256 and f["backward_branches"] == 0]
    band.sort(key=lambda f: -f["size"])
    print("%d funcs, %d bytes" % (len(band), sum(f["size"] for f in band)))
    for f in band[:15]:
        print("  %-16s %5dB calls=%-2d refs=%-2d %s"
              % (f["name"], f["size"], len(f["calls"]),
                 len(f["data_refs"]), f["src"]))

    # 4. best.json sweep -- wave 20 found a finished 392-byte function here.
    sect("work/*/best.json near-misses (unmatched only)")
    st = {f["name"]: f["status"] for f in data}
    rows = []
    sizes = {f["name"]: f["size"] for f in data}
    for p in glob.glob("work/*/best.json"):
        fn = os.path.basename(os.path.dirname(p))
        if st.get(fn) == "matched":
            continue
        try:
            b = json.load(open(p, encoding="utf-8"))
        except (OSError, ValueError):
            continue
        pct = b.get("percent", b.get("pct", -1))
        delta = b.get("size_delta")
        delta_source = "best" if delta is not None else None
        cand = os.path.join(os.path.dirname(p), "_cand.bin")
        if delta is None and os.path.exists(cand) and fn in sizes:
            delta = os.path.getsize(cand) - sizes[fn]
            delta_source = "current"
        rows.append((pct, fn, st.get(fn, "?"), delta, delta_source))
    rows.sort(key=lambda t: -t[0])

    # Does best.c actually correspond to the score recorded beside it? Before
    # source_sha1 (added 2026-08-29) nothing did, and the answer was
    # unknowable: record_best is a non-atomic read-modify-write and the MCP
    # server rewrites <fn>.c per call, so a parallel wave can leave best.c
    # holding a source that never scored. Entries written before the field
    # exists stay unverifiable -- say so rather than implying they passed.
    verified = mismatched = legacy = 0
    bad = []
    for p in glob.glob("work/*/best.json"):
        fn = os.path.basename(os.path.dirname(p))
        try:
            want = json.load(open(p, encoding="utf-8")).get("source_sha1")
        except (OSError, ValueError):
            continue
        if not want:
            legacy += 1
            continue
        bc = os.path.join(os.path.dirname(p), "best.c")
        if not os.path.exists(bc):
            mismatched += 1
            bad.append((fn, "best.c missing"))
            continue
        got = hashlib.sha1(open(bc, "rb").read()).hexdigest()
        if got == want:
            verified += 1
        else:
            mismatched += 1
            bad.append((fn, "best.c does not match its recorded source_sha1"))
    print("  best.c <-> best.json: %d verified, %d MISMATCHED, %d legacy "
          "(no source_sha1 -- unverifiable, not passing)"
          % (verified, mismatched, legacy))
    for fn, why in bad:
        print("  !! %s -- %s; the score is not evidence about this file" % (fn, why))

    hot = [r for r in rows if r[0] >= 90]
    for pct, fn, s, delta, delta_source in hot:
        size_note = ""
        if delta is not None and delta != 0:
            label = "BEST SIZE" if delta_source == "best" else "current SIZE"
            size_note = "  %s %+dB (prefix cannot match)" % (label, delta)
        elif delta == 0:
            size_note = ("  best size-exact" if delta_source == "best"
                         else "  current size-exact")
        if delta_source == "current":
            size_note += "; legacy best size unknown"
        print("  %6.2f%%  %-16s %s%s" % (pct, fn, s, size_note))
    print("(%d at >= 90%%, %d at 70-90%%; re-test anything here by exit code"
          % (len(hot), len([r for r in rows if 70 <= r[0] < 90])))
    print(" before believing it -- a score is not a match)")
    # Wave 31: the percentage counts REGISTER FIELDS. A THUMB register number
    # shares its halfword with the opcode, so a candidate whose instruction
    # stream is exactly right but whose allocation differs scores LOW --
    # sub_0800C124's structurally exact rebuild measured 89.4% against the
    # wrong-shaped draft it replaced at 92.4%. This list is therefore not a
    # ranking of correctness, and a fix that LOWERS the number is a normal
    # outcome, not a regression to revert.
    print("(percent counts register fields -- a structurally EXACT candidate")
    print(" can score below a structurally wrong one. Not a correctness rank.)")

    # 4b. Permuter contamination. tools/permute.py used to strand raw
    # header-expanded output in work/<fn>/ on abnormal exit; wave 17 fixed the
    # bug, but the artefacts it already wrote are still in the tree and are
    # invisible until someone opens one. Wave 31 lost time to two of them.
    # A blob is ~50KB of expanded headers; a MUTATED draft can be normal-sized
    # and is only findable by the permuter's injected `new_var`, so check both.
    sect("permuter-contaminated drafts (wave 17 residue, still present)")
    # Only unmatched functions: a matched function's draft is refreshed from
    # src/decomp by sync_work.py, and a permuter-found match keeps its
    # `new_var` there legitimately -- listing those buried the real cases.
    blobs, mutated = [], []
    for p in glob.glob("work/*/*.c"):
        if st.get(os.path.basename(os.path.dirname(p))) == "matched":
            continue
        try:
            with open(p, encoding="utf-8", errors="replace") as fh:
                text = fh.read()
        except OSError:
            continue
        if len(text) > 40000 and text.count("#") < 5:
            blobs.append((len(text), p))
        # Strip comments before looking for the injected symbol. A draft that
        # was CLEANED of a mutation names `new_var` in the comment explaining
        # what was removed -- sub_0800C124 does, after wave 31 rebuilt it -- and
        # flagging that would report the fix as the defect.
        code = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
        if re.search(r"\bnew_var\d*\b", code):
            mutated.append(p)
    for n, p in sorted(blobs, reverse=True):
        print("  BLOB     %7d B  %s" % (n, p))
    for p in sorted(mutated):
        # The dangerous case is the WORK DRAFT being gone, not best.c: promote
        # reads the draft, and best.c beside a good draft is merely ignorable.
        fn = os.path.basename(os.path.dirname(p))
        tag = "DRAFT" if os.path.basename(p) == fn + ".c" else "other file"
        print("  MUTATED  %-12s %s" % (tag, p))
    print("%d blob(s), %d mutated file(s), unmatched functions only. `DRAFT`"
          % (len(blobs), len(mutated)))
    print("means the draft itself carries permuter edits (`new_var`): read what")
    print("they changed before building on it. A blob is header-expanded output;")
    print("permute.splice() turns one back into a readable draft, and a blob")
    print("kept for reference is named *.c.blob so nothing scans it.")
    print("For any function here, `python tools/drafts.py bases <fn>` compiles")
    print("every candidate file in work/<fn>/ and names the real base (read-only).")

    # 5. Parked queue: format, staleness, then the exit-code re-test.
    sect("parked queue re-test (exit code, not message text)")
    raw = open("data/parked.json", "rb").read()
    # Wave 30: the file is CRLF and indent=1. Get either wrong and every one
    # of its ~380 lines re-serialises, burying a real five-entry addition in a
    # 425/379 diff. Catch it here rather than in `git diff` at commit time.
    fmt = []
    if raw.count(b"\r\n") == 0 and raw.count(b"\n") > 0:
        fmt.append("LF line endings (the file is CRLF in git)")
    body = raw.replace(b"\r\n", b"\n")
    first_key = re.search(rb'\{\n( +)"', body)
    if first_key and first_key.group(1) != b" ":
        fmt.append("top-level indent is %d spaces (the file is indent=1)"
                   % len(first_key.group(1)))
    if fmt:
        print("  !! parked.json FORMAT DRIFT: " + "; ".join(fmt))
        print("  !! rewrite with indent=1 and CRLF before committing, or the")
        print("  !! whole file churns and real changes become unreviewable")
    parked = json.load(open("data/parked.json", encoding="utf-8"))["functions"]
    # A parked entry for a function that is already MATCHED is a stale lie --
    # wave 30 had an agent park two functions that a later agent then matched,
    # and left in place they would suppress both from every future pre-flight.
    stale = [n for n in parked if st.get(n) == "matched"]
    for n in stale:
        print("  !! STALE %s -- parked but its status is MATCHED; delete the"
              " entry" % n)
    if args.skip_parked:
        print("skipped (--skip-parked)")
    else:
        retest_parked([n for n in parked if n not in stale])

    # 5b. Drafts that read a compiler-made pool word instead of naming the
    # global it points at. Renaming moved 4 of 4 such drafts in wave 91.
    sect("pool-word spellings in parked drafts (fix these first)")
    r = run(["tools/lc_screen.py", "--drafts"])
    print((r.stdout + r.stderr).rstrip())

    # 6. The residual kind is now the primary queue. Run it here so a plain
    # preflight cannot accidentally omit unmeasured drafts or loop candidates.
    queue_ok = True
    sect("current residual queue (primary post-wave-60 batching axis)")
    if args.skip_queue:
        print("skipped (--skip-queue)")
    else:
        r = run(["tools/overlap_screen.py", "--delta-only"])
        print((r.stdout + r.stderr).rstrip())
        queue_ok = r.returncode == 0
        if not queue_ok:
            print("!! residual screen FAILED -- do not batch from stale output")

    # 7. The full report is longer and still needs coordinator judgement.
    sect("remaining manual batching view")
    print("python tools/overlap_screen.py   <- current defaults are min-size 24,")
    print("loops included, block-min-matched 1, and top-blocks 12. Inspect its")
    print("locality/shape output before assigning batches; check --self-test.")

    return 0 if families_ok and queue_ok else 1


if __name__ == "__main__":
    sys.exit(main())
