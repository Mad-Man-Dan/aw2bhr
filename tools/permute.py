#!/usr/bin/env python3
"""Brute-force a near-match with decomp-permuter, then judge it with trymatch.

This exists for one failure mode. A candidate can be semantically correct,
compile to the right instructions in the right order, and still not match
because a value landed in the wrong register -- and register allocation is
decided by phrasing the source rather than by what it computes, so it is
reachable by rewriting and not by reasoning. `docs/agbcc-codegen.md` records two
functions where fourteen and eight rewrites respectively all produced identical
output. The permuter searches that space mechanically.

Two things are worth being precise about.

**The permuter's score is not the verdict.** It scores by diffing objdump text,
so it ignores encoding details and stops at the boundary of what objdump prints.
Every candidate it produces is re-checked here with tools/trymatch.py, which
compares the encoded bytes and the relocations. A permuter score of 0 that
trymatch rejects is a real outcome, not a bug.

Measured, the two are not even monotonically related: on both blocked functions
the permuter's best-scoring candidate was a byte-level *regression* -- 35 -> 30
while bytes went 80% -> 75%, and 220 -> 80 while bytes went 88.2% -> 55.9%.
That is why the original source is restored when nothing matches, and why
best.c is only ever moved forward by trymatch's own measurement.

**target.o must contain the target function and nothing else.** The scorer
disassembles the whole object, so a multi-function unit would make every score
meaningless while still looking like it worked. This refuses to run in that case
rather than produce numbers that cannot be trusted.

    python tools/permute.py sub_08063980 --seconds 600
    python tools/permute.py sub_08001158 --setup-only   # inspect, do not run

The vendored permuter is not committed; see vendor/README.md.

**Output (wave 90).** The search's progress is no longer echoed: the
permuter's carriage-return counter is ~400 KB a minute and several agents
redirect it into their own logs. It goes to work/<fn>/permuter/permuter.log,
compacted (every message kept; the iteration counter as one snapshot a minute
plus the final one). Each candidate's full trymatch report goes to
permuter/verify.log; the console gets one line per candidate. `--live` restores
the old raw echo. The run ends with one line to read:

    PERMUTE MATCH sub_X
    PERMUTE IMPROVED sub_X 93.68 -> 95.11 (kept in work/sub_X/sub_X.c)
    PERMUTE NO-IMPROVEMENT sub_X
    PERMUTE SETUP-FAILED sub_X

**Threads default to 4**, not every core: agents share this machine and each
runs up to three searches.

**Only a readable candidate is kept in the draft.** An improvement is written
to <fn>.c only in its spliced form (the draft's own includes plus the new
function). If only the header-expanded raw form improves, it goes to
permuter/best-raw.c and best.c/best.json are put back as they were -- that path
is how 36 best.c files became 190 KB blobs (preflight's BLOB list).
"""

import argparse
import contextlib
import io
import json
import os
import re
import shutil
import sys

import agbenv
import awlib
import trymatch

PERMUTER_DIR = os.path.join(awlib.REPO, "vendor", "decomp-permuter")
WORK = os.path.join(awlib.REPO, "work")

CLONE_HINT = (
    "vendor/decomp-permuter is missing. It is not committed -- clone it with:\n"
    "    git clone https://github.com/WhenGryphonsFly/decomp-permuter-agbcc.git \\\n"
    "        vendor/decomp-permuter\n"
    "See vendor/README.md for the pinned commit and the WSL packages it needs.")

# The compile pipeline, byte-identical to agbenv.compile_c and so to the
# Makefile's own recipe. `cd` is required because the permuter invokes this from
# whatever directory it was started in, with absolute temp paths for input and
# output, while the compiler flags are all relative to the repo.
COMPILE_SH = """#!/bin/bash
set -e
IN="$1"
OUT="$3"
cd {repo}
S="$OUT.s"
trap 'rm -f "$S"' EXIT
{cpp} {cppflags} "$IN" | iconv -f UTF-8 -t CP932 | {cc1} {cflags} -o "$S"
printf '.text\\n\\t.align\\t2, 0\\n' >> "$S"
{as_} {asflags} "$S" -o "$OUT"
{strip} -N .gcc2_compiled. "$OUT"
"""


def shq(s):
    return "'" + s.replace("'", "'\\''") + "'"


def definition_line(lines, name):
    """Index of the line where `name`'s definition starts, or None.

    A prototype is excluded by the trailing semicolon -- `unknown-functions.h`
    declares many of these functions, so after preprocessing the declaration and
    the definition both appear and only the second one is wanted.
    """
    rx = re.compile(r'^[A-Za-z_].*\b%s\s*\(' % re.escape(name))
    for i, ln in enumerate(lines):
        if rx.match(ln) and not ln.rstrip().endswith(";"):
            return i
    return None


