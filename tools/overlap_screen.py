#!/usr/bin/env python3
"""Rank unmatched functions by their proximity to an ALREADY-MATCHED one.

Every band screen this project has run filtered on `size >= 256 &&
backward_branches == 0`. Wave 20 (W20-C) found that floor was wrong: the 0x08052
sprite block holds five matched exemplars interleaved with seven unmatched
siblings at 144-256 bytes, and no band screen ever showed it. Size was never the
right floor -- EXEMPLAR PROXIMITY is, and it is cheap to measure.

Two measurements, both off `calls` in data/functions.json, plus one off the
assembly itself:

  overlap    |shared callees| and the Jaccard ratio between an unmatched
             function and every matched one. `calls` is invariant to inserted
             code, which is why wave 19 could batch on it where a normalised
             shape string scored the same pair 0.366.

  duplicate  the normalised instruction stream, label names erased. Two
             functions with the same stream have the SAME C body -- match one
             and the other is transcription. This is the highest-value thing
             the screen finds and it costs one pass over asm/.

  cluster    unmatched functions that share a matched exemplar, plus every
             unmatched function that is itself close to one of them. That is
             the unit a wave can be batched on.

  delta      WAVE 59. The residual's KIND, measured off the DRAFT rather than
             off the address map. Every axis above is address- or callee-based
             and they are all spent -- the clean pool ended in wave 57, so what
             is left is 300 functions that each already carry a draft, and what
             separates them is what their draft's residual IS. See delta_rows().

    python tools/overlap_screen.py                 # current small/loop-inclusive report
    python tools/overlap_screen.py --min-size 96 --no-loops  # historical view
    python tools/overlap_screen.py --delta-only    # just the wave-59 screen
    python tools/overlap_screen.py --self-test     # the wave-20 acceptance test

`--self-test` re-derives the 0x08052 cluster with no knowledge of it: it
requires sub_08052650/sub_08052AF4 to come back as a byte-identical pair and
sub_08052718/sub_08052BBC to come back grouped with the matched sub_08051BEC
and sub_08051F4C. A screen that cannot rediscover a cluster we know is real is
not finding the ones we do not.
"""

import argparse
import bisect
import collections
import hashlib
import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import awlib

ASM_DIR = os.path.join(awlib.REPO, "asm")
LOCAL_LABEL = re.compile(r"\b_0[0-9A-F]{7}\b")
POOL_LINE = re.compile(r"^_0[0-9A-F]{7}: \.4byte (\S+)")
START = re.compile(r"^\s*(?:thumb|arm)_func_start (\S+)")


def load_index():
    with open(os.path.join(awlib.DATA_DIR, "functions.json"),
              encoding="utf-8") as fh:
        return json.load(fh)


def apply_pending(recs):
    """Promote in-memory anything trymatch has already matched but that has not
    been promoted yet, and return the names.

    THIS SCREEN GOES STALE EVERY WAVE and that is the whole reason the flag
    exists. Its output is a function of the MATCHED SET, so a wave that adds 30
    exemplars invalidates the previous wave's list -- most sharply the duplicate
    section, whose headline ("no unmatched function is a byte-identical twin of
    a matched one") can flip the moment a new exemplar lands. `data/functions.json`
    only learns about a match at promotion time, which is after the wave ends,
    so a screen run inside a wave sees the state the wave STARTED in.
    `work/<fn>/_cand.bin` and `_target.bin` are what trymatch wrote on its last
    run; byte-equal means that draft matched. Regenerating the list is a
    PRE-FLIGHT step for the next wave, not a wave-end one.
    """
    units_path = os.path.join(awlib.REPO, "build", "functions", "units.json")
    if not os.path.exists(units_path):
        return []
    with open(units_path, encoding="utf-8") as fh:
        units = json.load(fh)
    base = {}
    for u in units:
        for fn in u["functions"]:
            base[fn] = int(u["addr_hex"], 16)

    pending = []
    for r in recs:
        if r["status"] != "asm" or r["name"] not in base:
            continue
        d = os.path.join(awlib.REPO, "work", r["name"])
        a = os.path.join(d, "_cand.bin")
        b = os.path.join(d, "_target.bin")
        if not (os.path.exists(a) and os.path.exists(b)):
            continue
        cand = open(a, "rb").read()
        tgt = open(b, "rb").read()
        off = r["addr"] - base[r["name"]]
        # `_target.bin` is the whole SPLIT UNIT's .text, `_cand.bin` only the
        # candidate's -- comparing the files whole is wrong for any unit that
        # holds more than one function, which is most of them.
        if off < 0 or len(cand) != r["size"]:
            continue
        if tgt[off:off + r["size"]] == cand:
            r["status"] = "matched"
            pending.append(r["name"])
    return pending


def drop_hub_callees(recs, max_fanin):
    """Delete ubiquitous callees from every `calls` set, in place.

    Measured in wave 20: the top cluster the raw metric returns is 17 functions
    and 7,476 bytes across three subsystems, and the callees ALL of them share
    are `Div` and `SetObjAffine` -- called by 137 and 67 functions each. A
    library helper carries no family information, so a Jaccard built on it
    reports a family that is not there. Dropping callees above the threshold is
    a plain IDF cut and it is what makes the clusters takeable.
    """
    if not max_fanin:
        return set()
    fan = collections.Counter()
    for r in recs:
        for c in set(r["calls"]):
            fan[c] += 1
    hubs = set(c for c, n in fan.items() if n > max_fanin)
    for r in recs:
        r["calls"] = [c for c in r["calls"] if c not in hubs]
    return hubs


def asm_bodies():
    """{name: (instruction tuple, pool-word tuple)} for every function in asm/."""
    out = {}
    for fname in sorted(os.listdir(ASM_DIR)):
        if not fname.endswith(".s"):
            continue
        path = os.path.join(ASM_DIR, fname)
        cur = None
        insns = []
        pool = []
        with open(path, encoding="utf-8", errors="replace") as fh:
            for line in fh:
                m = START.match(line)
                if m:
                    if cur:
                        out[cur] = (tuple(insns), tuple(pool))
                    cur, insns, pool = m.group(1), [], []
                    continue
                if cur is None:
                    continue
                t = line.split("@")[0].strip()
                if not t:
                    continue
                mp = POOL_LINE.match(t)
                if mp:
                    pool.append(mp.group(1))
                    continue
                if t.endswith(":") or t.startswith("."):
                    continue
                insns.append(LOCAL_LABEL.sub("L", t))
        if cur:
            out[cur] = (tuple(insns), tuple(pool))
    return out


def digest(parts):
    return hashlib.sha1("\n".join(parts).encode()).hexdigest()[:12]


def duplicates(recs, bodies):
    """Groups of functions whose instruction streams are identical.

    Two digests per function.  `strict` keeps the pool words' symbol NAMES;
    `loose` replaces each with its slot index, so two siblings whose only
    difference is which `gUnknown_` a private -fforce-addr word names still
    group.  Report both -- strict is a certainty, loose is a lead.
    """
    strict = collections.defaultdict(list)
    loose = collections.defaultdict(list)
    callee = collections.defaultdict(list)
    for r in recs:
        body = bodies.get(r["name"])
        if not body or len(body[0]) < 4:
            continue
        insns, pool = body
        strict[digest(list(insns) + ["POOL"] + list(pool))].append(r)
        loose[digest(list(insns) + ["POOL"] + ["#%d" % i
                                               for i in range(len(pool))])
              ].append(r)
        callee[digest(normalise_callees(insns) + ["POOL"]
                      + ["#%d" % i for i in range(len(pool))])].append(r)
    return ({k: v for k, v in strict.items() if len(v) > 1},
            {k: v for k, v in loose.items() if len(v) > 1},
            {k: v for k, v in callee.items() if len(v) > 1})


BL_TARGET = re.compile(r"^bl (sub_[0-9A-Fa-f]{8})$")


def normalise_callees(insns):
    """Instruction stream with `bl sub_XXXXXXXX` targets replaced by an index.

    WAVE 38. The strict and loose tiers both key on the literal callee name, so
    two functions that are ONE SHAPE calling a different sibling do not group --
    and that turns out to be a common way for this ROM to repeat itself.
    sub_08028A68 and sub_08028AEC are 132 bytes each, 56 instructions each, and
    differ in exactly one thing: four `bl` sites naming sub_08028894 against
    sub_08028874. Neither the strict nor the loose tier saw them; the wave-38
    orchestrator found the pair by hand while batching, which is not a screen.

    Indexing by ORDER OF FIRST APPEARANCE rather than erasing the name keeps the
    tier honest: two functions calling three different callees still only group
    if they call them in the same PATTERN. One derivation gives every member --
    the callee is a parameter of the transcription, not new work.
    """
    seen = {}
    out = []
    for t in insns:
        m = BL_TARGET.match(t)
        if not m:
            out.append(t)
            continue
        out.append("bl CALLEE#%d" % seen.setdefault(m.group(1), len(seen)))
    return out


def overlaps(targets, matched, min_shared, min_j):
    """[(target, exemplar, |shared|, J)] over the threshold, best first."""
    pairs = []
    for t in targets:
        ct = set(t["calls"])
        if len(ct) < min_shared:
            continue
        for m in matched:
            cm = set(m["calls"])
            if len(cm) < min_shared:
                continue
            shared = ct & cm
            if len(shared) < min_shared:
                continue
            j = len(shared) / float(len(ct | cm))
            if j < min_j:
                continue
            pairs.append((t, m, len(shared), j))
    pairs.sort(key=lambda p: (-p[2], -p[3]))
    return pairs


