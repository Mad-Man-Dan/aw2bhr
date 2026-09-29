#!/usr/bin/env python3
"""Compile and score drafts WITHOUT touching work/, and answer three questions
the wave protocol used to answer by hand.

trymatch.py is the verdict, and it is built around ONE file: it compiles
work/<fn>/<fn>.c into work/<fn>/, rewrites _cand.bin, and moves best.c. That
makes it the wrong instrument for anything that has to look at several sources,
or at a draft someone else is editing, or at a draft against a DIFFERENT set of
headers. Everything here compiles into build/drafts/ instead, deletes its own
outputs before each compile (a failed compile never leaves an older object that
reads as current), and never writes to work/.

    python tools/drafts.py score sub_08031824                 # the draft
    python tools/drafts.py score sub_08031824 work/sub_08031824/w89-start.c
    python tools/drafts.py score sub_080283E4 work/sub_080283E4/sub_080283E4.prepr3.c --headers-at 803d1bd

    python tools/drafts.py bases sub_0807E980                 # which file is the real base?

    python tools/drafts.py snapshot before-rename --unmatched # before a header change
    python tools/drafts.py compare before-rename              # after it

`score` prints one line: MATCH / MISMATCH <pct> size<+d> first+0x<off> /
COMPILE-FAIL <first error> / NO-DRAFT. It uses trymatch's own byte and
relocation rules, but it is NOT the verdict and records nothing: promote only
on trymatch's exit status.

`bases` answers "which file is the real starting point" for a function whose
best.c / best.json may have rotted. It compiles every candidate in
work/<fn>/ -- the draft, best.c, the w<N>-*.c snapshots, the newest permuter
outputs -- scores each against the ROM, checks best.json's recorded score and
source_sha1 against best.c, and names one base with its reason. It copies
nothing; the recommendation is a `cp` line for you to run.

`snapshot TAG` compiles every listed draft and stores its .text, .rodata and
relocations under build/snapshots/TAG/. `compare TAG` recompiles the same
drafts and reports, per function, SAME / CHANGED / NOW-COMPILE-FAIL /
STILL-COMPILE-FAIL / NOW-COMPILES. Take a snapshot before any header rename or
struct reshape; afterwards `compare` lists exactly the drafts the change
broke or silently altered. A draft that still compiles but whose bytes
CHANGED is the dangerous case: wave 90's KeySt reshape left old field names
compiling at new offsets. Relocations are compared by resolved address, so a
pure symbol rename (gUnknown_03003FC0 -> gPlaySt) is SAME.

`audit [fn...]` recompiles each function's best.c (default: every parked
function) and checks best.json against it: the hash, the size fields and the
score. `--write` rewrites the stale records from best.c's score; it is the
one command here that writes in work/, and only best.json.

    python tools/drafts.py audit              # report
    python tools/drafts.py audit --write      # and fix the stale records

`--headers-at REV` compiles against include/ as it was at git revision REV
(extracted once into build/headers/<sha>/, placed ahead of include/ on the
quote-include path). With it a snapshot can be taken AFTER the fact, from the
pre-change source: that is how tools/port_rename.py proves a port preserved
the old draft's bytes.

Exit status: 0 on success, 1 if any draft failed to compile or changed
(compare) / did not match (score), 2 on a usage or setup error.
"""

import argparse
import collections
import glob
import hashlib
import io
import json
import os
import re
import shlex
import subprocess
import sys
import tarfile

import agbenv
import awlib
import trymatch

REPO = awlib.REPO
WORK = os.path.join(REPO, "work")
OUT = "build/drafts"
SNAP = "build/snapshots"
HEADERS = "build/headers"

MARK = "@@AW2-RELOCS@@"
DEBUG_SECTIONS = (".debug", ".stab", ".comment")


def rel(path):
    return os.path.relpath(path, REPO).replace(os.sep, "/")


def absp(relpath):
    return os.path.join(REPO, relpath.replace("/", os.sep))


def atomic_write(path, data):
    """Write bytes via a temp file and os.replace, so no reader sees half."""
    os.makedirs(os.path.dirname(path), exist_ok=True)
    tmp = "%s.tmp%d" % (path, os.getpid())
    with open(tmp, "wb") as fh:
        fh.write(data)
    os.replace(tmp, path)