def pick_source(workdir, name, prefer_best):
    """The C to start permuting from.

    best.c is the highest byte score ever reached, which is not necessarily the
    file left behind by the last iteration; starting anywhere else throws away
    the work that got us close enough for this to be worth running.

    `<name>.perm.c`, if present, wins over both. That file is the ONLY way to
    reach the permuter's directed search: PERM_GENERAL / PERM_RANDOMIZE /
    PERM_LINESWAP / PERM_INT and the rest (ten forms, src/perm/parse.py) are
    scanned TEXTUALLY out of base.c, so they survive `cpp -P` -- but they are
    not valid C, so a draft carrying them cannot be compiled or trymatched.
    They therefore cannot live in <name>.c, and until this existed there was
    nowhere else to put them. The measured consequence: zero PERM_ macros
    anywhere in the project, so _rec_perm_parse always yielded a single
    TextPerm and EVERY permuter result ever banked here came from undirected
    random fallback -- including the 26,041- and 23,107-iteration runs.

    Candidates come back as ordinary C (the permuter resolves the macros when
    it emits), so the rest of the pipeline is unchanged.
    """
    perm = os.path.join(workdir, name + ".perm.c")
    if os.path.exists(perm):
        return perm, name + ".perm.c"
    cur = os.path.join(workdir, name + ".c")
    best = os.path.join(workdir, "best.c")
    if prefer_best and os.path.exists(best):
        # A best.c the permuter produced can be wrong C that happens to score
        # well (a do/while(0) wrapper, `by = (ax *= 2)`). Starting from it
        # builds on the mistake, so the draft is used instead unless asked.
        try:
            with open(os.path.join(workdir, "best.json"), encoding="utf-8") as fh:
                origin = json.load(fh).get("origin")
        except (OSError, ValueError):
            origin = None
        if origin != "permuter" or prefer_best == "any" or not os.path.exists(cur):
            return best, "best.c"
        print("  NOTE     best.c came from an earlier permuter run and may be "
              "wrong C;")
        print("           starting from %s.c instead (--from-permuter-best to "
              "override)" % name)
    if os.path.exists(cur):
        return cur, name + ".c"
    return None, None


def setup(rec, unit, prefer_best):
    """Build work/<name>/permuter/ and return its path, or None on failure."""
    name = rec["name"]
    workdir = os.path.join(WORK, name)
    pdir = os.path.join(workdir, "permuter")

    src, which = pick_source(workdir, name, prefer_best)
    if src is None:
        print("error: no C to permute -- run `python tools/newfunc.py %s` first"
              % name)
        return None

    if len(unit["functions"]) != 1:
        print("error: %s shares split unit %s with %s."
              % (name, unit["unit"],
                 ", ".join(f for f in unit["functions"] if f != name)))
        print("       target.o would contain those too and every score would be")
        print("       measured against the wrong code. Refusing to run.")
        return None

    os.makedirs(pdir, exist_ok=True)
    # Old results would be harvested as if this run had produced them.
    for entry in os.listdir(pdir):
        if entry.startswith("output-"):
            shutil.rmtree(os.path.join(pdir, entry), ignore_errors=True)

    unit_dir = unit.get("dir", "build/functions")
    unit_o = "%s/%s.o" % (unit_dir, unit["unit"])
    rc, _, se = agbenv.assemble("%s/%s" % (unit_dir, unit["file"]), unit_o)
    if rc != 0:
        print("error: could not assemble the original unit\n%s" % se[-1500:])
        return None
    shutil.copyfile(os.path.join(awlib.REPO, unit_o.replace("/", os.sep)),
                    os.path.join(pdir, "target.o"))

    # base.c must be a self-contained translation unit: the permuter parses it
    # with pycparser after running `cpp -P -nostdinc`, which resolves no include
    # paths, so anything left unexpanded here fails at load time.
    # flags(NAME), not flags(): compile.sh below is the permuter's whole notion
    # of how this function is built, so an unnamed call silently searches with
    # the DEFAULT toolchain against a function the ROM built with another one.
    # Every candidate is then scored under the wrong compiler and the run
    # reports "no candidate scored better" for a residual that was never
    # reachable. Wave 47 (W47-F): sub_08070F44 had been permuted for 300 s under
    # default agbcc while its block is old_agbcc, and the flash trio at
    # 0x0808B had 38,000 iterations spent at -O2 against an -O1 library.
    f = agbenv.flags(name, profile=PROFILE)
    rel_src = os.path.relpath(src, awlib.REPO).replace(os.sep, "/")
    rel_base = os.path.relpath(os.path.join(pdir, "base.c"),
                               awlib.REPO).replace(os.sep, "/")
    # pycparser cannot parse GCC attributes, so any source reaching hardware.h
    # died at load time with `Syntax error in base.c ... struct
    # __attribute__((aligned(4))) DispCnt` -- which took every function touching
    # a hardware register off the permuter entirely, including textbook cases
    # like sub_0806EB5C (exact size, one register swap). Define the attribute
    # away for the parse.
    #
    # base.c is also what compile.sh builds, so this is only safe if stripping
    # is byte-neutral. Measured before committing to it: seven promoted files
    # that include hardware.h, compiled both ways and compared on .text alone
    # -- all seven identical, c_08012420.c (364 B, uses REG_BG3CNT) included.
    # Re-measure if a struct ever needs alignment the ABI does not already give.
    rc, _, se = agbenv.run('%s %s -P -D"__attribute__(x)=" %s -o %s'
                           % (f["CPP"], f["CPPFLAGS"], shq(rel_src),
                              shq(rel_base)))
    if rc != 0:
        print("error: preprocessing %s failed\n%s" % (rel_src, se[-1500:]))
        return None

    sh = COMPILE_SH.format(
        repo=shq(agbenv.REPO_POSIX), cpp=f["CPP"], cppflags=f["CPPFLAGS"],
        cc1=f["CC1"], cflags=f["CFLAGS"], as_=f["AS"], asflags=f["ASFLAGS"],
        strip=f["STRIP"])
    # Written as bytes with LF endings on purpose. A CRLF shebang makes the
    # kernel look for an interpreter called "/bin/bash\r" and the script exits
    # 127 -- which the permuter reports as "every candidate failed to compile".
    with open(os.path.join(pdir, "compile.sh"), "wb") as fh:
        fh.write(sh.encode("utf-8"))

    awlib.write_text(os.path.join(pdir, "settings.toml"),
                     'func_name = "%s"\ncompiler_type = "gcc"\n' % name)

    rel_pdir = os.path.relpath(pdir, awlib.REPO).replace(os.sep, "/")
    # The permuter checks the executable bit itself and refuses to start
    # without it. NTFS under WSL usually reports 0777 anyway, but not when the
    # drive is mounted with metadata.
    agbenv.run("chmod +x %s/compile.sh" % shq(rel_pdir))

    print("set up %s" % rel_pdir)
    print("  source   %s" % which)
    if which.endswith(".perm.c"):
        print("  NOTE     %s is used INSTEAD of --current / best.c while it exists;"
              % which)
        print("           rename or delete it when this directed search is done.")
    print("  target   %s (%d bytes, %d function)"
          % (unit_o, rec["size"], len(unit["functions"])))
    return pdir