def dataref_neighbours(targets, matched, min_shared):
    """Unmatched functions whose `data_refs` are a SUBSET of a matched one's.

    This is the axis the callee-set screen is blind to, and it is the axis the
    wave-20 W20-C batch actually lived on. sub_08052358 has ONE callee, so
    `|shared callees| >= 4` can never see it -- but its four data_refs are a
    strict subset of the matched sub_08052154's, every type it needed was
    already declared, and it matched on the first attempt. A subset means the
    exemplar's type model covers the target completely: no new struct, no new
    global, no new prototype.

    TWO DEFECTS FIXED IN WAVE 20 (found by W20-A, fixed here):

    1. `matched` used to be filtered `status == "matched" and r["calls"]`. That
       guard belongs to the CALLEE axis -- this function never looks at `calls`
       -- and it silently excluded every matched LEAF, 596 of them, from ever
       being a data_refs exemplar. It is why sub_08075368 was reported as "57%
       of sub_08076494" when its four refs are in fact EQUAL to those of three
       different matched functions the screen could not see.
    2. The reported degree, `len(target_refs) / len(exemplar_refs)`, was
       PRESENTED as a percentage "of" the exemplar and read as coverage. That
       reading carries no information: coverage is 100% by construction, since
       this axis only reports subsets. The number is gone.

       IT WAS NOT, HOWEVER, ANTI-CORRELATED, and the proposal to rank by the
       exemplar's SIZE instead is inverted. That degree is maximised by the
       exemplar whose vocabulary is CLOSEST to the target's, which is the right
       one to read: an exemplar using the target's four globals and nothing
       else is about the same thing. Measured against the two functions this
       axis actually produced in wave 20:

           sub_08052358   5 covering exemplars, sub_08052154 was the one read
                          tightest-first rank 1,  widest-first rank 5
           sub_08050134  11 covering exemplars, sub_08052154 was the one read
                          tightest-first rank 1,  widest-first rank 11

       Widest-first puts the exemplar an agent actually used LAST in both
       cases, because the widest cover of any small ref set is whatever large
       function happens to touch those globals among dozens -- sub_0801258C
       covers sub_08075368's four refs among its own 88, and covers 24 other
       targets the same way. Selection therefore stays tightest-first; what
       changed is that the ratio is no longer printed as though it measured
       coverage.

       Printed instead: the number of refs covered, the TIGHTEST exemplar with
       its own ref count so the fit is visible, and HOW MANY distinct matched
       functions cover the target -- the last of these new, exposed by the leaf
       fix, and the one that separates a lucky pairing from a vocabulary that
       is thoroughly worked out.

    THE TIE-BREAK WAS WRONG, AND WAVE 21 (W21-C) MEASURED IT. Ref-count
    minimality is the right PRIMARY key -- it is the subset guarantee -- but it
    ties constantly, and the tie was broken by `m["size"]` ASCENDING, i.e. by
    preferring the smallest covering exemplar. Smallest means "does least",
    which means "demonstrates fewest of the operations the target performs".

    The controlled case: sub_08012358 (36 B) and sub_08085F40 (80 B) have
    IDENTICAL data_refs -- {gUnknown_03001FFC, gUnknown_03002020,
    gUnknown_03002B28, gUnknown_030030E0} -- and both are zero-call leaves, so
    they tie on every key the screen had except size, and the smallest-first
    tie-break named sub_08012358 for twelve targets. sub_08012358's entire body
    is four zero stores. Against the four 96-116 B targets W21-C took on that
    exact ref set it supplied NOTHING beyond "these globals are declared", and
    it is worse than uninformative: it writes `gUnknown_030030E0.raw = 0`, and
    the union `.raw` spelling is precisely the one that FAILS on any target
    performing a read-modify-write (it drags a live zero into a spurious `orr`
    and costs a register -- see c_08085F40.c). sub_08085F40, the exemplar the
    old tie-break ranked second, is the one that carried the whole batch.

    So the tie-break is now PROXIMITY: closest in size, then closest in callee
    count. On those four targets |dsize| to sub_08085F40 is 16-36 and to
    sub_08012358 is 60-80, so proximity picks the useful one every time. This
    does NOT reinstate the widest-first proposal rejected above: the primary key
    is still minimal ref count, and "nearest" is bounded on both sides where
    "widest" was not.

    `nearest` is also reported separately -- the best covering exemplar by
    proximity ALONE, ignoring ref count -- and printed only when it differs from
    `tightest`. Reporting both is cheap and loses nothing; where they disagree,
    that disagreement is itself the signal that the tightest cover is a
    vocabulary match rather than a shape match.
    """
    out = []
    for t in targets:
        dt = set(t["data_refs"])
        if len(dt) < min_shared:
            continue
        covering = [m for m in matched if dt <= set(m["data_refs"])]
        if not covering:
            continue

        def proximity(m):
            return (abs(m["size"] - t["size"]),
                    abs(len(m["calls"]) - len(t["calls"])),
                    m["name"])

        tightest = min(covering, key=lambda m: (len(m["data_refs"]),)
                       + proximity(m))
        nearest = min(covering, key=proximity)
        out.append((t, tightest, len(dt), len(covering), nearest))
    out.sort(key=lambda p: (-p[3], -p[0]["size"]))
    return out


def near_misses(recs, floor):
    """Unpromoted drafts trymatch already scored at `floor`% or better.

    `work/<fn>/best.json` is written on every MISS and nothing reads it. A
    function sitting at 95%+ that nobody promoted is a stronger lead than any
    overlap score -- somebody has already done the derivation and stopped one
    edit short. Cross-referenced here so the batching list sees it (wave 20).
    """
    out = []
    for r in recs:
        if r["status"] not in ("asm", "parked"):
            continue
        meta = os.path.join(awlib.REPO, "work", r["name"], "best.json")
        if not os.path.exists(meta):
            continue
        try:
            with open(meta, encoding="utf-8") as fh:
                pct = json.load(fh).get("percent", 0)
        except (ValueError, OSError):
            continue
        if pct >= floor:
            out.append((pct, r))
    out.sort(key=lambda x: (-x[0], -x[1]["size"]))
    return out


# ---------------------------------------------------------------------------
# THE DELTA SCREEN (wave 59)
# ---------------------------------------------------------------------------
#
# Everything above this line ranks a function by WHERE IT SITS -- which globals
# it names, which callees it shares, which 4KB block it lives in. That family of
# axis is finished. Wave 57 emptied the never-attempted pool, so every function
# left carries a draft, drafted work converts at 13.6% against 83-86% for
# undrafted work, and the overlap axis now returns 39 blocks holding one
# candidate each. Address locality cannot separate 300 functions that are all
# equally close to something already matched.
#
# What still separates them is the DRAFT: how far its compiled output is from
# the ROM, and in which direction. Wave 58's orchestrator probed 37 loop-carrying
# drafts by hand and got a distribution nothing in tools/ could produce --
#
#     small non-zero delta (induction-variable signature)   23   16 at +/-4
#     size-exact (allocation / block layout)                13
#     large delta (likely shape)                             1
#
# -- and found sub_080200EC, open for seventeen waves, on the first run from
# exactly the small-delta signature. It lived in a scratchpad and the scratchpad
# is gone. This is that probe, made into a maintained tool with a self-test.

DELTA_WIDTH = {"word": 4, "4byte": 4, "long": 4, "2byte": 2, "short": 2,
               "hword": 2, "byte": 1}
DIRECTIVE_RE = re.compile(r"^\.(\w+)")
LABELLED_RE = re.compile(r"^\S+:")


def accumulate(stmts, arm):
    """(code, pad, data) bytes emitted by a sequence of assembler statements.

    THE POINT OF SPLITTING THESE THREE IS THAT trymatch REPORTS ONLY THEIR SUM,
    and the sum is a different quantity from any of them. `sub_08045FC8` reports
    `size: match` at 104 bytes with a candidate whose CODE is 2 bytes shorter:
    the literal pool must start on a 4-byte boundary, so the shortfall comes
    straight back as alignment padding and the totals agree. It is a
    one-extra-instruction residual wearing a size-exact costume, and every
    screen keyed on the reported size will keep re-selecting it forever.

    There is a second way the same lie is told, and it is invisible from the
    other side: `_cand.bin` is objcopy's dump of the whole `.text` SECTION,
    which the assembler rounds up to its 4-byte alignment. So a candidate whose
    code is 2 bytes short of a 2-mod-4 ROM function also reports `size: match`,
    with no `.align` involved at all. 42 of the 283 measurable drafts in the
    tree -- one in seven -- have a reported size that disagrees with their code
    length, in one direction or the other.

    `.align n` is GAS's 2**n form, which is what both agbcc and the disassembly
    in asm/ emit.
    """
    code = pad = data = total = 0
    for s in stmts:
        if s.startswith("."):
            m = DIRECTIVE_RE.match(s)
            d = m.group(1) if m else ""
            if d == "align":
                arg = s.split(None, 1)
                n = int(arg[1].split(",")[0].strip(), 0) if len(arg) > 1 else 2
                need = (-total) % (1 << n)
                pad += need
                total += need
            elif d in DELTA_WIDTH:
                w = DELTA_WIDTH[d] * (s.count(",") + 1)
                data += w
                total += w
            continue
        mn = s.split()[0].lower()
        w = 4 if (arm or mn in ("bl", "blx")) else 2
        code += w
        total += w
    return code, pad, data


def cand_statements(path, name):
    """Statements of `name`'s .text in an agbcc-emitted work/<fn>/<fn>.s."""
    out = []
    seen = False
    with open(path, encoding="utf-8", errors="replace") as fh:
        for raw in fh:
            s = raw.split("@")[0].strip()
            if not s:
                continue
            if not seen:
                seen = (s == name + ":")
                continue
            # `.size` closes the function; `.section` starts .debug_line, which
            # is 200KB of `.byte` in a typical draft and would swamp everything.
            if s.startswith(".size") or s.startswith(".section"):
                break
            if LABELLED_RE.match(s):
                s = s.partition(":")[2].strip()
                if not s:
                    continue
            out.append(s)
    return out


def rom_statements(fn):
    """Statements of one asm/ function, from an awlib.Function."""
    out = []
    for raw in fn.lines:
        s = raw.split("@")[0].strip()
        if not s:
            continue
        if awlib.MACRO_RE.match(s):
            continue
        if LABELLED_RE.match(s):
            # `sub_08059F24: @ 0x08059F24` leaves nothing; `_0805A0F0: .4byte g`
            # leaves the pool word, which is exactly the line that must be
            # counted as DATA and not as an instruction.
            s = s.partition(":")[2].strip()
            if not s:
                continue
        out.append(s)
    return out