def sha1(data):
    return hashlib.sha1(data).hexdigest()


# ---------------------------------------------------------------- headers --

def headers_at(rev):
    """Repo-relative include dir holding include/ as of `rev`, or None.

    Extracted once per commit into build/headers/<sha12>/include and reused.
    """
    r = subprocess.run(["git", "rev-parse", "--verify", rev + "^{commit}"],
                       cwd=REPO, capture_output=True, text=True)
    if r.returncode != 0:
        print("error: %r is not a git revision" % rev)
        return None
    sha = r.stdout.strip()[:12]
    dest = "%s/%s" % (HEADERS, sha)
    inc = dest + "/include"
    if os.path.isfile(absp(dest + "/.complete")):
        return inc
    r = subprocess.run(["git", "archive", "--format=tar", sha, "include"],
                       cwd=REPO, capture_output=True)
    if r.returncode != 0:
        print("error: git archive %s include failed: %s"
              % (sha, r.stderr.decode(errors="replace")[-500:]))
        return None
    tmp = absp(dest) + ".tmp%d" % os.getpid()
    with tarfile.open(fileobj=io.BytesIO(r.stdout)) as tf:
        tf.extractall(tmp, filter="data")
    if os.path.isdir(absp(dest)):
        # Another process won the race; theirs is identical.
        import shutil
        shutil.rmtree(tmp, ignore_errors=True)
    else:
        os.makedirs(os.path.dirname(absp(dest)), exist_ok=True)
        os.replace(tmp, absp(dest))
    atomic_write(absp(dest + "/.complete"), sha.encode())
    return inc


def cppflags(fn, include=None, profile="configured"):
    f = agbenv.flags(fn, profile=profile)
    flags = f["CPPFLAGS"]
    if include:
        # Quote includes search the including file's own directory first, then
        # the -iquote list in order. Putting the old tree ahead of include/
        # makes every header -- and every header those headers include, since
        # they resolve beside themselves -- come from the old revision.
        if "-iquote include" not in flags:
            raise RuntimeError("CPPFLAGS has no `-iquote include` to override")
        flags = flags.replace("-iquote include",
                              "-iquote %s -iquote include" % shlex.quote(include), 1)
    return f, flags


def preprocess(fn, src_rel, include=None, keep_lines=True):
    """(rc, text, stderr) of the C preprocessor on src, as the build runs it.

    `__attribute__` is defined away (pycparser cannot parse it), exactly as
    tools/permute.py does for base.c.
    """
    f, flags = cppflags(fn, include)
    return agbenv.run('%s %s %s -D"__attribute__(x)=" %s'
                      % (f["CPP"], flags, "" if keep_lines else "-P",
                         shlex.quote(src_rel)))


# ------------------------------------------------------------------ build --

class Build:
    """One compile: ok, err, .text, .rodata and relocations [(sect, off, type, sym)]."""
    __slots__ = ("ok", "err", "text", "rodata", "relocs", "obj", "src")

    def __init__(self):
        self.ok, self.err = False, ""
        self.text, self.rodata, self.relocs = b"", b"", []
        self.obj = self.src = None