def base_score(pdir):
    """Compile and score the starting point without permuting anything.

    A failure here is a harness problem -- unparseable base.c, a broken
    compile.sh -- and is worth separating from "the search found nothing",
    because the two look identical from the outside.

    Scored with --stack-diffs, the way run() searches. Without it the scorer
    turns every sp offset into `addr(sp)`, so a draft whose only residual is
    two swapped spill slots (sub_08037A78, sub_0802AA78) scored 0 here and the
    run ended as BASE-SCORES-ZERO before a single mutation was tried.
    """
    rel = os.path.relpath(pdir, awlib.REPO).replace(os.sep, "/")
    # --debug writes debug_source.c and debug_compiled_object.o into the working
    # directory, which is the repo root. Left there, debug_source.c breaks the
    # Makefile's source discovery (see the comment on C_SRCS). Move them into
    # the permuter directory, where they are still available and harmless.
    rc, so, se = agbenv.run(
        "python3 tools/permuter_entry.py %s --debug --stack-diffs; rc=$?\n"
        "mv -f debug_source.c debug_compiled_object.o %s/ 2>/dev/null\n"
        "exit $rc" % (shq(rel), shq(rel)),
        timeout=300)
    text = so + se
    m = re.search(r'base score = (\d+)', text)
    if m is None:
        print("error: the permuter could not score the starting point.")
        for ln in text.strip().splitlines()[-20:]:
            print("  " + ln)
        return None
    return int(m.group(1))


def run(pdir, seconds, threads, live=False):
    rel = os.path.relpath(pdir, awlib.REPO).replace(os.sep, "/")
    # SIGINT first, so the permuter's own handler shuts the workers down and the
    # output directories are left complete. `-k 30` is not optional: measured, a
    # run took SIGINT at 900s and was still going at 2906s, holding every core
    # it had. Whatever swallows the signal -- the multiprocessing pool, most
    # likely -- a wall-clock limit that does not actually stop the process is
    # worse than no limit, because nothing is watching it any more.
    # --stack-diffs: without it the scorer normalises stack offsets away, so a
    # candidate that puts a spill at sp+16 instead of sp+24 scores as a tie.
    # Those are real byte differences and at least one park (sub_080627F4,
    # 99.2%, six differing bytes) is nothing BUT a stack-slot transposition --
    # a residual the permuter was structurally unable to see until now.
    cmd = ("timeout -s INT -k 30 %d python3 tools/permuter_entry.py "
           "%s --better-only --stop-on-zero --stack-diffs -j %d"
           % (seconds, shq(rel), threads))
    log = os.path.join(pdir, "permuter.log")
    if live:
        print("\nrunning for up to %ds with %d threads; live output below, also"
              % (seconds, threads))
        print("logged to %s\n" % os.path.relpath(log, awlib.REPO))
        rc, _ = agbenv.stream(cmd, log_path=log)
        return rc
    print("\nrunning for up to %ds with %d threads; progress is in %s"
          % (seconds, threads, os.path.relpath(log, awlib.REPO).replace(os.sep, "/")),
          flush=True)
    rc, kept = agbenv.stream_compact(cmd, log)
    lines = kept.splitlines()
    final = next((ln for ln in reversed(lines) if "iteration" in ln), None)
    news = [ln for ln in lines if "new best score" in ln or "better score" in ln]
    # 124 is `timeout` reaching the wall-clock limit: the normal end of a run.
    how = "at the time limit" if rc == 124 else "with exit %d" % rc
    print("search ended %s: %s; %d improving candidate(s) reported"
          % (how, final.split("] ", 1)[-1] if final else "no iterations recorded",
             len(news)))
    for ln in news[-5:]:
        print("  " + ln)
    return rc