def rom_layouts():
    """{name: (code, pad, data)} for every function in asm/."""
    out = {}
    for f in awlib.load_all():
        for fn in f.funcs:
            out[fn.name] = accumulate(rom_statements(fn), fn.mode == "ARM")
    return out


def delta_rows(recs, args):
    """Measure every unmatched draft's residual. Returns (rows, excluded).

    NOTHING HERE COMPILES ANYTHING. `work/<fn>/_cand.bin` and `_target.bin` are
    what trymatch wrote on its last run -- the same two files apply_pending()
    already reads to spot an unpromoted match -- so a full-corpus delta screen
    costs one pass over work/ and one over asm/, not 300 invocations of agbcc.

    THE SCORE IS RECOMPUTED FROM THE BINS, NOT READ FROM best.json. best.json
    tracks `best.c`, which is a DIFFERENT FILE from the draft on disk, and the
    gap between them has misled two waves: wave 56 had best.json reading 33.99%
    at the moment trymatch returned exit 0 on the draft beside it, and wave 57
    had 56.9% quoted for a draft that was 18.5%. The arithmetic below is
    trymatch's own, on trymatch's own output bytes, so it describes the draft.

    THREE INTEGRITY GATES, each reported by name rather than silently dropped.
    A screen whose inputs can go stale without saying so is how this project
    loses screens.

      stale draft      <fn>.c is NEWER than _cand.bin -- somebody edited the
                       draft after the last trymatch run, so the measurement
                       describes code that no longer exists.
      stale .s         <fn>.s is NEWER than _cand.bin. compile_probe and
                       permute.py both write into work/<fn>/, so the assembly
                       listing is not always the compile the .bin came from.
                       sub_0803BF10 has a .s from eight days after its .bin and
                       the two disagree by 24 bytes.
      unaccounted      the parsed layout does not add up to the bytes that
                       actually exist (rounded up to .text's 4-byte alignment on
                       the candidate side, exact on the ROM side). This is the
                       check that makes the code/pool split evidence rather than
                       a plausible-looking number -- if a line cannot be
                       classified, the totals disagree and the row is dropped
                       instead of reported with a wrong split.
    """
    units_path = os.path.join(awlib.REPO, "build", "functions", "units.json")
    base = {}
    if os.path.exists(units_path):
        with open(units_path, encoding="utf-8") as fh:
            for u in json.load(fh):
                for fn in u["functions"]:
                    base[fn] = int(u["addr_hex"], 16)
    roms = rom_layouts()

    rows = []
    excluded = collections.defaultdict(list)
    for r in recs:
        if r["status"] not in ("asm", "parked"):
            continue
        if r["mode"] != "THUMB" or r["trivial"]:
            continue
        d = os.path.join(awlib.REPO, "work", r["name"])
        cb = os.path.join(d, "_cand.bin")
        tb = os.path.join(d, "_target.bin")
        cs = os.path.join(d, r["name"] + ".c")
        sf = os.path.join(d, r["name"] + ".s")
        if not (os.path.exists(cb) and os.path.exists(tb)
                and os.path.exists(cs)):
            excluded["never compiled by trymatch"].append(r)
            continue
        if r["name"] not in base:
            excluded["in no split unit"].append(r)
            continue
        cand = open(cb, "rb").read()
        tgt = open(tb, "rb").read()
        off = r["addr"] - base[r["name"]]
        if off < 0 or off + r["size"] > len(tgt):
            excluded["target slice out of range"].append(r)
            continue
        bin_mtime = os.path.getmtime(cb)
        if os.path.getmtime(cs) > bin_mtime:
            excluded["draft edited since the last trymatch run"].append(r)
            continue
        if not os.path.exists(sf) or os.path.getmtime(sf) > bin_mtime:
            excluded["<fn>.s is not the compile _cand.bin came from"].append(r)
            continue

        cl = accumulate(cand_statements(sf, r["name"]), False)
        rl = roms.get(r["name"])
        if rl is None:
            excluded["not in asm/"].append(r)
            continue
        if -(-sum(cl) // 4) * 4 != len(cand) or sum(rl) != r["size"]:
            excluded["layout does not account for every byte"].append(r)
            continue

        tgt_fn = tgt[off:off + r["size"]]
        cand_fn = cand[:r["size"]]
        n_diff, common, pct = awlib.byte_score(tgt_fn, cand, r["size"])
        code_delta = cl[0] - rl[0]
        rows.append({
            "rec": r, "name": r["name"], "size": r["size"],
            "cand": len(cand), "size_delta": len(cand) - r["size"],
            "code_delta": code_delta, "cand_code": cl[0], "rom_code": rl[0],
            "cand_pool": cl[2], "rom_pool": rl[2], "pct": pct,
            "loops": r["backward_branches"], "status": r["status"],
            # WAVE 59, and it came straight out of testing this screen's own
            # ranking. `status == "parked"` is NOT the whole "a slot has been
            # spent here" signal: sub_08045FC8 ranked near the top of the small
            # group -- 104 bytes, one loop, |code delta| 2, status `asm` -- and
            # it carries three recorded decomp-permuter runs (22,157 iterations
            # in wave 43 alone), ten measured source shapes and four explicitly
            # ruled-out axes. It is one of the most thoroughly exhausted
            # functions in the tree and the screen offered it as fresh work.
            # 43 further `asm`-status drafts are in the same position. The
            # permuter directory is the cheap durable evidence of it -- a park
            # note can be deleted or rewritten by the next agent, a
            # work/<fn>/permuter/ tree survives.
            "permuted": os.path.isdir(os.path.join(d, "permuter")),
            "kind": ("size-exact" if code_delta == 0
                     else "small" if abs(code_delta) <= args.delta_small
                     else "large"),
        })
    return rows, excluded


# The rank keys, in one place, because the whole claim of this screen is that
# ORDER BY RESIDUAL KIND beats order by score and wave 58 measured it twice.
KIND_RANK = {"small": 0, "size-exact": 1, "large": 2}


def delta_key(row, parked_floor):
    """Sort key. Read the comment -- every term in it was measured.

    GROUP ORDER: small-delta above size-exact above large.

      small       A residual of one or two THUMB instructions inside a loop is
                  the induction-variable signature. sub_080200EC was open for
                  seventeen waves and fell on the first attempt once it was
                  read as "the candidate is missing a second source variable";
                  sub_0800CAA0 is the same axis pointing the other way, the
                  candidate carrying a giv the ROM does not. Both are the same
                  small delta. This is the group with a known lever.
      size-exact  Being the right length rules out missing statements and wrong
                  loop structure, but what is left -- allocation, constant
                  placement, cross-jump block selection -- includes two kinds
                  no spelling reaches. It still converted 3 of 11 in wave 58,
                  above the 13.6% drafted baseline, so it is not a bad class;
                  it is a worse class than small.
      large       Probably a shape, and wave 57 measured 11 of 11 drafted
                  residuals as NOT shapes. A large delta here more often means
                  the draft is old than that the shape is wrong.

    WITHIN EVERY KIND, work nobody has spent a slot on sorts first. That is this
    file's existing convention -- report() already builds `targets` from `asm`
    only, because a parked function has had a slot spent on it and was
    deliberately not handed out again -- and the delta screen must not quietly
    reverse it. Such rows are still LISTED, because parked.json's ruled-out axes
    are the most valuable part of a draft and a delta the parker never measured
    is new information; they are just not offered ahead of untouched work.

    "SPENT" IS NOT THE SAME AS "PARKED", and this screen's own first outing
    proved it. sub_08045FC8 came out near the top of the small group and is
    among the most exhausted functions in the tree: three decomp-permuter runs,
    ten measured source shapes, four ruled-out axes, two waves of park notes --
    all of it recorded in the draft, none of it in `status`, which is `asm`. A
    `work/<fn>/permuter/` directory is the durable evidence, so it demotes too;
    43 `asm`-status drafts carry one.

    WITHIN small: loop-carrying first, then |code delta| ascending. NOT by
    score, and that is deliberate rather than an oversight -- this project's
    score is POSITIONAL, so it records where the divergence starts and not how
    much is left, and ranking on it would contradict the rule the rest of this
    file is built on.

      The loops term is the measured one. Wave 58's probe was on loop-carrying
      drafts specifically, and both functions it names -- sub_080200EC, missing
      a second source induction variable, and sub_0800CAA0, carrying a pointer
      giv the ROM does not -- are loop functions whose residual IS the induction
      variable. A small delta with no loop is a different mechanism.

      The |code delta| term is REASONED, NOT MEASURED, and is flagged as such
      so nobody cites it back as a finding: a residual of one THUMB instruction
      is fewer decisions than one of four, and wave 58's data (16 of 23 at
      +/-4) says only that the group is tight, not that +/-2 beats +/-4.

    WITHIN size-exact: score ASCENDING, and 93%+ drafts that have ALREADY BEEN
    PARKED ONCE sink to the bottom of the group. Wave 58 measured six size-exact
    loop functions: both matches came from the lowest-scoring drafts (87.9% and
    66.9%) and all three at 93%+ were instruction-ORDER decisions of which not
    one moved. An order-only difference leaves prologue, body and tail intact so
    it reads HIGH; a wrong register or operand order early reads low and is
    often one edit. The 93%+ drafts are not filtered out -- they are listed with
    a `parked-high` tag, because "screened out" and "ranked last" differ exactly
    when the diff shows something other than order, which is the documented
    exception.
    """
    demote = int(row["kind"] == "size-exact" and row["pct"] >= parked_floor
                 and row["status"] == "parked")
    spent = int(row["status"] == "parked" or row["permuted"])
    if row["kind"] == "small":
        secondary = (spent, 0 if row["loops"] else 1, abs(row["code_delta"]))
    elif row["kind"] == "size-exact":
        secondary = (spent, row["pct"])
    else:
        secondary = (spent, abs(row["code_delta"]))
    return (KIND_RANK[row["kind"]], demote) + secondary + (row["name"],)


def delta_report(recs, args):
    """The wave-59 section. Returns (rows, excluded) for the self-test."""
    rows, excluded = delta_rows(recs, args)
    loopy = [x for x in rows if x["loops"] > 0]
    straight = [x for x in rows if x["loops"] == 0]

    print("\n== RESIDUAL DELTA on the DRAFTED pool (wave 59) ==")
    print("measured: %d drafts, %d bytes  -- from work/<fn>/_cand.bin, written "
          "by the\n          last trymatch run. Nothing here recompiles; the "
          "score is recomputed\n          from the bins, NOT read from "
          "best.json (which tracks best.c)."
          % (len(rows), sum(x["size"] for x in rows)))

    # WHAT THIS SCREEN DOES NOT SEE. Same discipline as --min-size and
    # --block-max above: three floor artifacts in this file have each hidden
    # most of the corpus for multiple waves, and each was found by a wave
    # stumbling on it rather than by the tool saying so.
    total_ex = sum(len(v) for v in excluded.values())
    if total_ex:
        print("NOT MEASURED: %d unmatched non-trivial THUMB function(s), %d "
              "bytes." % (total_ex, sum(r["size"] for v in excluded.values()
                                        for r in v)))
        for reason in sorted(excluded):
            v = excluded[reason]
            print("    %-46s %3d  %s"
                  % (reason, len(v),
                     " ".join(r["name"] for r in v[:6])
                     + (" ..." if len(v) > 6 else "")))
        print("  A draft in that list is NOT a hard function -- it is an "
              "unmeasured one.\n  Re-run tools/trymatch.py on it and it "
              "reappears here.")
    print("  NOTE: this screen ignores --min-size %d entirely. It reads the "
          "whole\n        unmatched pool, including the %d straight-line "
          "function(s) that sit\n        above --block-max %d with no other "
          "screen at all."
          % (args.min_size,
             sum(1 for x in straight if x["size"] > args.block_max),
             args.block_max))

    for label, pool in (("loop-carrying", loopy), ("straight-line", straight)):
        n = collections.Counter(x["kind"] for x in pool)
        print("\n  %s drafts: %d   small %d / size-exact %d / large %d "
              "(|code delta| > %d)"
              % (label, len(pool), n["small"], n["size-exact"], n["large"],
                 args.delta_small))
        near = [x for x in pool
                if x["kind"] == "small" and abs(x["code_delta"]) <= 4]
        if n["small"]:
            print("      of the small group, %d (%.0f%%) sit at |code delta| "
                  "<= 4 -- one or two THUMB\n      instructions, which is the "
                  "induction-variable signature."
                  % (len(near), 100.0 * len(near) / n["small"]))

    # The single most useful number this screen produces, and the reason the
    # code/pool split exists at all.
    liars = [x for x in rows
             if (x["size_delta"] == 0) != (x["code_delta"] == 0)]
    print("\n  REPORTED SIZE DISAGREES WITH CODE LENGTH on %d of %d drafts "
          "(%.0f%%)." % (len(liars), len(rows),
                         100.0 * len(liars) / len(rows) if rows else 0))
    print("      trymatch's `size: match` counts the .text SECTION, which is "
          "code +\n      alignment padding + literal pool, rounded up to 4. Two "
          "bytes lost from\n      the code come straight back as padding. Every "
          "one of these is\n      classified by its CODE length here, so "
          "sub_08045FC8 is small, not exact.")

    print("\n  Ranked by RESIDUAL KIND, not by score -- see delta_key(). "
          "`parked-high`\n  marks a size-exact draft at %.0f%%+ that has "
          "already been parked once:\n  wave 58 measured three of those as "
          "instruction-ORDER decisions and not\n  one moved." % args.delta_parked_floor)
    shown = 0
    order = sorted(rows, key=lambda x: delta_key(x, args.delta_parked_floor))
    if args.delta_loops_only:
        order = [x for x in order if x["loops"] > 0]
    for x in order[:args.delta_top]:
        demote = (x["kind"] == "size-exact"
                  and x["pct"] >= args.delta_parked_floor
                  and x["status"] == "parked")
        print("    %-14s %5dB  %-10s code %+4d  size %+4d  %5.1f%%  "
              "loops=%-2d pool %d/%d  %s%s%s"
              % (x["name"], x["size"], x["kind"], x["code_delta"],
                 x["size_delta"], x["pct"], x["loops"],
                 x["cand_pool"] // 4, x["rom_pool"] // 4, x["status"],
                 "+permuted" if x["permuted"] else "",
                 "  parked-high" if demote else ""))
        shown += 1
    # NO SILENT CAPS -- the lesson of waves 26 and 28, which each read a header
    # count above a shorter listing and concluded the axis was nearly dry.
    if len(order) > shown:
        rest = order[shown:]
        print("    (+%d more NOT SHOWN, %d bytes. Re-run with --delta-top %d. "
              "The counts\n     above cover ALL rows and will not agree with "
              "this listing until you do.)"
              % (len(rest), sum(x["size"] for x in rest), len(order)))
    return rows, excluded


def clusters(pairs, targets, min_shared, min_j):
    """Group unmatched functions that share an exemplar, then merge on
    target-to-target overlap so a sibling with no exemplar of its own still
    lands in the cluster its twin is in."""
    parent = {}

    def find(x):
        while parent.setdefault(x, x) != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    def union(a, b):
        ra, rb = find(a), find(b)
        if ra != rb:
            parent[ra] = rb

    # Sharing an exemplar is NOT enough to merge. A hub exemplar such as
    # sub_0807567C is close to eight unrelated targets, and unioning on it
    # produced one 17-function 7.5 kB blob spanning three subsystems -- not a
    # batch anyone can take. Merge only on TARGET-TO-TARGET overlap; the
    # exemplar list is then a property of the cluster, not its definition.
    named = {t["name"]: t for t in targets}
    hot = sorted(set(t["name"] for t, _, _, _ in pairs))
    for i, a in enumerate(hot):
        ca = set(named[a]["calls"])
        for b in hot[i + 1:]:
            cb = set(named[b]["calls"])
            sh = ca & cb
            if len(sh) >= min_shared and len(sh) / float(len(ca | cb)) >= min_j:
                union(a, b)

    groups = collections.defaultdict(set)
    exemplars = collections.defaultdict(set)
    for t, m, n, j in pairs:
        groups[find(t["name"])].add(t["name"])
        exemplars[find(t["name"])].add(m["name"])
    out = []
    for root, members in groups.items():
        by = sum(named[x]["size"] for x in members)
        out.append((by, sorted(members), sorted(exemplars[root])))
    out.sort(key=lambda g: -g[0])
    return out


def report(args):
    recs = load_index()
    bodies = asm_bodies()
    pending = apply_pending(recs) if args.pending else []
    hubs = drop_hub_callees(recs, args.max_fanin)

    # TWO pools, deliberately. The callee axis needs an exemplar that HAS
    # callees; the data_refs axis does not, and serving both from one list
    # excluded every matched LEAF -- 596 of them -- from the subset screen for
    # as long as the `and r["calls"]` guard was shared (wave 20, found by
    # W20-A). Do not re-merge them.
    matched = [r for r in recs if r["status"] == "matched" and r["calls"]]
    matched_all = [r for r in recs if r["status"] == "matched"]
    # Normal runs batch on `asm` only -- parked functions are deliberately not
    # handed out again. The self-test must still see them: its two cluster
    # checks anchor on sub_08052718/sub_08052BBC, which wave 20 PARKED after
    # writing the test, so from wave 21 on those checks could not pass however
    # healthy the screen was. A permanently-red acceptance test hides the
    # regression it exists to catch, so widen the pool for --self-test only.
    ok_status = ("asm", "parked") if args.include_parked else ("asm",)
    targets = [r for r in recs
               if r["status"] in ok_status and r["mode"] == "THUMB"
               and r["size"] >= args.min_size and not r["trivial"]]

    print("corpus: %d functions, %d matched, %d unmatched THUMB >= %dB "
          "(%d bytes)"
          % (len(recs), sum(1 for r in recs if r["status"] == "matched"),
             len(targets), args.min_size, sum(t["size"] for t in targets)))

    # WHAT THE FLOOR HID. Every axis below reports on `targets`, so a `--min-size`
    # that excludes most of the corpus makes "0 pairs" read as "this axis is
    # exhausted" when it means "this axis was not asked". Wave 20 established
    # that exemplar proximity is the floor and SIZE IS NOT, and the flag
    # outlived the lesson: wave 24 found the default of 96 hiding 2,986
    # unmatched functions, and dropping it to 24 resurfaced duplicate pairs
    # immediately -- including one wave 21 had backed out and left free.
    below = [r for r in recs
             if r["status"] in ok_status and r["mode"] == "THUMB"
             and not r["trivial"] and r["size"] < args.min_size]
    if below:
        print("EXCLUDED BY --min-size %d: %d unmatched non-trivial THUMB "
              "functions, %d bytes." % (args.min_size, len(below),
                                        sum(r["size"] for r in below)))
        print("  A dry result below is a statement about THIS FLOOR, not about "
              "the corpus.")
        if args.min_size > 24:
            print("  Re-run with --min-size 24 before concluding an axis is "
                  "exhausted.")
    if pending:
        print("pending (matched by trymatch, not yet promoted): %d counted as "
              "exemplars" % len(pending))
    if hubs:
        print("hub callees dropped (fan-in > %d): %d -- %s"
              % (args.max_fanin, len(hubs),
                 ", ".join(sorted(hubs)[:8])
                 + (" ..." if len(hubs) > 8 else "")))

    pairs = overlaps(targets, matched, args.min_shared, args.min_j)
    covered = {}
    for t, m, n, j in pairs:
        if t["name"] not in covered or n > covered[t["name"]][1]:
            covered[t["name"]] = (t, n, j, m["name"])
    print("\n== overlap with a MATCHED exemplar "
          "(|shared| >= %d, J >= %.2f) ==" % (args.min_shared, args.min_j))
    print("pairs: %d   distinct unmatched functions: %d   bytes: %d"
          % (len(pairs), len(covered), sum(v[0]["size"] for v in covered.values())))

    strict, loose, by_callee = duplicates(recs, bodies)

    def kinds(group):
        st = set(r["status"] for r in group)
        if "matched" in st and len(st) > 1:
            return "FREE"          # an unmatched twin of a matched function
        if st == {"matched"}:
            return "both-matched"
        return "pair"              # two unmatched siblings: halves the work

    print("\n== byte-identical duplicates (labels stripped) ==")
    for tag, table in (("strict (pool symbols too)", strict),
                       ("loose  (pool slots only)", loose)):
        free = [g for g in table.values() if kinds(g) == "FREE"]
        pair = [g for g in table.values() if kinds(g) == "pair"]
        print("  %s: %d groups -- %d with a matched member (%d free bytes), "
              "%d all-unmatched (%d bytes)"
              % (tag, len(table), len(free),
                 sum(r["size"] for g in free for r in g
                     if r["status"] != "matched"),
                 len(pair), sum(r["size"] for g in pair for r in g)))

    print("\n  LOOSE groups with a MATCHED member -- a free match if the pool"
          "\n  words really are interchangeable; check each by hand:")
    any_free = False
    for g in sorted(loose.values(), key=lambda g: -max(r["size"] for r in g)):
        if kinds(g) != "FREE":
            continue
        any_free = True
        print("    %5dB  %s" % (g[0]["size"],
              "  ".join("%s[%s]" % (r["name"], r["status"]) for r in g)))
    if not any_free:
        print("    (none)")

    strict_sets = [frozenset(r["name"] for r in g) for g in strict.values()]
    print("\n  LOOSE groups, all-unmatched, >= %dB -- ONE SHAPE with differing"
          "\n  pool symbols; derive one member, transcribe the rest:"
          % args.min_size)
    shown = 0
    for g in sorted(loose.values(), key=lambda g: -max(r["size"] for r in g)):
        if kinds(g) != "pair" or max(r["size"] for r in g) < args.min_size:
            continue
        if frozenset(r["name"] for r in g) in strict_sets:
            continue
        print("    %5dB x%d  %s" % (g[0]["size"], len(g),
                                    "  ".join(r["name"] for r in g)))
        shown += 1
        if shown >= args.top:
            break
    if not shown:
        print("    (none)")

    print("\n  STRICT groups worth taking (>= %dB members, unmatched involved):"
          % args.min_size)
    shown = 0
    for g in sorted(strict.values(), key=lambda g: -sum(r["size"] for r in g)):
        if all(r["status"] == "matched" for r in g):
            continue
        if max(r["size"] for r in g) < args.min_size:
            continue
        print("    %5dB  %-9s %s"
              % (g[0]["size"], kinds(g),
                 "  ".join("%s[%s]" % (r["name"], r["status"]) for r in g)))
        shown += 1
        if shown >= args.top:
            break
    if not shown:
        print("    (none)")

    # WAVE 38: the callee-normalised tier. Reported separately from strict and
    # loose because it is a WEAKER claim -- one shape whose `bl` targets differ
    # -- but it is the tier that catches this ROM's commonest repeat, and the
    # first pair it found was invisible to both of the others.
    print("\n  CALLEE-NORMALISED groups (one shape, different `bl` targets --"
          "\n  derive one member, transcribe the rest with the callee swapped):")
    seen_sets = set(frozenset(r["name"] for r in g)
                    for g in list(strict.values()) + list(loose.values()))
    shown = 0
    for g in sorted(by_callee.values(), key=lambda g: -sum(r["size"] for r in g)):
        if all(r["status"] == "matched" for r in g):
            continue
        if max(r["size"] for r in g) < args.min_size:
            continue
        if frozenset(r["name"] for r in g) in seen_sets:
            continue          # already reported by a stronger tier
        print("    %5dB  %-9s %s"
              % (g[0]["size"], kinds(g),
                 "  ".join("%s[%s]" % (r["name"], r["status"]) for r in g)))
        shown += 1
        if shown >= args.top:
            break
    if not shown:
        print("    (none)")

    subs = dataref_neighbours(targets, matched_all, args.min_shared)
    print("\n== data_refs a strict SUBSET of a matched function's "
          "(>= %d refs) ==" % args.min_shared)
    print("functions: %d   bytes: %d   (the axis |shared callees| cannot see)"
          % (len(subs), sum(t["size"] for t, _, _, _, _ in subs)))
    print("  All coverage here is 100% -- this axis only reports SUBSETS, so a"
          "\n  percentage would carry no information. exemplars=N is how many"
          "\n  DISTINCT matched functions cover the target. `tightest` is the"
          "\n  smallest covering VOCABULARY, ties broken by proximity in size and"
          "\n  callee count; `nearest` is the closest covering SHAPE ignoring ref"
          "\n  count, printed only where the two disagree. READ BOTH -- wave 21"
          "\n  measured a 36-byte all-zero-stores leaf winning the old"
          "\n  smallest-first tie-break for twelve targets and teaching nothing.")
    for t, m, covers, n, near in subs[:args.top]:
        line = ("    %-14s %4dB  %d refs, exemplars=%-3d tightest %s "
                "(%d refs, %dB)"
                % (t["name"], t["size"], covers, n, m["name"],
                   len(m["data_refs"]), m["size"]))
        if near["name"] != m["name"]:
            line += "  nearest %s (%d refs, %dB)" % (
                near["name"], len(near["data_refs"]), near["size"])
        print("%s  [%s]" % (line, t["src"]))

    nm = near_misses(recs, args.near_miss)
    print("\n== unpromoted near-misses: work/<fn>/best.json >= %.0f%% =="
          % args.near_miss)
    print("functions: %d   bytes: %d   (somebody already derived these and "
          "stopped short)" % (len(nm), sum(r["size"] for _, r in nm)))
    for pct, r in nm[:args.top]:
        print("    %-14s %5dB  %5.1f%%  [%s]"
              % (r["name"], r["size"], pct, r["status"]))

    print("\n== top %d clusters by bytes ==" % args.top)
    named = {t["name"]: t for t in targets}
    for by, members, exs in clusters(pairs, targets,
                                     args.min_shared, args.min_j)[:args.top]:
        core = set(named[members[0]]["calls"])
        for name in members[1:]:
            core &= set(named[name]["calls"])
        print("  %5dB  %d fn  exemplars: %s"
              % (by, len(members), ", ".join(exs[:4])
                 + (" ..." if len(exs) > 4 else "")))
        print("           core (callees shared by ALL %d): %s"
              % (len(members),
                 ", ".join(sorted(core)) if core
                 else "EMPTY -- transitive chain, split it before batching"))
        for name in sorted(members, key=lambda n: -named[n]["size"]):
            t = named[name]
            best = covered[name]
            print("           %-14s %4dB  |shared|=%d J=%.3f  vs %s"
                  % (name, t["size"], best[1], best[2], best[3]))

    # Every axis above is shape-based and they go dry together; when they did,
    # this was the one that still had work in it. As of wave 59 it is going the
    # same way -- 39 blocks holding ONE candidate each -- so the delta screen
    # below it is now the last thing on screen, on the same reasoning.
    block_offered = []
    if not args.no_blocks:
        _, block_offered = blocks(recs, args)

    # Runs LAST on purpose (wave 59). Address locality cannot separate a pool
    # in which every member already carries a draft; what the residual IS can.
    delta = ([], {})
    if not args.no_delta:
        delta = delta_report(recs, args)

    return pairs, covered, strict, loose, targets, block_offered, delta


def blocks(recs, args):
    """Rank 4KB address blocks by how cheap they are to batch.

    THIS IS THE AXIS THAT CARRIED WAVE 24 (109 functions from 39 assigned) and
    it is not a shape metric at all. Every shape-based screen above was dry
    that wave -- families 0 with_exemplar, callee overlap 0 pairs, duplicates 0
    usable groups, data_refs subset down to 3 -- and the work was still there.

    The insight is that a block's cost is its VOCABULARY, not its shapes. Ten
    small functions from one already-half-promoted block reuse the same globals,
    the same struct layouts and the same callee prototypes; the promoted
    siblings have already written all of that down. The shapes are unrelated and
    that does not matter. Wave 24's W24-C matched its assigned 13 in two
    compile_probe rounds and 13 first-attempt try_match calls.

    Two limits, both measured, both worth stating in the brief:
      - It is only cheap where the block is ALREADY half promoted. A cold block
        has no vocabulary to inherit and behaves nothing like this.
      - These are small functions. Wave 24's 109 are ~4,500 bytes; wave 23's 17
        were 3,344. Function count and byte count diverged long ago.

    AND DO NOT BATCH ON A (size, calls, refs) SIGNATURE. It looks like it finds
    25-member families. Wave 24 checked three members of one and got a rounding
    divide, a byte-store wrapper and a stack-args forwarder -- the same
    false-positive mode wave 21 recorded for duplicate signatures. Group by
    LOCALITY and tell the agent the shapes are unrelated.
    """
    promoted_units = set()
    try:
        with open(os.path.join(awlib.DATA_DIR, "promoted.json"),
                  encoding="utf-8") as fh:
            for u in json.load(fh):
                m = re.search(r'c_([0-9A-Fa-f]{8})\.c', u["file"])
                if m:
                    promoted_units.add(int(m.group(1), 16))
    except (OSError, ValueError):
        pass

    # WAVE 30 MEASURED THE COST SIGNAL, AND IT IS NOT DENSITY. Blocks at
    # density 9/10/10 went 45/45 at mean 1.24 attempts; 29 of 45 functions
    # authored new globals/structs and 24 of them still fell first try,
    # because a global's width and signedness read straight off the target's
    # own loads. What cost attempts was CALLEE PROTOTYPES -- and callees are
    # not address-local, so density never could have predicted it. The screen
    # below therefore counts each block's UNDECLARED, UNPROMOTED callees:
    #   - declared in include/unknown-functions.h  -> free
    #   - promoted (status matched)                -> free, even if undeclared
    #     (the definition exists; wave 30 had three of these and each cost
    #     nothing)
    #   - anything else named by a `bl`            -> declarations to WRITE
    #
    # WAVE 31 THEN REFUTED THE COUNT AS A COST RANKING, from both directions
    # in one wave: W31-A's callee-HEAVY tier went 20/20 while both its parks
    # came from the ONE-undeclared tier, and W31-C's nine-undeclared block was
    # the cheapest it touched. An undeclared callee called with literals
    # constrains nothing -- arity and void-ness read off the callee's own
    # prologue -- so the count predicts WORK (declarations an agent must
    # write), not COST (attempts). It is printed per block as a workload note
    # and is NO LONGER THE SORT KEY. Blocks list in ADDRESS ORDER, which is
    # the one batching signal that has survived measurement (wave 34:
    # adjacency tracked hit rate almost monotonically while size and count
    # were flat; feeding slots in address order compounds vocabulary because
    # each finished block hands the next its callees and drafts).
    declared = set()
    try:
        with open(os.path.join(awlib.REPO, "include", "unknown-functions.h"),
                  encoding="utf-8", errors="replace") as fh:
            for ln in fh:
                m = re.match(r'\s*[A-Za-z_][\w\s*]*?\b(sub_[0-9A-Fa-f]{8})'
                             r'\s*\(', ln)
                if m and not ln.lstrip().startswith(("*", "/*", "//")):
                    declared.add(m.group(1))
    except OSError:
        pass
    status_of = {r["name"]: r["status"] for r in recs}

    def undeclared_callees(fns):
        out = set()
        for r in fns:
            for c in r.get("calls", []):
                if not c.startswith("sub_"):
                    continue          # upstream-named: declared in its header
                if c in declared or status_of.get(c) == "matched":
                    continue
                out.add(c)
        return sorted(out)

    named = [r for r in recs if r["name"].startswith("sub_")]
    addr = {r["name"]: int(r["name"][4:], 16) for r in named}
    matched_addrs = sorted(addr[r["name"]] for r in named
                           if r["status"] == "matched")

    # Wave 37: the loop filter was the fourth and largest screen artifact.
    # `--allow-loops` lifts it; the band it admits measured at or above the
    # straight-line hit rate. Loop candidates are marked `[loop]` in the
    # listing so a batch can still be kept to one kind if a wave wants that.
    def in_band(r):
        return (r["status"] == "asm" and r["mode"] == "THUMB"
                and not r["trivial"]
                and (args.allow_loops or r["backward_branches"] == 0))

    pool = [r for r in named
            if in_band(r) and args.block_min <= r["size"] <= args.block_max]

    # Wave 33: the CEILING is a floor artifact too. At the old default of 72
    # this screen read "2 blocks, 3 candidates" -- indistinguishable from an
    # exhausted axis -- while 565 candidates (80KB) sat between 72 and 256.
    # Report what the ceiling excludes, exactly as --min-size reports its
    # floor, so the next artifact self-reports instead of waiting for a wave
    # to stumble on it.
    above = [r for r in named if in_band(r) and r["size"] > args.block_max]

    # And report what the LOOP filter excludes, for the same reason the
    # ceiling and the floor report theirs.
    loops_hidden = [] if args.allow_loops else [
        r for r in named
        if r["status"] == "asm" and r["mode"] == "THUMB" and not r["trivial"]
        and r["backward_branches"] > 0
        and args.block_min <= r["size"] <= args.block_max]

    by_block = collections.defaultdict(list)
    for r in pool:
        by_block[addr[r["name"]] >> 12].append(r)

    rows = []
    for blk, fns in by_block.items():
        n_matched = sum(1 for a in matched_addrs if a >> 12 == blk)
        n_units = sum(1 for a in promoted_units if a >> 12 == blk)
        if n_matched < args.block_min_matched:
            continue
        und = undeclared_callees(fns)
        rows.append((len(fns), n_matched, n_units, blk, fns, und))
    # ADDRESS ORDER, deliberately: the undeclared-callee ratio was the sort
    # key from wave 30 until wave 39, and wave 31 had already refuted it as a
    # cost ranking (see the comment above undeclared_callees). Address order
    # is the signal wave 34 validated and the order waves feed slots in
    # anyway -- a sort the orchestrator overrides every wave is worse than
    # none, because --top-blocks truncates on it.
    rows.sort(key=lambda x: x[3])

    print("\n== ADDRESS-LOCALITY BLOCKS (%d-%dB, %s, non-trivial, "
          ">= %d matched in block) ==" % (args.block_min, args.block_max,
                                          "loops INCLUDED" if args.allow_loops
                                          else "straight-line",
                                          args.block_min_matched))
    print("blocks: %d   candidate functions: %d   bytes: %d"
          % (len(rows), sum(r[0] for r in rows),
             sum(f["size"] for r in rows for f in r[4])))
    if above:
        print("EXCLUDED BY --block-max %d: %d straight-line unmatched THUMB "
              "functions, %d bytes, sit ABOVE the ceiling."
              % (args.block_max, len(above), sum(r["size"] for r in above)))
        print("  A dry result here is a statement about THIS CEILING, not the "
              "corpus (wave 33).")
    if loops_hidden:
        print("EXCLUDED BY THE LOOP FILTER: %d unmatched THUMB functions in "
              "band, %d bytes,\n  have backward branches. Wave 37 measured "
              "that band at 39/41 and 62-166 B/attempt --\n  pass "
              "--allow-loops to screen them."
              % (len(loops_hidden), sum(r["size"] for r in loops_hidden)))
    print("  Cost is the block's VOCABULARY, not its shapes -- see blocks().")
    print("  Give ONE block per agent and name each target's nearest matched")
    print("  neighbour as its exemplar. Tell the agent the shapes are NOT")
    print("  variations of each other.")

    offered = []          # [(record, nearest_matched_addr, exemplar_path)]
    for n, n_matched, n_units, blk, fns, und in rows[:args.top_blocks]:
        print("\n  block 0x%05X000  %2d candidates, %4d bytes  "
              "(%d matched, %d promoted units in block)"
              % (blk, n, sum(f["size"] for f in fns), n_matched, n_units))
        if und:
            print("    undeclared callees: %d (declarations to WRITE -- a "
                  "workload note, not a cost rank; wave 31) -- %s%s"
                  % (len(und), " ".join(und[:6]),
                     " (+%d more)" % (len(und) - 6) if len(und) > 6 else ""))
        else:
            print("    undeclared callees: 0 -- every bl target is declared"
                  " or promoted")
        shown = 0
        for r in sorted(fns, key=lambda x: addr[x["name"]])[:args.per_block]:
            a = addr[r["name"]]
            i = bisect.bisect_left(matched_addrs, a)
            near = min((matched_addrs[j] for j in (i - 1, i)
                        if 0 <= j < len(matched_addrs)),
                       key=lambda x: abs(x - a), default=None)
            if near is None:
                continue
            shown += 1
            # The exemplar is the promoted UNIT that contains the nearest
            # matched neighbour, i.e. the greatest unit address <= it. That is
            # a guess about unit boundaries rather than a lookup, which is why
            # --self-test checks every path it prints actually exists.
            ex = [u for u in promoted_units if u <= near]
            exf = "src/decomp/c_%08X.c" % max(ex) if ex else None
            offered.append((r, near, exf))
            print("    %-16s %3dB calls=%-2d refs=%-2d  nearest sub_%08X  %s%s"
                  % (r["name"], r["size"], len(r["calls"]),
                     len(r["data_refs"]), near, exf or "?",
                     "  [loop x%d]" % r["backward_branches"]
                     if r["backward_branches"] else ""))
        # NO SILENT CAPS. This listing IS the batching list, so a block that
        # says "19 candidates" and prints 14 hands the next wave a short batch
        # that looks complete. Wave 26 caught it only by comparing the two
        # numbers by hand.
        if shown < n:
            print("    (+%d more not shown -- re-run with --per-block %d)"
                  % (n - shown, n))

    # NO SILENT CAPS -- AT THE BLOCK LEVEL EITHER. The per-block notice above
    # has existed since wave 26; this one had not, so wave 28 read a header
    # saying "blocks: 28, candidate functions: 284" above six listings totalling
    # 120 and never saw the other 164. A truncated batching list does not look
    # truncated -- it looks like the axis is nearly exhausted, which is the
    # single most expensive wrong belief a wave can start from.
    if len(rows) > args.top_blocks:
        hidden = rows[args.top_blocks:]
        print("\n  (+%d more block(s) NOT SHOWN: %d further candidate "
              "function(s), %d bytes.\n"
              "   re-run with --top-blocks %d to see them. The header count "
              "above covers\n"
              "   ALL blocks, so it will not agree with the listings until you "
              "do.)"
              % (len(hidden), sum(len(r[4]) for r in hidden),
                 sum(f["size"] for r in hidden for f in r[4]), len(rows)))
    return rows, offered


def delta_self_test(args, delta):
    """The wave-59 acceptance test for the delta screen.

    Three criteria, set by the wave brief before the screen was written, plus
    two structural ones. The point of writing them down before the tool is that
    a screen which produces plausible-looking output it cannot validate is worse
    than no screen -- that sentence is why families.py has a self-test and why
    the ad-hoc wave-58 probe this replaces was worth nothing once its scratchpad
    was deleted.

    ANCHORS RETIRE GRACEFULLY. Criteria 2 and 3 name three specific functions,
    and the moment one is matched and promoted it leaves the drafted pool and
    the check can never pass again. Wave 20 wrote two anchors that wave 21
    parked, and from wave 21 to wave 33 this file's self-test carried two
    permanently-red checks -- which hides the regression the test exists to
    catch. So an anchor that is no longer unmatched PASSES with a loud note
    saying it has retired, and only a REPORTED-BUT-WRONG anchor fails.

    TWO WAYS TO STOP BEING UNMATCHED, AND THE SECOND ONE BIT WITHIN AN HOUR OF
    THIS TEST BEING WRITTEN. "Gone from the pool" only catches PROMOTION, which
    happens at wave end. The former sub_08059F24/sub_0805A008 anchor matched by
    try_match later in the same wave that wrote this check, so they were still
    `status: asm` in data/functions.json -- still measured, still listed, now
    reporting +0 and 100.0% -- and criterion 2 went red for the best possible
    reason. A row whose candidate reproduces the ROM bytes is a MATCH pending
    promotion (this is what apply_pending() reads the same files for), and it
    retires the anchor exactly as promotion would.
    """
    rows, excluded = delta
    if not rows:
        print("\n[self-test] delta screen produced NO rows: FAIL (nothing to "
              "check -- run tools/trymatch.py on at least one draft)")
        return False
    ok = True
    by_name = {x["name"]: x for x in rows}
    loopy = [x for x in rows if x["loops"] > 0]
    n = collections.Counter(x["kind"] for x in loopy)

    # (1) The wave-58 distribution SHAPE on the loop-carrying drafted pool.
    # Asserted as ratios, not counts: wave 58's absolute numbers (23/13/1) came
    # from 37 hand-probed drafts and this screen reads the whole pool, so any
    # hardcoded count would be wrong on the first run and rot on every later
    # one. The three claims that are actually the finding: small is the biggest
    # group, size-exact is a real second group rather than a rounding artifact,
    # large is nearly empty, and the small group piles up at one or two THUMB
    # instructions.
    near = [x for x in loopy
            if x["kind"] == "small" and abs(x["code_delta"]) <= 4]
    tests = [
        ("small is the largest group", n["small"] > n["size-exact"]),
        ("size-exact is a real group (>= 10%)",
         n["size-exact"] >= 0.10 * len(loopy)),
        ("large is nearly empty (<= 20%)", n["large"] <= 0.20 * len(loopy)),
        ("the small group clusters at |code delta| <= 4 (>= 40%)",
         n["small"] and len(near) >= 0.40 * n["small"]),
    ]
    shape_ok = all(t[1] for t in tests)
    print("\n[self-test] (1) wave-58 distribution shape reproduced on %d "
          "loop-carrying drafts\n            (small %d / size-exact %d / large "
          "%d; %d of the small at |delta| <= 4): %s"
          % (len(loopy), n["small"], n["size-exact"], n["large"], len(near),
             "PASS" if shape_ok else "FAIL"))
    for label, good in tests:
        if not good:
            print("              FAILED: %s" % label)
    ok &= shape_ok

    # (2) The active callee-normalised pair. Their instruction streams differ
    # only at the bl target and their configured drafts have the same exact-size
    # residual, so the delta screen must continue to classify them together.
    def solved(name):
        """Matched by trymatch, whether or not it has been promoted yet."""
        x = by_name.get(name)
        return x is None or (x["size_delta"] == 0 and x["pct"] >= 99.999)

    pair = ("sub_0802E010", "sub_0802E130")
    have = [p for p in pair if not solved(p)]
    if not have:
        print("[self-test] (2) pair %s: PASS (ANCHOR RETIRED -- both now "
              "reproduce the ROM\n            bytes. Replace this anchor with "
              "another verified twin pair.)" % "/".join(pair))
    else:
        got = [by_name[p] for p in have]
        # The absolute score is not the invariant: both drafts have improved
        # together several times since this anchor was introduced.  What makes
        # the pair a useful acceptance test is that one derivation continues
        # to give the same exact-size residual in both functions.
        good = (len(have) == 2
                and len({(x["kind"], x["size_delta"]) for x in got}) == 1
                and got[0]["kind"] == "size-exact"
                and got[0]["size_delta"] == 0
                and abs(got[0]["pct"] - got[1]["pct"]) < 0.1)
        print("[self-test] (2) %s reported TOGETHER at size %+d, %s: %s"
              % ("/".join(p[4:] for p in have), got[0]["size_delta"],
                 ", ".join("%.1f%%" % x["pct"] for x in got),
                 "PASS" if good else "FAIL"))
        if not good and len(have) == 1:
            print("              only one of the two is in the pool")
        ok &= good

    # (3) The size-exact costume. This is the check the whole code/pool split
    # exists for, and it is the one that would silently rot a screen keyed on
    # trymatch's reported size into re-selecting the same function every wave.
    liar = "sub_08045FC8"
    if solved(liar):
        print("[self-test] (3) %s: PASS (ANCHOR RETIRED -- it now reproduces "
              "the ROM bytes)" % liar)
    else:
        x = by_name[liar]
        good = x["kind"] != "size-exact" and x["size_delta"] == 0
        print("[self-test] (3) %s reports size %+d but code %+d, so it is "
              "classified `%s`\n            and NOT size-exact: %s"
              % (liar, x["size_delta"], x["code_delta"], x["kind"],
                 "PASS" if good else "FAIL"))
        ok &= good

    # (4) Every reported row's layout accounts for every byte that exists. This
    # is what separates a measurement from a plausible number: if a line of
    # assembly cannot be classified as code, padding or data, the totals
    # disagree and delta_rows() drops the row into `excluded` rather than
    # reporting a wrong split. Re-derive it here independently of the filter.
    bad = []
    roms = rom_layouts()
    for x in rows:
        d = os.path.join(awlib.REPO, "work", x["name"])
        cl = accumulate(cand_statements(
            os.path.join(d, x["name"] + ".s"), x["name"]), False)
        n_bytes = os.path.getsize(os.path.join(d, "_cand.bin"))
        if -(-sum(cl) // 4) * 4 != n_bytes or sum(roms[x["name"]]) != x["size"]:
            bad.append(x["name"])
    print("[self-test] (4) every reported row's code+pad+pool accounts for "
          "every byte: %s"
          % ("PASS (%d rows)" % len(rows) if not bad
             else "FAIL (%d bad: %s)" % (len(bad), " ".join(bad[:4]))))
    ok &= not bad

    # (5) No silent truncation, the lesson of waves 26 and 28. The listing is a
    # batching list; a clipped one looks like an exhausted axis.
    full = argparse.Namespace(**{**vars(args), "delta_top": 10 ** 6})
    deep = delta_rows(load_index(), full)[0]
    good = len(deep) == len(rows)
    print("[self-test] (5) every counted row is reachable via --delta-top: %s"
          % ("PASS (%d)" % len(deep) if good
             else "FAIL (%d counted, %d reachable)" % (len(rows), len(deep))))
    ok &= good

    # (6) The exclusion list must be REPORTED, not silent -- and it must be
    # accurate, which means a function is in exactly one of the two.
    measured = set(x["name"] for x in rows)
    skipped = set(r["name"] for v in excluded.values() for r in v)
    pool = set(r["name"] for r in load_index()
               if r["status"] in ("asm", "parked") and r["mode"] == "THUMB"
               and not r["trivial"])
    good = (measured | skipped) == pool and not (measured & skipped)
    print("[self-test] (6) measured + excluded partitions the unmatched pool "
          "exactly: %s"
          % ("PASS (%d + %d = %d)" % (len(measured), len(skipped), len(pool))
             if good else
             "FAIL (%d + %d vs %d; %d in both, %d in neither)"
             % (len(measured), len(skipped), len(pool),
                len(measured & skipped), len(pool - measured - skipped))))
    ok &= good
    return ok


def self_test(args):
    """The wave-20 acceptance test -- the screen must rediscover a cluster we
    already know is real, with no knowledge of it."""
    pairs, covered, strict, loose, targets, block_offered, delta = report(args)
    ok = True
    ok &= delta_self_test(args, delta)

    dup = [g for g in list(strict.values()) + list(loose.values())
           if set(r["name"] for r in g) >= {"sub_08052650", "sub_08052AF4"}]
    print("\n[self-test] sub_08052650 / sub_08052AF4 byte-identical: %s"
          % ("PASS" if dup else "FAIL"))
    ok &= bool(dup)

    for t in ("sub_08052718", "sub_08052BBC"):
        exs = set(m["name"] for tt, m, _, _ in pairs if tt["name"] == t)
        want = {"sub_08051BEC", "sub_08051F4C"}
        hit = want & exs
        print("[self-test] %s grouped with %s: %s"
              % (t, "/".join(sorted(want)),
                 "PASS (%s)" % ", ".join(sorted(hit)) if hit else "FAIL"))
        ok &= bool(hit)

    # The block screen's acceptance test. It has no "known real cluster" to
    # rediscover the way the axes above do, so it is checked on the two things
    # that would actually make it lie to a brief: offering a function that is
    # not available, and naming an exemplar file that does not exist. The
    # second is the realistic bug -- the exemplar is resolved as "the greatest
    # promoted unit address <= the nearest matched neighbour", which is a
    # guess about unit boundaries, not a lookup.
    bad_status = [r["name"] for r, _, _ in block_offered
                  if r["status"] != "asm"]
    print("\n[self-test] block screen offers only unmatched targets: %s"
          % ("PASS (%d offered)" % len(block_offered) if not bad_status
             else "FAIL (%s)" % ", ".join(bad_status[:4])))
    ok &= not bad_status

    # An EMPTY block screen is a legitimate state -- wave 27 promoted the last
    # of the twelve blocks -- but it is also exactly what a broken screen looks
    # like, so it may not simply pass. Distinguish the two the way the project
    # distinguishes every other dry axis: drop the matched-neighbour floor,
    # then raise the size ceiling. If either probe finds work, the screen works
    # and only the configured band is exhausted. The second probe matters once
    # the corpus has no non-trivial functions left at or below --block-max.
    #
    # This assertion used to be `ok &= ... and bool(block_offered)` while the
    # label above only tested `bad_status`, so the moment wave 27 emptied the
    # last block the test printed "PASS (0 offered)" on the very line that was
    # failing, and only the summary went red. A check whose label disagrees
    # with its verdict is worse than no check.
    if not block_offered:
        probe = argparse.Namespace(**vars(args))
        probe.block_min_matched = 1
        deep = blocks(load_index(), probe)[1]
        wide_probe = argparse.Namespace(**vars(probe))
        wide_probe.block_max = max(args.block_max * 2, 512)
        wide_probe.top_blocks = 10 ** 6
        wide = blocks(load_index(), wide_probe)[1]
        healthy = bool(deep or wide)
        print("[self-test] block screen is EMPTY -- lowering the floor to "
              "--block-min-matched 1 finds %d; raising --block-max to %d "
              "finds %d: %s"
              % (len(deep), wide_probe.block_max, len(wide),
                 "PASS (screen works; the configured band is exhausted, not "
                 "broken)" if healthy
                 else "FAIL (nothing at either boundary -- suspect the screen)"))
        ok &= healthy

    # Wave 28: the block LIST itself was silently truncated to --top-blocks
    # (default 6). The header counted all 28 blocks while six were listed, so a
    # wave read "284 candidates" above 120 rows and never learned the other 164
    # existed. Truncation is fine; unannounced truncation is not, because a short
    # batching list looks like an exhausted axis rather than a clipped one.
    #
    # Assert the property that actually matters -- REACHABILITY. Every candidate
    # the header counts must be obtainable by raising --top-blocks, and the
    # totals must agree once it is raised. This catches both the missing notice
    # and any future off-by-one in the slice.
    all_rows = blocks(load_index(), args)[0]
    if len(all_rows) > args.top_blocks:
        full = argparse.Namespace(**vars(args))
        full.top_blocks = len(all_rows)
        full.per_block = max(len(r[4]) for r in all_rows)
        deep = blocks(load_index(), full)[1]
        want = sum(len(r[4]) for r in all_rows)
        good = len(deep) == want
        print("[self-test] block list truncated to %d of %d -- every hidden "
              "candidate is reachable via --top-blocks: %s"
              % (args.top_blocks, len(all_rows),
                 "PASS (%d of %d offered when raised)" % (len(deep), want)
                 if good else
                 "FAIL (%d offered, header counts %d)" % (len(deep), want)))
        ok &= good

    missing = [p for _, _, p in block_offered
               if not p or not os.path.isfile(os.path.join(awlib.REPO, p))]
    print("[self-test] every named exemplar file exists: %s"
          % ("PASS" if not missing
             else "FAIL (%d bad: %s)"
                  % (len(missing), ", ".join(sorted(set(map(str, missing)))[:3]))))
    ok &= not missing

    # Wave 33: the CEILING check. --block-max hid 565 candidates behind
    # "2 blocks, 3 candidates" for nine waves because nothing asserted the
    # ceiling reports what it excludes. Same discipline as the floor checks:
    # raise the ceiling and require that the candidate pool is non-decreasing
    # and that whatever sits above the default ceiling is REPORTED, not silent.
    wide = argparse.Namespace(**vars(args))
    wide.block_max = max(args.block_max * 2, 512)
    wide.top_blocks = 10 ** 6
    wide_offered = blocks(load_index(), wide)[1]
    base_offered = blocks(load_index(), argparse.Namespace(
        **{**vars(args), "top_blocks": 10 ** 6}))[1]
    good = len(wide_offered) >= len(base_offered)
    print("[self-test] raising --block-max %d -> %d never LOSES candidates: %s"
          % (args.block_max, wide.block_max,
             "PASS (%d -> %d)" % (len(base_offered), len(wide_offered))
             if good else
             "FAIL (%d -> %d; the ceiling filter is broken)"
             % (len(base_offered), len(wide_offered))))
    ok &= good

    # Wave 38: the LOOP filter, checked the same way as the ceiling and the two
    # floors. It hid 532 in-band functions for 22 waves. Lifting it must never
    # lose candidates -- if it does, --allow-loops is filtering rather than
    # admitting, which is how a "dry" axis gets manufactured.
    loose = argparse.Namespace(**{**vars(args), "top_blocks": 10 ** 6})
    loose.allow_loops = True
    loop_offered = blocks(load_index(), loose)[1]
    good = len(loop_offered) >= len(base_offered)
    print("[self-test] --allow-loops never LOSES candidates: %s"
          % ("PASS (%d -> %d)" % (len(base_offered), len(loop_offered))
             if good else
             "FAIL (%d -> %d; the loop filter is inverted)"
             % (len(base_offered), len(loop_offered))))
    ok &= good

    # Wave 38: the callee-normalised tier must be able to SUBSTANTIATE its own
    # claim. Rather than hardcode a pair (which rots the moment it is matched),
    # re-derive every group it reports and require that the members' instruction
    # streams differ ONLY on `bl` lines. A tier that groups functions differing
    # anywhere else is lying, and the whole value of a duplicate group is that
    # one derivation gives every member.
    bodies = asm_bodies()
    _, _, by_callee = duplicates(load_index(), bodies)
    bad = []
    for g in by_callee.values():
        streams = [bodies[r["name"]][0] for r in g if r["name"] in bodies]
        if len(streams) < 2:
            continue
        base = streams[0]
        for other in streams[1:]:
            if len(base) != len(other):
                bad.append(g)
                break
            for x, y in zip(base, other):
                if x != y and not (BL_TARGET.match(x) and BL_TARGET.match(y)):
                    bad.append(g)
                    break
            else:
                continue
            break
    good = not bad
    print("[self-test] callee-normalised groups differ ONLY at `bl` targets: %s"
          % ("PASS (%d group(s) checked)" % len(by_callee) if good else
             "FAIL (%d group(s) differ elsewhere, e.g. %s)"
             % (len(bad), " ".join(r["name"] for r in bad[0]))))
    ok &= good

    print("[self-test] %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--min-size", type=int, default=24,
                   help="smallest target for shape screens (24 is the measured "
                        "post-wave-60 floor; delta screening ignores it)")
    p.add_argument("--min-shared", type=int, default=4)
    p.add_argument("--min-j", type=float, default=0.6)
    p.add_argument("--top", type=int, default=15)
    p.add_argument("--max-fanin", type=int, default=40,
                   help="drop callees called by more than this many functions "
                        "(0 disables the IDF cut)")
    p.add_argument("--near-miss", type=float, default=95.0,
                   help="floor for the work/<fn>/best.json near-miss sweep")
    p.add_argument("--pending", action="store_true",
                   help="also count drafts trymatch has matched but that are "
                        "not promoted yet -- use this DURING a wave, because "
                        "data/functions.json does not learn about a match "
                        "until promotion")
    p.add_argument("--block-min", type=int, default=12,
                   help="smallest function the address-locality screen offers")
    p.add_argument("--block-max", type=int, default=256,
                   help="largest function the address-locality screen offers. "
                        "Was 72 until wave 33, when the default hid 565 "
                        "candidates (80KB) behind '2 blocks, 3 candidates' -- "
                        "the third floor artifact in this tool and the first "
                        "on an UPPER bound. 76-256B functions in a half-"
                        "promoted block are still exemplar-cheap (wave 33 "
                        "measured 115 bytes/attempt); they just need batches "
                        "sized by BYTES, ~2-5KB per agent, not by count")
    p.add_argument("--block-min-matched", type=int, default=1,
                   help="skip blocks with fewer matched functions than this -- "
                        "the axis is only cheap where the vocabulary is ALREADY "
                        "written down, and a cold block behaves nothing like it")
    p.add_argument("--top-blocks", type=int, default=12)
    p.add_argument("--per-block", type=int, default=14,
                   help="targets listed per block; ~13 is one agent's batch")
    p.add_argument("--no-blocks", action="store_true",
                   help="skip the address-locality screen")
    loops = p.add_mutually_exclusive_group()
    loops.add_argument("--allow-loops", dest="allow_loops",
                       action="store_true", default=True,
                   help="also offer functions with backward branches. Every "
                        "band screen since wave 16 hard-filtered "
                        "backward_branches == 0 on the theory that a loop is "
                        "one COUPLED decision. WAVE 37 REFUTED THAT: seven "
                        "agents measured 39 of 41 loop functions matched at "
                        "62-166 bytes/attempt, at or above the straight-line "
                        "rate, with every miss pointing at itself. 869 clean "
                        "loop functions / 227,912 bytes sit behind this filter "
                        "against 244 / 41,078 in front of it -- the fourth "
                        "screen artifact in this tool and by far the largest; "
                        "this is now the default")
    loops.add_argument("--no-loops", dest="allow_loops", action="store_false",
                       help="restore the historical straight-line-only screen")
    p.add_argument("--no-delta", action="store_true",
                   help="skip the wave-59 residual-delta screen")
    p.add_argument("--delta-only", action="store_true",
                   help="run ONLY the residual-delta screen (implies "
                        "--no-blocks and skips the shape axes' output)")
    p.add_argument("--delta-small", type=int, default=16,
                   help="|candidate code bytes - ROM code bytes| at or below "
                        "this is a `small` residual; above it is `large`. 16 "
                        "is EIGHT THUMB instructions -- past that a residual "
                        "stops being an allocation or induction-variable "
                        "difference and starts being a shape. Nothing is "
                        "hidden by this threshold: it splits the listing, it "
                        "does not filter it")
    p.add_argument("--delta-parked-floor", type=float, default=93.0,
                   help="a SIZE-EXACT draft at or above this percentage that "
                        "has already been parked once sinks to the bottom of "
                        "its group. Wave 58: all three size-exact drafts at "
                        "93%%+ were instruction-ORDER decisions and not one "
                        "moved, while both matches came from 87.9%% and 66.9%%")
    p.add_argument("--delta-top", type=int, default=40,
                   help="rows listed by the delta screen")
    p.add_argument("--delta-loops-only", action="store_true",
                   help="list only loop-carrying drafts. The counts still "
                        "cover both pools")
    p.add_argument("--self-test", action="store_true")
    p.add_argument("--include-parked", action="store_true",
                   help="also screen parked functions as targets; implied by "
                        "--self-test, whose anchors are parked")
    args = p.parse_args()
    if args.delta_only:
        delta_report(load_index(), args)
        return
    if args.self_test:
        args.include_parked = True
        print("[self-test] --include-parked is ON (the anchors are parked), so "
              "every count below\n            is HIGHER than a plain run's. "
              "The two outputs are not comparable;\n            batch off the "
              "plain run.")
        sys.exit(self_test(args))
    report(args)


main()