def build(fn, src_rel, label, include=None, profile="configured"):
    """Compile src for fn into build/drafts/<fn>/<label>.o. Never writes work/.

    The pipeline is agbenv.compile_c's (and so the Makefile's) plus one guard:
    `rm -f` of every output first, so a failed compile can never leave an
    older object behind that reads as this one. That trap is how wave 90's hand
    check passed a draft that did not compile: trymatch left the previous
    _cand.bin in place. There is deliberately NO pipefail: `make` pipes cpp
    into agbcc without it, so a preprocessor-only error (`#error`) does not
    fail the real build either, and this must agree with trymatch and make.
    """
    b = Build()
    b.src = src_rel
    safe = re.sub(r'[^A-Za-z0-9_.-]', '_', label)
    base = "%s/%s/%s" % (OUT, fn, safe)
    o, s, tb, rb = base + ".o", base + ".s", base + ".text.bin", base + ".rodata.bin"
    b.obj = o
    f, flags = cppflags(fn, include, profile)
    prefix = agbenv.makefile_var("PREFIX") or "arm-none-eabi-"
    q = shlex.quote
    script = (
        'set -e\n'
        'rm -f {o} {s} {tb} {rb}\n'
        'mkdir -p "$(dirname {o})"\n'
        '{cpp} {cppflags} {src} | iconv -f UTF-8 -t CP932 | {cc1} {cflags} -o {s}\n'
        'printf ".text\\n\\t.align\\t2, 0\\n" >> {s}\n'
        '{as_} {asflags} {s} -o {o}\n'
        '{strip} -N .gcc2_compiled. {o}\n'
        '{p}objcopy -O binary --only-section=.text {o} {tb}\n'
        '{p}objcopy -O binary --only-section=.rodata {o} {rb} 2>/dev/null || true\n'
        'echo {mark}\n'
        '{p}objdump -r {o}\n'
    ).format(cpp=f["CPP"], cppflags=flags, src=q(src_rel), cc1=f["CC1"],
             cflags=f["CFLAGS"], s=q(s), as_=f["AS"], asflags=f["ASFLAGS"],
             o=q(o), strip=f["STRIP"], p=prefix, tb=q(tb), rb=q(rb), mark=MARK)
    rc, so, se = agbenv.run(script)
    if rc != 0 or MARK not in so:
        b.err = (se or so).strip()
        return b
    b.text = _read(absp(tb))
    if not b.text:
        # Without pipefail a failed cpp still "succeeds": agbcc reads nothing,
        # `as` assembles an empty file. No draft compiles to zero bytes.
        b.err = se.strip() or "the draft compiled to an empty .text " \
                              "(the preprocessor probably failed)"
        return b
    b.ok = True
    b.rodata = _read(absp(rb))
    b.relocs = _parse_relocs(so.split(MARK, 1)[1])
    return b


def _read(path):
    try:
        with open(path, "rb") as fh:
            return fh.read()
    except OSError:
        return b""


def _parse_relocs(text):
    out, sect = [], None
    for ln in text.splitlines():
        s = ln.strip()
        m = re.match(r'RELOCATION RECORDS FOR \[(.+)\]:', s)
        if m:
            # Debug sections carry the source PATH and line table, so they
            # differ between any two files even when the code is identical.
            sect = None if m.group(1).startswith(DEBUG_SECTIONS) else m.group(1)
            continue
        parts = s.split()
        if sect is None or len(parts) < 3 or parts[0] == "OFFSET":
            continue
        try:
            out.append((sect, int(parts[0], 16), parts[1], parts[2]))
        except ValueError:
            pass
    return out


def first_error(err):
    """The first line that names an error, for a one-line report."""
    for ln in err.splitlines():
        if re.search(r'error|undeclared|no member|incomplete|conflicting|'
                     r'parse error|warning', ln):
            return ln.strip()
    lines = [ln.strip() for ln in err.splitlines() if ln.strip()]
    return lines[0] if lines else "(no message)"


# ----------------------------------------------------------------- target --

_TARGETS = {}


def target(fn):
    """(rec, tgt_fn bytes, target .text relocations) for fn, or None."""
    if fn in _TARGETS:
        return _TARGETS[fn]
    rec, unit = trymatch.resolve(fn)
    if rec is None or unit is None:
        return None
    unit_dir = unit.get("dir", "build/functions")
    unit_s = "%s/%s" % (unit_dir, unit["file"])
    unit_o = "%s/%s.o" % (unit_dir, unit["unit"])
    if not os.path.exists(absp(unit_o)):
        rc, _, se = agbenv.assemble(unit_s, unit_o)
        if rc != 0:
            print("error: could not assemble %s\n%s" % (unit_s, se[-800:]))
            return None
    os.makedirs(absp("%s/%s" % (OUT, rec["name"])), exist_ok=True)
    tgt, err = trymatch.section_bytes(unit_o, "%s/%s/_target.bin" % (OUT, rec["name"]))
    if tgt is None:
        print("error: could not read target .text: %s" % err)
        return None
    off = rec["addr"] - int(unit["addr_hex"], 16)
    t_rel = trymatch.relocations(unit_o, off, off + rec["size"])
    _TARGETS[fn] = (rec, tgt[off:off + rec["size"]], t_rel)
    return _TARGETS[fn]