def harvest(pdir):
    """(score, source path) for this run's outputs, best score first."""
    out = []
    for entry in sorted(os.listdir(pdir)):
        if not entry.startswith("output-"):
            continue
        src = os.path.join(pdir, entry, "source.c")
        if not os.path.exists(src):
            continue
        try:
            with open(os.path.join(pdir, entry, "score.txt")) as fh:
                score = int(fh.read().strip())
        except (OSError, ValueError):
            continue
        out.append((score, src))
    return sorted(out, key=lambda t: t[0])


def check_scorer_patch():
    """False if the vendored scorer still carries upstream's penalty weights.

    vendor/ is gitignored, so a re-clone silently restores PENALTY_REGALLOC = 5
    and the search goes back to treating a wrong register as a twelfth of a
    reordering -- which for a byte verdict is the wrong objective and produced
    two documented cases where the best-SCORING candidate was a byte-level
    regression. A run under the wrong objective still looks like it worked,
    and a warning scrolls past at the start of a long run, so the caller
    refuses to start unless --force is given.
    """
    p = os.path.join(PERMUTER_DIR, "src", "scorer.py")
    try:
        with open(p, encoding="utf-8") as fh:
            text = fh.read()
    except OSError:
        return True          # no permuter at all; the caller reports that
    if "AW2_PENALTY_REGALLOC" in text and "AW2_PENALTY_SIZE" in text:
        return True
    if "AW2_PENALTY_REGALLOC" in text:
        print("\n  !! vendor/decomp-permuter/src/scorer.py lacks the length penalty")
        print("     (AW2_PENALTY_SIZE): a size-exact draft's search will drift to")
        print("     shorter, worse candidates. See vendor/README.md.\n")
        return False
    print("\n  !! vendor/decomp-permuter/src/scorer.py is UNPATCHED --")
    print("     PENALTY_REGALLOC is upstream's 5 against PENALTY_REORDERING 60,")
    print("     so this search will trade register correctness for ordering and")
    print("     its best-scoring candidate may be a byte-level regression.")
    print("     See vendor/README.md; re-apply the patch before trusting a result.\n")
    return False


def helper_run_start(cand_lines, b):
    """Index of the first line of the helper run directly above `b`, or `b`.

    The randomizer's perm_inline pass EXTRACTS an expression into a synthesized
    function and inserts it with `ast.ext.insert(ast.ext.index(fn), fn_def)` --
    at the target's own index, so immediately above it, every time. Repeated
    applications stack into a contiguous run. The winning candidate then CALLS
    those helpers, so a graft that starts at the target's definition drops them
    and the result fails to compile with `implicit declaration of inline_fn`.

    That was the whole defect: the spliced form died, the raw header-expanded
    form was the only one left that scored, and best.c became 60-200KB of
    preprocessed source for 23 parked functions -- unpromotable, while the
    working draft had already been rolled back to the worse form.

    Upstream has the identical bug in trim_source() (src/helpers.py), a textual
    cut at the first mention of the function name; it escapes notice only
    because --no-context-output is off by default. There is no fix to inherit.

    Walk upward over whole definitions while the name looks synthesized. Names
    come from get_noncolliding_name, so they are inline_fn, inline_fn2, ...;
    match the prefix rather than the literal string.
    """
    i = b
    while i > 0:
        j = _definition_above(cand_lines, i)
        if j is None:
            break
        m = re.match(r'^[A-Za-z_][^;()]*?\b(\w+)\s*\(', cand_lines[j])
        if not m or not m.group(1).startswith("inline_fn"):
            break
        i = j
    return i


def _definition_above(lines, i):
    """Index of the definition whose body ends just above line `i`, or None."""
    k = i - 1
    while k >= 0 and not lines[k].strip():
        k -= 1
    if k < 0 or lines[k].rstrip() != "}":
        return None
    depth = 0
    while k >= 0:
        depth += lines[k].count("}") - lines[k].count("{")
        if depth == 0:
            # Walk back over the signature's own continuation lines.
            while k > 0 and not re.match(r'^[A-Za-z_]', lines[k]):
                k -= 1
            return k
        k -= 1
    return None


def splice(orig_lines, cand_lines, name):
    """The original file's preamble plus the permuted function and its helpers.

    The permuter's output is base.c -- every header expanded -- with the
    function rewritten. That compiles, but it is not something anyone would
    commit. Grafting the new function onto the original's includes and externs
    gives back a normal source file. Returns None if either side cannot be
    split, in which case the caller falls back to the raw output.

    NOT byte-neutral in general: the candidate's expanded preamble may also
    carry a DECLARATION the randomizer mutated in place -- perm_randomize_
    external_type and perm_randomize_function_type rewrite a global's or a
    function's type at an arbitrary index, weight 5 each under [gcc]. Those
    rewrites are deliberately NOT carried across: the project's real headers
    are the environment we have to match in. A candidate whose score depended
    on one will score lower here than it did in the permuter, and that is the
    correct answer -- the recorded score was never reachable. The 2026-08-29
    review measured this at 7 of 53 affected parks.
    """
    a = definition_line(orig_lines, name)
    b = definition_line(cand_lines, name)
    if a is None or b is None:
        return None
    a_end, b_end = definition_end(orig_lines, a), definition_end(cand_lines, b)
    if a_end is None or b_end is None:
        return None
    # Whatever followed the function in the draft (a `.thumb_set` alias, a
    # second function) comes from the draft. The permuter's copy of the file
    # can stop at the function's closing brace, and taking the candidate's
    # tail used to drop that code.
    return (orig_lines[:a] + cand_lines[helper_run_start(cand_lines, b):b_end + 1]
            + orig_lines[a_end + 1:])


def definition_end(lines, start):
    """Index of the line holding the closing brace of the definition that
    starts at `start`, or None. Braces inside comments are ignored."""
    from promote import strip_comments
    depth, opened, in_comment = 0, False, False
    for i in range(start, len(lines)):
        code, in_comment = strip_comments(lines[i], in_comment)
        depth += code.count("{") - code.count("}")
        opened = opened or "{" in code
        if opened and depth <= 0:
            return i
    return None


def _check_quietly(name, log):
    """trymatch.check with its report appended to `log`; (rc, result dict)."""
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        rc = trymatch.check(name, profile=PROFILE)
    with open(log, "a", encoding="utf-8") as fh:
        fh.write(buf.getvalue() + "\n")
    return rc, dict(trymatch.LAST_RESULT or {"state": "ERROR"})


def _snapshot(paths):
    out = {}
    for p in paths:
        try:
            with open(p, "rb") as fh:
                out[p] = fh.read()
        except OSError:
            out[p] = None
    return out


def _restore(snap):
    """Put files back exactly as snapshotted (absent files removed)."""
    changed = False
    for p, data in snap.items():
        try:
            with open(p, "rb") as fh:
                now = fh.read()
        except OSError:
            now = None
        if now == data:
            continue
        changed = True
        if data is None:
            os.remove(p)
        else:
            tmp = "%s.tmp%d" % (p, os.getpid())
            with open(tmp, "wb") as fh:
                fh.write(data)
            os.replace(tmp, p)
    return changed


# How many of a run's outputs verify() re-checks, best permuter score first.
# Each check is a full compile with the draft held as the candidate, and an
# unlucky run can leave hundreds: sub_0802F6A0 left 854 in wave 92, hours of
# checking with the draft sitting as expanded source the whole time. The rest
# stay on disk under permuter/output-*/.
MAX_VERIFY = 40

# The compiler profile every compile in a run uses: the permuter's own
# compile.sh, each trymatch check and the uninitialised-read check. A
# function whose block needs another toolchain and has no override entry
# yet (sub_08070F44, old_agbcc) could not be permuted at all before this.
# Under a temporary profile trymatch leaves best.c alone, so the kept form
# lives only in <fn>.c, and a match is provisional until an override
# entry names the profile.
PROFILE = "configured"


def improves(pct, size, best_pct, best_size):
    """Whether a candidate (pct, size delta) should replace the kept form.

    The right size comes first, as in `drafts.py bases`: a size-exact form is
    never given up for one of the wrong size, and it beats one of the wrong
    size at any score. The percentage alone installed a +4-byte candidate over
    a size-exact sub_0802F588 draft in wave 74 and again in wave 92.
    """
    if best_size == 0 and size != 0:
        return False
    if size == 0 and best_size not in (0, None):
        return True
    return pct > best_pct


def _wrongc_reason(name, base_path, cand_path):
    """Why tools/wrongc.py rejects `cand_path` against the run's starting draft
    (a semantic change: volatile, a clobbered counter, a lost call, a different
    result under differential testing), or None."""
    try:
        import wrongc
        return wrongc.rejects(name, cand_path, base_path)
    except Exception:
        return None