def score(fn, b):
    """dict(state, pct, size_delta, first, n_diff) for a Build against the ROM.

    Same arithmetic as trymatch.check() (awlib.byte_score: identical bytes
    as a share of the longer of candidate and target); relocations must agree or be
    equivalent under trymatch's own rules.
    """
    if not b.ok:
        return {"state": "COMPILE-FAIL", "error": first_error(b.err)}
    t = target(fn)
    if t is None:
        return {"state": "ERROR", "error": "no target for %s" % fn}
    rec, tgt_fn, t_rel = t
    size = rec["size"]
    cand = b.text
    cand_fn = cand[:size]
    c_rel = [(o, ty, sy) for (sect, o, ty, sy) in b.relocs
             if sect == ".text" and o < size]
    n_diff, common, pct = awlib.byte_score(tgt_fn, cand, size)
    first = next((i for i, (x, y) in enumerate(zip(tgt_fn, cand_fn)) if x != y),
                 common)
    same = tgt_fn == cand_fn and len(cand) == size
    if same and t_rel is not None and t_rel != c_rel:
        same = trymatch.reloc_equivalent(tgt_fn, cand_fn, t_rel, c_rel, b.obj)
    elif not same and len(cand) == size:
        same = trymatch.reloc_equivalent(tgt_fn, cand_fn, t_rel, c_rel, b.obj)
    if same:
        return {"state": "MATCH", "pct": 100.0, "size_delta": 0, "first": None,
                "n_diff": 0}
    return {"state": "MISMATCH", "pct": round(pct, 2),
            "size_delta": len(cand) - size, "first": first, "n_diff": n_diff}


def fmt(r):
    st = r["state"]
    if st == "MISMATCH":
        return "MISMATCH %6.2f%% size%+d first+0x%x" % (
            r["pct"], r["size_delta"], r["first"])
    if st in ("COMPILE-FAIL", "ERROR"):
        return "%s  %s" % (st, r.get("error", ""))
    return st


# ------------------------------------------------------------ comparison --

def relocs_equivalent(a, b):
    """(True, "") if two relocation lists link to the same addresses.

    Same section, offset and type per record; the symbol may differ only if
    both resolve to one address (a rename: gUnknown_03003FC0 vs gPlaySt).
    """
    a = [r for r in a if not str(r[0]).startswith(DEBUG_SECTIONS)]
    b = [r for r in b if not str(r[0]).startswith(DEBUG_SECTIONS)]
    if len(a) != len(b):
        return False, "%d vs %d relocations" % (len(a), len(b))
    syms = None
    for (sa, oa, ta, na), (sb, ob, tb, nb) in zip(sorted(a), sorted(b)):
        if (sa, oa, ta) != (sb, ob, tb):
            return False, "%s+0x%x %s vs %s+0x%x %s" % (sa, oa, ta, sb, ob, tb)
        if na == nb:
            continue
        if syms is None:
            syms = trymatch.symbol_addresses()
        an, ax = trymatch._split_sym(na)
        bn, bx = trymatch._split_sym(nb)
        aa, ba = trymatch.sym_addr(an, syms), trymatch.sym_addr(bn, syms)
        if aa is None or ba is None or aa + ax != ba + bx:
            return False, "%s+0x%x: %s vs %s" % (sa, oa, na, nb)
    return True, ""


def compare_builds(old, new):
    """(verdict, detail) for two Builds of what should be the same code."""
    if not old.ok and not new.ok:
        return "STILL-COMPILE-FAIL", first_error(new.err)
    if not old.ok:
        return "NOW-COMPILES", "(no bytes before to compare against)"
    if not new.ok:
        return "NOW-COMPILE-FAIL", first_error(new.err)
    if old.text != new.text:
        n = sum(1 for x, y in zip(old.text, new.text) if x != y)
        first = next((i for i, (x, y) in enumerate(zip(old.text, new.text))
                      if x != y), min(len(old.text), len(new.text)))
        return "CHANGED", ".text %d -> %d bytes, %d differ, first +0x%x" % (
            len(old.text), len(new.text), n, first)
    if old.rodata != new.rodata:
        return "CHANGED", ".rodata differs (%d -> %d bytes)" % (
            len(old.rodata), len(new.rodata))
    ok, why = relocs_equivalent(old.relocs, new.relocs)
    if not ok:
        return "CHANGED", "relocations: " + why
    return "SAME", ""


# ---------------------------------------------------------------- helpers --

def draft_rel(fn):
    return "work/%s/%s.c" % (fn, fn)


def names_from(args):
    names = list(args.names or [])
    if getattr(args, "list", None):
        with open(args.list, encoding="utf-8-sig", errors="replace") as fh:
            names += fh.read().replace("\r", "\n").split()
    if getattr(args, "unmatched", False):
        recs = json.load(open(os.path.join(awlib.DATA_DIR, "functions.json"),
                              encoding="utf-8"))
        names += [r["name"] for r in recs if r["status"] != "matched"
                  and os.path.exists(absp(draft_rel(r["name"])))]
    return list(dict.fromkeys(names))


def is_blob(text):
    return len(text) > 40000 and text.count(b"#") < 5


def is_mutated(text):
    code = re.sub(rb"/\*.*?\*/", b"", text, flags=re.S)
    return re.search(rb"\bnew_var\d*\b", code) is not None


# ---------------------------------------------------------------- commands --

def cmd_score(args):
    fn = args.fn
    src = args.file or draft_rel(fn)
    src = rel(os.path.abspath(src)) if os.path.isabs(src) else src.replace("\\", "/")
    if not os.path.exists(absp(src)):
        print("%-16s NO-DRAFT  %s" % (fn, src))
        return 1
    inc = headers_at(args.headers_at) if args.headers_at else None
    if args.headers_at and inc is None:
        return 2
    b = build(fn, src, "score-" + os.path.basename(src), include=inc)
    r = score(fn, b)
    print("%-16s %s  (%s%s)" % (fn, fmt(r), src,
                                ", headers at %s" % args.headers_at if inc else ""))
    if r["state"] == "COMPILE-FAIL" and args.verbose:
        print("\n".join("  " + ln for ln in b.err.splitlines()[-25:]))
    return 0 if r["state"] == "MATCH" else 1


def _wrongc_note(fn, cand, draft):
    """What tools/wrongc.py's pattern rules (no compile) say `cand` adds or
    changes relative to the draft that would make it wrong C, else ''."""
    try:
        import wrongc
        return wrongc.rejects(fn, cand, draft, emu=False,
                              only=wrongc.STRICT_RULES) or ""
    except Exception:
        return ""