def verify(rec, pdir, keep_all):
    """Run the best candidates past trymatch and keep the first that matches.

    Returns (exit status, final-line state, detail).
    """
    name = rec["name"]
    workdir = os.path.join(WORK, name)
    csrc = os.path.join(workdir, name + ".c")
    orig = awlib.read_lines(csrc)
    vlog = os.path.join(pdir, "verify.log")
    open(vlog, "w").close()

    cands = harvest(pdir)
    if not cands:
        print("\nno candidate scored better than the starting point.")
        return 1, "NO-IMPROVEMENT", "no candidate scored better in the permuter"
    if len(cands) > MAX_VERIFY:
        print("\n%d outputs; checking the best %d by permuter score (--max-verify)."
              % (len(cands), MAX_VERIFY))
        cands = cands[:MAX_VERIFY]

    print("\n%d candidate(s) to check against trymatch (full reports: %s):"
          % (len(cands), os.path.relpath(vlog, awlib.REPO).replace(os.sep, "/")))

    # The loop below leaves each candidate in csrc while trymatch judges it, so
    # ANY abnormal exit -- timeout, Ctrl-C, an exception out of trymatch --
    # strands the last form written there. That form may be the header-expanded
    # permuter output, which compiles cleanly and would be promoted silently as
    # if it were the draft. So the draft is always restored to a KNOWN form on
    # the way out -- but which form is the point below.
    # One compile to establish where the draft actually stands. Deliberately
    # not read from best.json: that file is what this whole mechanism has been
    # corrupting, and a baseline taken from it would compare a candidate
    # against a number describing some other source.
    _, base = _check_quietly(name, vlog)
    base_pct = trymatch.LAST_PCT
    base_size = base.get("size")
    print("  baseline  %s" % trymatch.format_result(base))
    # Locals the draft itself might read before setting; a candidate may not
    # add to them (agbenv.uninitialized_reads).
    base_uninit = set(agbenv.uninitialized_reads(csrc, fn=name, profile=PROFILE))
    # wrongc.py compares each kept candidate with the draft as the run found it.
    wrongc_base = os.path.join(pdir, "wrongc-base.c")
    awlib.write_text(wrongc_base, "".join(orig))
    if base_pct is None:
        print("  draft does not compile or cannot be scored; improvements "
              "cannot be judged, so the original will be restored as before.")
    best_pct, best_lines, best_label = base_pct, None, None
    best_size = base_size
    raw_pct, raw_lines = base_pct, None
    best_files = [os.path.join(workdir, "best.c"), os.path.join(workdir, "best.json")]
    matched = False
    trymatch.RECORD_ORIGIN = "permuter"     # best.c written below is a mutation
    try:
        for score, src in cands:
            rel = os.path.relpath(src, awlib.REPO).replace(os.sep, "/")
            cand = awlib.read_lines(src)

            # Try the tidy form first. If the graft is wrong in any way the raw
            # output still holds the real result, so a failure here costs one
            # compile rather than the candidate.
            forms = []
            grafted = splice(orig, cand, name)
            if grafted is not None:
                forms.append(("spliced", grafted))
            forms.append(("raw", cand))

            for label, lines in forms:
                awlib.write_text(csrc, "".join(lines))
                # A raw form must never become best.c: that is how 36 best.c
                # files turned into 190 KB header-expanded blobs. Its score is
                # kept in permuter/best-raw.c instead.
                # Every form is checked with best.c snapshotted: trymatch
                # records any higher score in best.c by itself, and a spliced
                # form this loop then REFUSES (wrong size, or a read before a
                # set) used to stay there as the recommended base. Wave 94 found
                # three best.c files holding forms the permuter had rejected.
                # best.c keeps a form only if it is kept below or matches.
                snap = _snapshot(best_files)
                rc, r = _check_quietly(name, vlog)
                if label == "raw" and _restore(snap):
                    r["note"] = "best.c left as it was"
                print("  score %-6d %-7s %s%s" % (score, label, trymatch.format_result(r),
                                                  "  (" + r["note"] + ")" if r.get("note") else ""))
                if rc == 0:
                    matched = True
                    print("\nMATCH from the permuter -- the %s form is written to %s"
                          % (label, os.path.relpath(csrc, awlib.REPO)))
                    if lines is cand:
                        print("      this is the header-expanded form; reduce it to")
                        print("      an include plus externs before promoting.")
                    else:
                        bad = sorted(set(agbenv.uninitialized_reads(csrc, fn=name, profile=PROFILE))
                                     - base_uninit)
                        if bad:
                            print("      WARNING: it may read %s before setting it;"
                                  " the bytes match but the C is wrong -- fix the"
                                  " source before promoting." % ", ".join(bad))
                        why = _wrongc_reason(name, wrongc_base, csrc)
                        if why:
                            print("      WARNING: wrongc.py says this changes what the function"
                                  " does (%s); the bytes match but check the C." % why)
                    return 0, "MATCH", "%s form, permuter output %s" % (label, rel)
                # Not a match, but it may still be an IMPROVEMENT, and until
                # 2026-08-29 that was thrown away: the finally block restored
                # the pre-run draft unconditionally. 53 of 123 parked functions
                # ended up with a live draft scoring below their own recorded
                # best, one of them by 54 points across 26,041 iterations. Keep
                # the best SPLICED form in the draft; a raw-only improvement is
                # saved beside it, never written over the readable draft.
                pct = trymatch.LAST_PCT
                if pct is None or base_pct is None:
                    _restore(snap)
                    continue
                if label == "spliced" and not improves(pct, r.get("size"), best_pct, best_size):
                    _restore(snap)
                if label == "spliced" and improves(pct, r.get("size"), best_pct, best_size):
                    bad = sorted(set(agbenv.uninitialized_reads(csrc, fn=name, profile=PROFILE))
                                 - base_uninit)
                    if bad:
                        _restore(snap)
                        print("             not kept: reads %s before setting it"
                              % ", ".join(bad))
                        break
                    why = _wrongc_reason(name, wrongc_base, csrc)
                    if why:
                        _restore(snap)
                        print("             not kept: wrong C (tools/wrongc.py): %s" % why)
                        break
                    best_pct, best_lines, best_label = pct, lines, rel
                    best_size = r.get("size")
                    print("             improvement: %.2f%% -> %.2f%% size%+d (kept)"
                          % (base_pct, pct, best_size or 0))
                    break   # the raw form of this candidate is the same code
                if label == "raw" and pct > raw_pct:
                    raw_pct, raw_lines = pct, lines
    finally:
        trymatch.RECORD_ORIGIN = "draft"
        if not matched:
            if best_lines is not None:
                awlib.write_text(csrc, "".join(best_lines))
            else:
                awlib.write_text(csrc, "".join(orig))

    if raw_lines is not None and (best_lines is None or raw_pct > best_pct):
        raw_path = os.path.join(pdir, "best-raw.c")
        awlib.write_text(raw_path, "".join(raw_lines))
        print("\nonly the HEADER-EXPANDED form of a candidate reached %.2f%%; it is in %s,"
              % (raw_pct, os.path.relpath(raw_path, awlib.REPO).replace(os.sep, "/")))
        print("not in the draft. Its gain usually rests on a declaration the permuter")
        print("retyped in the expanded headers, which the real headers will not allow.")
        print("Read what it changed before building on it.")
    if best_lines is not None:
        print("\nno candidate matched at the byte level, but %s IMPROVED "
              "%.2f%% -> %.2f%% and is kept (from %s)."
              % (os.path.relpath(csrc, awlib.REPO), base_pct, best_pct, best_label))
        print("      read what the mutation did before building on it: an")
        print("      improvement can be semantically wrong C.")
        detail = "%.2f -> %.2f (kept in %s)" % (
            base_pct, best_pct, os.path.relpath(csrc, awlib.REPO).replace(os.sep, "/"))
        state = "IMPROVED"
    else:
        print("\nno candidate matched or improved; %s restored unchanged."
              % os.path.relpath(csrc, awlib.REPO))
        state, detail = "NO-IMPROVEMENT", ("raw form only: %.2f in permuter/best-raw.c"
                                           % raw_pct if raw_lines is not None else "")
    if not keep_all:
        print("output directories kept under %s for inspection."
              % os.path.relpath(pdir, awlib.REPO))
    return 1, state, detail