def cmd_bases(args):
    fn = args.fn
    wd = os.path.join(WORK, fn)
    if not os.path.isdir(wd):
        print("error: no work/%s" % fn)
        return 2
    cands = []
    for p in sorted(glob.glob(os.path.join(wd, "*.c"))):
        name = os.path.basename(p)
        if name.endswith(".perm.c"):
            continue            # PERM_ macros: not C, the permuter's input only
        cands.append(p)
    outs = []
    for d in glob.glob(os.path.join(wd, "permuter", "output-*")):
        m = re.match(r'output-(\d+)-', os.path.basename(d))
        src = os.path.join(d, "source.c")
        if m and os.path.exists(src):
            outs.append((int(m.group(1)), src))
    cands += [p for _, p in sorted(outs)[:args.permuter_outputs]]

    meta = {}
    try:
        meta = json.load(open(os.path.join(wd, "best.json"), encoding="utf-8"))
    except (OSError, ValueError):
        pass

    # Locals the draft itself might read before setting. A candidate that adds
    # a name to this list is reading something no assignment reaches, which
    # is wrong C whatever it scores (see agbenv.uninitialized_reads).
    draft_path = os.path.join(wd, fn + ".c")
    draft_uninit = set(agbenv.uninitialized_reads(draft_path, fn=fn)
                       if os.path.exists(draft_path) else ())

    rows = []
    for p in cands:
        data = _read(p)
        label = rel(p).split("work/%s/" % fn, 1)[-1]
        notes = []
        if is_blob(data):
            notes.append("header-expanded")
        if is_mutated(data):
            notes.append("new_var")
        if ".prepr" in label:
            notes.append("pre-rename source")
        if not is_blob(data):
            extra = sorted(set(agbenv.uninitialized_reads(p, fn=fn)) - draft_uninit)
            if extra:
                notes.append("reads before set: " + ", ".join(extra))
        if os.path.abspath(p) != os.path.abspath(draft_path):
            if re.search(r"wrong", label, re.I):
                notes.append("wrong C: file is labelled wrong")
            elif not is_blob(data) and os.path.exists(draft_path):
                why = _wrongc_note(fn, p, draft_path)
                if why:
                    notes.append("wrong C: " + why)
        b = build(fn, rel(p), "base-" + label.replace("/", "_"))
        r = score(fn, b)
        rows.append((label, r, notes, data))
        print("  %-34s %s%s" % (label, fmt(r),
                                ("   [" + ", ".join(notes) + "]") if notes else ""),
              flush=True)

    print()
    # best.json's claims, checked against the file they describe.
    bc = next((row for row in rows if row[0] == "best.c"), None)
    if meta:
        claim = meta.get("percent")
        want = meta.get("source_sha1")
        if bc is None:
            print("best.json claims %s%% but there is no best.c" % claim)
        else:
            got = sha1(bc[3])
            if want and want != got:
                print("best.json: source_sha1 does NOT match best.c -- its %s%% "
                      "describes some other source" % claim)
            elif not want:
                print("best.json: legacy entry (no source_sha1); its %s%% is "
                      "unverifiable" % claim)
            else:
                print("best.json: source_sha1 matches best.c (%s%%)" % claim)
            now = bc[1].get("pct")
            if now is not None and claim is not None and abs(now - claim) > 0.05:
                print("best.json claims %.2f%%; best.c scores %.2f%% today "
                      "(headers or overrides moved since)" % (claim, now))
            if "size_delta" not in meta:
                print("best.json has no size_delta: best.c was never checked "
                      "for size, so it may be a percentage winner of the wrong size")

    # `new_var` is only a note: it is the permuter's name for a local it
    # added, and that local is sometimes the real fix (wave 89's sub_080073F8
    # rise came from one). Header-expanded text is unusable as a base, and so
    # is a form that reads a local no assignment reaches.
    usable = [row for row in rows
              if row[1]["state"] in ("MATCH", "MISMATCH")
              and "header-expanded" not in row[2]
              and not any(n.startswith(("reads before set", "wrong C")) for n in row[2])]
    if not usable:
        print("NO USABLE BASE: nothing here compiles as a readable draft. "
              "Rebuild from the assembly or port a pre-rename source "
              "(tools/port_rename.py).")
        return 1
    draft_label = fn + ".c"

    def rank(row):
        r = row[1]
        return (r["state"] != "MATCH", abs(r.get("size_delta") or 0) != 0,
                abs(r.get("size_delta") or 0), -(r.get("pct") or 0),
                row[0] != draft_label)
    usable.sort(key=rank)
    best = usable[0]
    r = best[1]
    why = ("matches" if r["state"] == "MATCH" else
           "%s, %.2f%%" % ("size-exact" if r["size_delta"] == 0
                           else "size%+d" % r["size_delta"], r["pct"]))
    print("BASE: work/%s/%s  (%s)" % (fn, best[0], why))
    if best[0] != draft_label:
        dr = next((row for row in rows if row[0] == draft_label), None)
        if dr is not None:
            print("      the draft is %s" % fmt(dr[1]))
            # A higher score with an EARLIER first difference usually means
            # bytes were gained after a point the candidate breaks. In wave 93
            # this flagged four of five rejected bases (dead-store padding, a
            # spelling measured and rejected in an earlier wave), though once
            # (sub_0808A3DC) the earlier-differing file was the better base.
            d1, b1 = dr[1].get("first"), r.get("first")
            if (dr[1]["state"] == "MISMATCH" and r["state"] == "MISMATCH"
                    and d1 is not None and b1 is not None and b1 < d1):
                print("      CAUTION: its first difference (+0x%x) is EARLIER than "
                      "the draft's (+0x%x). Read what it changed, and grep "
                      "work/%s/NOTES.md and include/ for that spelling, before "
                      "adopting it." % (b1, d1, fn))
        print("      to start from it:  cp work/%s/%s work/%s/%s.c   "
              "(back the draft up first)" % (fn, best[0], fn, fn))
        print("      It is a measurement, not a verdict: read every statement "
              "it changes first.")
    return 0


def cmd_audit(args):
    """Recompile each function's best.c and check best.json against it."""
    names = names_from(args)
    if not names:
        recs = json.load(open(os.path.join(awlib.DATA_DIR, "functions.json"),
                              encoding="utf-8"))
        names = [r["name"] for r in recs if r["status"] == "parked"]
    counts = collections.Counter()
    for fn in names:
        wd = os.path.join(WORK, fn)
        best_c, meta_p = os.path.join(wd, "best.c"), os.path.join(wd, "best.json")
        if not os.path.exists(best_c):
            counts["no best.c"] += 1
            continue
        try:
            meta = json.load(open(meta_p, encoding="utf-8"))
        except (OSError, ValueError):
            meta = {}
        data = _read(best_c)
        b = build(fn, rel(best_c), "audit-best")
        r = score(fn, b)
        if r["state"] not in ("MATCH", "MISMATCH"):
            counts["best.c does not compile"] += 1
            print("%-16s BEST.C FAILS  %s" % (fn, r.get("error", "")))
            continue
        now = 100.0 if r["state"] == "MATCH" else r["pct"]
        had = trymatch.recorded_percent(meta) if meta else None
        problems = []
        if not meta:
            problems.append("no best.json")
        else:
            if meta.get("source_sha1") != sha1(data):
                problems.append("hash missing" if not meta.get("source_sha1")
                                else "hash is for another source")
            if "size_delta" not in meta:
                problems.append("no size")
            if had is not None and abs(had - now) > 0.05:
                problems.append("recorded %.2f%%" % had)
        verdict = "OK" if not problems else "FIXED" if args.write else "STALE"
        counts[verdict] += 1
        print("%-16s %-5s %s%s" % (fn, verdict, fmt(r),
                                   ("   (" + "; ".join(problems) + ")") if problems else ""))
        if problems and args.write:
            size = target(fn)[0]["size"]
            payload = {"percent": round(now, 2),
                       "scored_over": trymatch.SCORED_OVER,
                       "source_sha1": sha1(data),
                       "candidate_bytes": len(b.text), "target_bytes": size,
                       "size_delta": len(b.text) - size,
                       "exact_size": len(b.text) == size,
                       "audited": True}
            if meta.get("origin"):
                payload["origin"] = meta["origin"]
            atomic_write(meta_p, (json.dumps(payload, sort_keys=True) + "\n")
                         .encode("utf-8"))
    print("\n" + ", ".join("%s %d" % kv for kv in sorted(counts.items())))
    if counts["STALE"]:
        print("re-run with --write to rewrite the STALE records from best.c")
    return 0


def _snap_dir(tag):
    if not re.match(r'^[A-Za-z0-9_.-]+$', tag):
        raise SystemExit("error: snapshot tag must be [A-Za-z0-9_.-]+")
    return "%s/%s" % (SNAP, tag)


def cmd_snapshot(args):
    names = names_from(args)
    if not names:
        print("error: no functions given (names, --list or --unmatched)")
        return 2
    inc = headers_at(args.headers_at) if args.headers_at else None
    if args.headers_at and inc is None:
        return 2
    d = _snap_dir(args.tag)
    manifest = {"headers_at": args.headers_at, "functions": {}}
    bad = 0
    for fn in names:
        src = args.source.replace("{fn}", fn) if args.source else draft_rel(fn)
        if not os.path.exists(absp(src)):
            manifest["functions"][fn] = {"state": "NO-DRAFT", "src": src}
            print("  %-16s NO-DRAFT %s" % (fn, src))
            continue
        b = build(fn, src, "snap-" + args.tag, include=inc)
        r = score(fn, b)
        ent = {"src": src, "src_sha1": sha1(_read(absp(src))), "ok": b.ok,
               "state": r["state"], "pct": r.get("pct"),
               "size_delta": r.get("size_delta"), "first": r.get("first")}
        if b.ok:
            atomic_write(absp("%s/%s.text" % (d, fn)), b.text)
            atomic_write(absp("%s/%s.rodata" % (d, fn)), b.rodata)
            ent["relocs"] = b.relocs
        else:
            ent["error"] = first_error(b.err)
            bad += 1
        manifest["functions"][fn] = ent
        print("  %-16s %s" % (fn, fmt(r)), flush=True)
    atomic_write(absp(d + "/manifest.json"),
                 (json.dumps(manifest, indent=1, sort_keys=True) + "\n").encode())
    print("snapshot %s: %d function(s), %d did not compile -> %s/"
          % (args.tag, len(names), bad, d))
    return 1 if bad else 0