def self_test():
    """The parts that need no compile: the log compactor, the splice, the
    thread default. Writes nothing under work/."""
    ok = True
    # 1. Carriage-return progress is compacted; messages survive whole.
    stream = ("Loading...\\n[f] base score = 720\\n"
              + "".join("iteration %d, 0 errors, score = 720\\b\\b\\b   \\r" % i
                        for i in range(1, 400))
              + "\\x1b[32;1m[f] found new best score! (500 vs 720)\\x1b[0m\\n"
              + "iteration 400, 1 errors, score = 500")
    log = os.path.join(awlib.REPO, "build", "permute-selftest.log")
    os.makedirs(os.path.dirname(log), exist_ok=True)
    rc, kept = agbenv.stream_compact("printf '%s'" % stream, log)
    lines = kept.splitlines()
    good = (rc == 0 and "[f] found new best score! (500 vs 720)" in lines
            and lines[-1].endswith("iteration 400, 1 errors, score = 500  (final)")
            and len(lines) <= 5)
    print("[self-test] progress compacted to %d line(s), messages kept: %s"
          % (len(lines), "PASS" if good else "FAIL"))
    ok &= good

    # 2. The splice keeps the synthesized inline_fn helpers above the function.
    orig = ['#include "global.h"\n', "\n", "int f(int a)\n", "{\n", "    return a;\n", "}\n"]
    cand = ["typedef int s32;\n", "int g;\n", "\n",
            "int inline_fn(int a)\n", "{\n", "    return a + 1;\n", "}\n", "\n",
            "int f(int a)\n", "{\n", "    return inline_fn(a);\n", "}\n"]
    out = splice(orig, cand, "f")
    good = (out is not None and out[0] == '#include "global.h"\n'
            and "int inline_fn(int a)\n" in out and "typedef int s32;\n" not in out)
    print("[self-test] splice keeps the draft's includes and the inline_fn helper: %s"
          % ("PASS" if good else "FAIL"))
    ok &= good

    # 3. The right size outranks the percentage when keeping an improvement.
    good = (not improves(90.0, 4, 80.0, 0)       # never trade size-exact away
            and improves(40.0, 0, 60.0, -4)      # size-exact wins at any score
            and improves(81.0, 4, 80.0, 8)       # same side of exact: by score
            and not improves(79.0, 0, 80.0, 0))
    print("[self-test] a size-exact form is never replaced by a wrong-size one: %s"
          % ("PASS" if good else "FAIL"))
    ok &= good

    # 4. The default thread count is a share of the machine, not all of it.
    good = DEFAULT_THREADS == 4
    print("[self-test] default threads = %d: %s" % (DEFAULT_THREADS, "PASS" if good else "FAIL"))
    ok &= good
    print("[self-test] %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


# Several agents share the machine and each runs up to three searches; the
# 2026-08-29 default of os.cpu_count() (32 here) put ~200 workers on 32 cores.
DEFAULT_THREADS = 4


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("name", nargs="?", help="function name or address")
    ap.add_argument("--seconds", type=int, default=300,
                    help="wall-clock limit for the search (default 300)")
    ap.add_argument("-j", "--threads", type=int, default=DEFAULT_THREADS,
                    help="permuter worker processes (default %d)" % DEFAULT_THREADS)
    ap.add_argument("--setup-only", action="store_true",
                    help="build the permuter directory and stop")
    ap.add_argument("--current", action="store_true",
                    help="start from <name>.c rather than best.c")
    ap.add_argument("--from-permuter-best", action="store_true",
                    help="start from best.c even when an earlier permuter run "
                         "wrote it (by default the draft is used then)")
    ap.add_argument("--force", action="store_true",
                    help="run even if the vendored scorer is unpatched")
    ap.add_argument("--keep", action="store_true",
                    help="keep output directories even on success")
    ap.add_argument("--live", action="store_true",
                    help="echo the permuter's raw progress (the pre-wave-90 output)")
    ap.add_argument("--max-verify", type=int, default=MAX_VERIFY,
                    help="check at most this many outputs, best permuter "
                         "score first (default %(default)s)")
    ap.add_argument("--profile", choices=agbenv.compiler_profiles(),
                    default="configured",
                    help="compile every candidate under this temporary profile "
                         "(as trymatch --profile); a match is provisional until "
                         "data/compiler-overrides.json names it")
    ap.add_argument("--self-test", action="store_true")
    args = ap.parse_args()
    if args.self_test:
        return self_test()
    if not args.name:
        ap.error("give a function name or address")
    rc, state, detail = _main(args)
    print("\nPERMUTE %s %s%s" % (state, args.name, (" " + detail) if detail else ""))
    return rc


def _main(args):
    global MAX_VERIFY, PROFILE
    MAX_VERIFY = max(1, args.max_verify)
    PROFILE = args.profile
    if not os.path.isdir(PERMUTER_DIR):
        print(CLONE_HINT)
        return 2, "SETUP-FAILED", "vendor/decomp-permuter missing"

    rec, unit = trymatch.resolve(args.name)
    if rec is None or unit is None:
        return 2, "SETUP-FAILED", "unknown function"

    if not check_scorer_patch() and not args.force:
        return 2, "SETUP-FAILED", "unpatched scorer (see above; --force to run anyway)"

    lock, held = _take_lock(rec["name"], args.seconds)
    if lock is None and not args.force:
        print("\nanother permute.py run on %s holds %s (%s)." % (rec["name"],
              os.path.relpath(_lock_path(rec["name"]), awlib.REPO), held))
        print("Two runs on one function corrupt its draft: the second restores")
        print("the first one's candidate. Wait for it, or pass --force if it died.")
        return 2, "SETUP-FAILED", "another run on this function is live"
    try:
        return _run_locked(args, rec, unit)
    finally:
        if lock is not None:
            try:
                os.remove(lock)
            except OSError:
                pass


def _lock_path(name):
    return os.path.join(WORK, name, ".permute.lock")


def _take_lock(name, seconds):
    """(path, None) when this run now owns work/<fn>/.permute.lock, else
    (None, description of the holder).

    Wave 94 had two runs on one function at once: a launch wrapper reported
    "failed" while its permute.py lived on, a second run started, and the
    second verify phase restored the FIRST run's header-expanded candidate
    (203 KB) over the draft while printing that it had restored it unchanged.
    A lock older than its run's time budget plus an hour is treated as stale.
    """
    import time
    path = _lock_path(name)
    for _ in range(2):
        try:
            fd = os.open(path, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
        except FileExistsError:
            try:
                with open(path, encoding="utf-8") as fh:
                    info = json.load(fh)
                started = float(info.get("started", 0))
                budget = float(info.get("seconds", 900)) + 3600
            except (OSError, ValueError):
                started, budget, info = 0.0, 0.0, {}
            if time.time() < started + budget:
                return None, "pid %s, started %s" % (
                    info.get("pid"), time.strftime("%H:%M:%S",
                                                   time.localtime(started)))
            os.remove(path)          # stale: its run cannot still be going
            continue
        with os.fdopen(fd, "w", encoding="utf-8") as fh:
            json.dump({"pid": os.getpid(), "started": time.time(),
                       "seconds": seconds}, fh)
        return path, None
    return None, "could not create the lock file"


def _run_locked(args, rec, unit):
    prefer = False if args.current else ("any" if args.from_permuter_best else True)
    pdir = setup(rec, unit, prefer_best=prefer)
    if pdir is None:
        return 2, "SETUP-FAILED", "see above"

    score = base_score(pdir)
    if score is None:
        return 2, "SETUP-FAILED", "the permuter could not score the starting point"
    print("  base score %d  (0 would mean the starting point already matches)"
          % score)
    if score == 0:
        print("\nthe starting point already scores 0 -- run trymatch to see")
        print("whether it matches at the byte level.")
        return 0, "BASE-SCORES-ZERO", "run trymatch"

    if args.setup_only:
        rel = os.path.relpath(pdir, awlib.REPO).replace(os.sep, "/")
        print("\nsetup only. To run it yourself:")
        print("    wsl -d %s -e bash -c 'cd %s && "
              "python3 vendor/decomp-permuter/permuter.py %s'"
              % (agbenv.DISTRO, shq(agbenv.REPO_POSIX), shq(rel)))
        return 0, "SETUP-ONLY", ""

    run(pdir, args.seconds, args.threads, live=args.live)
    return verify(rec, pdir, args.keep)


if __name__ == "__main__":
    sys.exit(main())