def cmd_compare(args):
    d = _snap_dir(args.tag)
    try:
        manifest = json.load(open(absp(d + "/manifest.json"), encoding="utf-8"))
    except (OSError, ValueError):
        print("error: no snapshot %r (run `drafts.py snapshot %s ...` first)"
              % (args.tag, args.tag))
        return 2
    names = names_from(args) or sorted(manifest["functions"])
    counts = {}
    for fn in names:
        ent = manifest["functions"].get(fn)
        if ent is None:
            print("  %-16s NOT-IN-SNAPSHOT" % fn)
            counts["NOT-IN-SNAPSHOT"] = counts.get("NOT-IN-SNAPSHOT", 0) + 1
            continue
        src = args.source.replace("{fn}", fn) if args.source else draft_rel(fn)
        old = Build()
        old.ok = ent.get("ok", False)
        old.err = ent.get("error", "")
        if old.ok:
            old.text = _read(absp("%s/%s.text" % (d, fn)))
            old.rodata = _read(absp("%s/%s.rodata" % (d, fn)))
            old.relocs = [tuple(x) for x in ent.get("relocs", [])]
        if not os.path.exists(absp(src)):
            verdict, detail = "NO-DRAFT", src
        else:
            new = build(fn, src, "compare-" + args.tag)
            verdict, detail = compare_builds(old, new)
            if ent.get("src") and ent["src"] != src:
                detail = (detail + "; " if detail else "") + "vs a snapshot of " + ent["src"]
            elif ent.get("src_sha1") and sha1(_read(absp(src))) != ent["src_sha1"]:
                detail = (detail + "; " if detail else "") + "source edited since the snapshot"
            if verdict == "CHANGED" and new.ok:
                r = score(fn, new)
                detail += "; now %s (was %s)" % (
                    fmt(r), "%.2f%%" % ent["pct"] if ent.get("pct") is not None
                    else ent.get("state"))
        counts[verdict] = counts.get(verdict, 0) + 1
        print("  %-16s %-18s %s" % (fn, verdict, detail), flush=True)
    print("compare %s: %s" % (args.tag, ", ".join(
        "%d %s" % (n, k) for k, n in sorted(counts.items()))))
    bad = sum(n for k, n in counts.items() if k != "SAME")
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = ap.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("score", help="compile one file and score it; writes nothing in work/")
    p.add_argument("fn")
    p.add_argument("file", nargs="?", help="default work/<fn>/<fn>.c")
    p.add_argument("--headers-at", metavar="REV")
    p.add_argument("-v", "--verbose", action="store_true", help="print compile errors")
    p.set_defaults(func=cmd_score)

    p = sub.add_parser("bases", help="rank every candidate base file for one function")
    p.add_argument("fn")
    p.add_argument("--permuter-outputs", type=int, default=3,
                   help="newest-best permuter outputs to include (default 3)")
    p.set_defaults(func=cmd_bases)

    p = sub.add_parser("audit", help="recompile best.c and check best.json "
                                     "against it (default: every parked function)")
    p.add_argument("names", nargs="*")
    p.add_argument("--list", help="file of function names")
    p.add_argument("--unmatched", action="store_true",
                   help="every unmatched function that has a draft")
    p.add_argument("--write", action="store_true",
                   help="rewrite each stale best.json from best.c's score")
    p.set_defaults(func=cmd_audit)

    for name, func, hlp in (("snapshot", cmd_snapshot, "store drafts' bytes under a tag"),
                            ("compare", cmd_compare, "recompile and diff against a tag")):
        p = sub.add_parser(name, help=hlp)
        p.add_argument("tag")
        p.add_argument("names", nargs="*")
        p.add_argument("--list", help="file of function names")
        p.add_argument("--unmatched", action="store_true",
                       help="every unmatched function that has a draft")
        p.add_argument("--source", metavar="PATH",
                       help="source path with {fn}, default work/{fn}/{fn}.c")
        if name == "snapshot":
            p.add_argument("--headers-at", metavar="REV",
                           help="compile against include/ as of this git revision")
        p.set_defaults(func=func)

    args = ap.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
