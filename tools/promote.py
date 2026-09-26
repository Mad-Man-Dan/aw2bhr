#!/usr/bin/env python3
"""Promote matched C out of work/ and into the build.

A function only counts once the ROM is built from its C instead of its assembly.
This performs that swap and records it in data/promoted.json, which split_asm.py
and gen_lds.py read to route the address slot to the C object.

asm/ is deliberately left alone. Because the build already assembles per-function
units, promoting means "stop emitting this unit and link the C object at its
address" -- not "delete the original". The assembly stays as reference, the split
still reconstructs it byte-for-byte, and the upstream monolithic build keeps
working untouched, which is a second independent check that the ROM is right.

Two constraints the linker imposes, both enforced here:

  * A .c file occupies one contiguous address range, because the linker script
    places its .text as a single unit. Functions are therefore grouped into runs
    of adjacent addresses, one file per run.
  * A promoted run cannot share a split unit with a function that is still
    assembly, since the unit is emitted or not as a whole.

Nothing is promoted unless tools/trymatch.py currently says it matches.

A draft may define its function under a new name (wave 90): through a
`.thumb_set sub_XXXXXXXX, NewName` line or a `#define NewName sub_XXXXXXXX`
header macro. split_source() finds it by that name, so merge() and
`trymatch --unit` accept such drafts.

    python tools/promote.py sub_08013AEC sub_08013AFC
    python tools/promote.py --all-matched
"""

import argparse
import json
import os
import re
import subprocess
import sys

import awlib
from comment_check import strip_c

WORK = os.path.join(awlib.REPO, "work")
DECOMP_DIR = os.path.join(awlib.REPO, "src", "decomp")
PROMOTED = os.path.join(awlib.DATA_DIR, "promoted.json")
INCLUDE_LINE = '#include "global.h"\n'
# The comment merge() writes at the top of every promoted file. A draft that
# was copied back from src/ (sync_work.py) carries it too, and merge() must
# drop that copy or each sync -> promote cycle adds another one.
BANNER = re.compile(r'/\* Promoted from assembly;.*?\*/[ \t]*(?:\r?\n)*', re.S)


def load_index():
    path = os.path.join(awlib.DATA_DIR, "functions.json")
    with open(path, encoding="utf-8") as fh:
        return {r["name"]: r for r in json.load(fh)}


def load_promoted():
    if not os.path.exists(PROMOTED):
        return []
    with open(PROMOTED, encoding="utf-8") as fh:
        return json.load(fh)


# name -> pool words its match depends on, filled by verify().
POOL_WORDS = {}


def verify(name):
    """True only if trymatch currently reports a byte-for-byte match.

    Also returns any `-fforce-addr` pool words the match DEPENDS ON being
    placed. trymatch prints them because a promotion that omits them either
    drops the word or shifts every address after it -- and that failure would
    surface in the split build, not here, with the function already moved.
    Parsing them out means the caller cannot forget.
    """
    proc = subprocess.run(
        [sys.executable, os.path.join(awlib.REPO, "tools", "trymatch.py"), name],
        cwd=awlib.REPO, capture_output=True, text=True, timeout=600,
        stdin=subprocess.DEVNULL)
    words = []
    for ln in proc.stdout.splitlines():
        m = re.search(r'"rodata": \[(.+?)\]', ln)
        if m:
            words = re.findall(r'0x[0-9A-Fa-f]{8}', m.group(1))
    if words:
        POOL_WORDS[name] = words
    return proc.returncode == 0, proc.stdout.strip().splitlines()[-1:], words


def unit_of(name):
    """(unit record, its function list) from the split manifest, or (None, [])."""
    path = os.path.join(awlib.REPO, "build", "functions", "units.json")
    if not os.path.exists(path):
        return None, []
    with open(path, encoding="utf-8") as fh:
        for u in json.load(fh):
            if name in u.get("functions", []):
                return u, list(u["functions"])
    return None, []


def verify_unit(name):
    """True only if the WHOLE unit's .text reproduces the ROM.

    Strictly stronger evidence than every function matching separately -- it is
    what the split build links -- so it is a legitimate basis for promotion.
    This is not a way around the gate; it is the gate applied to the artefact
    that actually gets built.

    It exists because a per-function oracle cannot verify a function that calls
    a `static` helper defined beside it: compiled alone the call is external and
    carries a relocation, while the original resolved it locally and carries
    none. Wave 27's AgbMain differed by exactly those 2 bytes in 252, with seven
    already-matching siblings stuck behind it because a unit promotes whole or
    not at all.
    """
    proc = subprocess.run(
        [sys.executable, os.path.join(awlib.REPO, "tools", "trymatch.py"),
         name, "--unit"],
        cwd=awlib.REPO, capture_output=True, text=True, timeout=900,
        stdin=subprocess.DEVNULL)
    words = []
    for ln in proc.stdout.splitlines():
        m = re.search(r'"rodata": \[(.+?)\]', ln)
        if m:
            words = re.findall(r'0x[0-9A-Fa-f]{8}', m.group(1))
    return proc.returncode == 0, proc.stdout.strip().splitlines()[-3:], words


def refresh_promoted(names, index, existing):
    """Re-emit promoted units whose drafts no longer match what was promoted.

    `promote.py` used to print "nothing to promote (all already promoted)" and
    stop, leaving src/decomp/ holding the OLD text -- while its own messages
    call the drafts the source of truth. That silence is dangerous rather than
    merely untidy: src/decomp/ is what the build compiles, so a post-promotion
    fix to a draft (a call-site cast after a prototype was settled, say) looked
    applied, verified clean per-function, and would have failed the split build.
    Wave 27 hit exactly that and caught it by hand.

    A refresh re-verifies the whole unit before rewriting, so a drifted draft
    cannot smuggle in unverified code.

    Only a CODE difference counts (comment_check.strip_c: comments removed,
    whitespace collapsed). Comments in src/decomp/ are edited in place and
    copied to the drafts by sync_work.py, and about 700 files have had their
    layout tidied by hand since promotion; a text comparison would rewrite
    all of them in merge()'s layout.
    """
    by_fn = {n: e for e in existing for n in e["functions"]}
    done, refreshed = set(), []
    for name in names:
        entry = by_fn.get(name)
        if entry is None or entry["file"] in done:
            continue
        done.add(entry["file"])
        run = entry["functions"]
        # Only generated files are rebuilt from drafts. promoted.json also
        # lists hand-written files (src/main.c, src/proc.c, ...); merge() would
        # rewrite those in its own layout, adding a banner and moving includes.
        if not entry["file"].startswith("src/decomp/"):
            print("  %s is hand-written, so it is not rebuilt from drafts; "
                  "edit it directly" % entry["file"])
            continue
        missing = [f for f in run if f not in index]
        if missing:
            print("  skipped %s: promoted.json lists %s, which is not in "
                  "data/functions.json" % (entry["file"], ", ".join(missing)))
            continue
        if any(not os.path.exists(os.path.join(WORK, f, f + ".c")) for f in run):
            continue
        text, err = merge(run, index)
        if err:
            continue
        path = os.path.join(awlib.REPO, entry["file"].replace("/", os.sep))
        try:
            current = open(path, encoding="utf-8", newline="").read()
        except OSError:
            current = None
        if current is not None and strip_c(current) == strip_c(text):
            continue
        ok, tail, _ = verify_unit(run[0])
        if not ok:
            print("  DRIFT in %s -- the draft(s) differ from the promoted file,"
                  % entry["file"])
            print("  but the unit does NOT verify, so it was left alone:")
            for ln in tail:
                print("      %s" % ln)
            continue
        # merge() writes LF; about 90 files in src/decomp/ have CRLF lines.
        # Keep each unchanged line's ending so the diff shows the real change.
        if current is not None:
            import fix_eol
            fixed, _ = fix_eol.churn(current.encode("utf-8"), text.encode("utf-8"))
            if fixed is not None:
                text = fixed.decode("utf-8")
        awlib.write_text(path, text)
        refreshed.append(entry["file"])
        print("  refreshed %s from its draft(s) -- unit re-verified"
              % entry["file"])
    return refreshed


# Wave 90: PR #3 renamed functions in C, in two ways. Old name first here:
#     asm(".global sub_08078E14\n.thumb_set sub_08078E14, GetCampaignMissionId\n");
# and new name first in a header macro:
#     #define BG_EnableSync sub_08013AD4
THUMB_SET = re.compile(r'\.thumb_set\s+(\w+)\s*,\s*(\w+)')
JOINED_STR = re.compile(r'"\s*\n\s*"')
DEFINE_ALIAS = re.compile(r'^\s*#\s*define\s+([A-Za-z_]\w*)\s+(sub_[0-9A-F]{8})\s*$',
                          re.M)


def macro_aliases():
    """old name -> new name for renames done with a macro in include/ or src/."""
    if macro_aliases.cache is None:
        out = {}
        for top in ("include", "src"):
            for root, _, files in os.walk(os.path.join(awlib.REPO, top)):
                for f in files:
                    if f.endswith((".h", ".c")):
                        with open(os.path.join(root, f), encoding="utf-8",
                                  errors="replace") as fh:
                            for new, old in DEFINE_ALIAS.findall(fh.read()):
                                out.setdefault(old, new)
        macro_aliases.cache = out
    return macro_aliases.cache


macro_aliases.cache = None


def definition_names(name, text):
    """Every name `name` may be DEFINED under in `text`: itself first, then a
    `.thumb_set name, New` alias in the text, then a header macro. A string
    literal broken across lines is joined first (src/design.c splits one)."""
    names = [name]
    for old, new in THUMB_SET.findall(JOINED_STR.sub("", text)):
        if old == name and new not in names:
            names.append(new)
    new = macro_aliases().get(name)
    if new and new not in names:
        names.append(new)
    return names


def split_source(name, text):
    """(declarations, body) for one work file.

    The body starts at the function's definition line; everything above it is
    includes, externs and struct definitions, which are merged and de-duplicated
    across the files sharing a translation unit.

    A function renamed in C (see definition_names) is found by its new name
    when its old one is not there. Only a DEFINITION counts for the new name,
    because a header section may declare it first.
    """
    lines = text.splitlines(keepends=True)
    for k, nm in enumerate(definition_names(name, text)):
        pat = re.compile(r'^\S.*\b%s\s*\(' % re.escape(nm))
        for i, ln in enumerate(lines):
            if pat.match(ln):
                if k and ln.split("//")[0].rstrip().endswith(";"):
                    continue    # a prototype of the new name
                j = doc_comment_start(lines, i)
                return lines[:j], lines[j:]
    return None, None


def doc_comment_start(lines, i):
    """Where the comment block documenting the definition at `lines[i]` begins.

    A comment written directly above a function is that function's
    documentation and has to travel with it into the body. Cutting at the
    definition line alone strands it in the declaration section, where it comes
    out above whichever declaration happens to merge first -- so `c_08063F98.c`
    ended up explaining how to copy a matrix directly above the function that
    loads the identity.
    """
    j = i
    while j > 0 and not lines[j - 1].strip():
        j -= 1
    if j == 0 or not lines[j - 1].strip().endswith("*/"):
        return i
    k = j - 1
    while k >= 0 and "/*" not in lines[k]:
        k -= 1
    return k if k >= 0 else i


def strip_comments(ln, in_comment):
    """(code with comments blanked out, whether a block comment is still open).

    Only the returned code is inspected for braces and terminators; the original
    line is what gets emitted. Every test below has to run on comment-free text,
    because comment prose is arbitrary and routinely contains `;`, `{` and `}`.
    """
    out, i, n = [], 0, len(ln)
    while i < n:
        if in_comment:
            end = ln.find("*/", i)
            if end < 0:
                break
            i, in_comment = end + 2, False
            continue
        start = ln.find("/*", i)
        line_c = ln.find("//", i)
        if line_c >= 0 and (start < 0 or line_c < start):
            out.append(ln[i:line_c])
            break
        if start < 0:
            out.append(ln[i:])
            break
        out.append(ln[i:start])
        i, in_comment = start + 2, True
    return "".join(out), in_comment


def decl_chunks(lines):
    """Split a declaration section into whole declarations.

    A chunk is one brace-balanced construct: a `struct X { ... };` block, a
    single `extern ...;` line, or one preprocessor directive -- together with any
    comment block sitting directly above it, which is the documentation for that
    declaration and has to travel with it. De-duplicating at line level instead
    silently destroys source -- two different structs both contain a line `{` and
    a line `};`, so dropping the repeats leaves the second one without its braces.

    Comment state is tracked explicitly rather than sniffed from the leading
    characters of each line. Two bugs came from not doing that, and they
    compounded: a comment whose prose ended in `;` closed the chunk mid-comment,
    and the continuation lines that followed were then discarded as stray
    comments because nothing was accumulated any more. `c_0806978C.c` was
    promoted with an unterminated comment swallowing the struct beneath it, and
    the agent's C was fine -- the draft in work/ compiled and matched. A
    directive was never a terminator either, so `#include "proc.h"` could not
    close its own chunk and glued itself onto the next declaration.
    """
    chunks, cur, depth = [], [], 0
    in_comment = False
    for ln in lines:
        raw = ln.strip()
        code, next_in_comment = strip_comments(ln, in_comment)
        s = code.strip()
        if not cur and not in_comment:
            if not raw or raw == INCLUDE_LINE.strip():
                continue
        cur.append(ln)
        depth += code.count("{") - code.count("}")
        was_open, in_comment = in_comment, next_in_comment
        if in_comment:
            continue
        if s.startswith("#") and depth <= 0:
            chunks.append("".join(cur))        # a directive is complete on its line
            cur, depth = [], 0
            continue
        if not s and (was_open or raw.startswith("/*")):
            continue                          # comment block; the decl follows
        if depth <= 0 and (s.endswith(";") or s.endswith("}")):
            chunks.append("".join(cur))
            cur, depth = [], 0
    if cur:
        chunks.append("".join(cur))
    return chunks


def chunk_key(chunk):
    """Identity of a declaration, ignoring its comments and whitespace.

    Two agents will document the same struct differently, and if the comment is
    part of the key those collapse to two definitions of one type -- which is a
    hard `redefinition of struct X` at compile time, not a cosmetic problem. The
    text kept is still the first occurrence's, comment included.
    """
    code, in_comment = [], False
    for ln in chunk.splitlines(keepends=True):
        part, in_comment = strip_comments(ln, in_comment)
        code.append(part)
    key = " ".join("".join(code).split())
    return key or " ".join(chunk.split())     # comment-only chunk: compare as-is


def defined_types(chunk):
    """{('struct'|'union'|'enum', name)} this chunk DEFINES, not merely names.

    A forward declaration (`struct Unk80824D4;`) must not count: these files
    routinely forward-declare every type at the top and define them further
    down, and both are legal in one translation unit. Only a definition -- a
    tag followed by a brace body -- can collide.

    ONE regex, deliberately. `\\s*` spans newlines, so a single pattern covers
    both `struct Foo {` and `struct Foo` with the brace on the next line. Using
    two patterns double-counts every struct written in the second style; that
    mistake reported 218 false positives when this check was first prototyped
    as a standalone scan.
    """
    text, in_comment, out = [], False, set()
    for ln in chunk.splitlines(keepends=True):
        part, in_comment = strip_comments(ln, in_comment)
        text.append(part)
    for kind, tag in re.findall(
            r'\b(struct|union|enum)\s+([A-Za-z_]\w*)\s*\{', "".join(text)):
        out.add((kind, tag))
    return out


def merge(run, index):
    """One .c source for a contiguous run of functions."""
    decls, bodies = [], []
    seen = set()
    # type definition -> (chunk_key, the draft that contributed it). Two drafts
    # defining the SAME tag with DIFFERENT bodies is not a formatting
    # difference that chunk_key can normalise away -- it is two agents
    # disagreeing about a type model, and it is invisible to every
    # per-function check because both drafts compile alone. Wave 24 shipped
    # c_08078250.c with two `struct Unk807831C`, one carrying
    # `filler_00[0x58]` and the other `filler_00[0x54]; void *unk_54`. Same
    # layout, both drafts byte-exact, split build dead on `redefinition`.
    types = {}
    for name in run:
        path = os.path.join(WORK, name, name + ".c")
        text = "".join(awlib.read_lines(path))
        head, body = split_source(name, text)
        if head is None:
            return None, "could not locate the definition of %s in %s" % (
                name, os.path.relpath(path, awlib.REPO))
        head = BANNER.sub("", "".join(head)).splitlines(keepends=True)
        for chunk in decl_chunks(head):
            key = chunk_key(chunk)
            for t in defined_types(chunk):
                prev_key, prev_name = types.get(t, (key, name))
                if prev_key != key:
                    return None, (
                        "%s %s is defined DIFFERENTLY by two drafts in this "
                        "unit:\n"
                        "    work/%s/%s.c\n"
                        "    work/%s/%s.c\n"
                        "  This is not a formatting difference -- the bodies "
                        "disagree, so it is two\n"
                        "  type models for one tag. Both drafts compile and "
                        "byte-match ALONE, which is\n"
                        "  why trymatch cannot see it; merged into one unit it "
                        "is a hard\n"
                        "  `redefinition of %s %s`.\n"
                        "  Reconcile by hand: keep the MORE REFINED body (the "
                        "one naming more fields),\n"
                        "  put it in BOTH drafts, re-run trymatch on every "
                        "function in the unit, then\n"
                        "  promote again. Do not just delete one copy here -- "
                        "the drafts are the\n"
                        "  source of truth and sync_work.py will reintroduce "
                        "it."
                        % (t[0], t[1], prev_name, prev_name, name, name,
                           t[0], t[1]))
                types.setdefault(t, (key, name))
            if key not in seen:
                seen.add(key)
                decls.append(chunk)
        bodies.append("".join(body).rstrip() + "\n")

    addrs = ", ".join("%s @ %s" % (n, index[n]["addr_hex"]) for n in run)
    header = (INCLUDE_LINE + "\n"
              "/* Promoted from assembly; each function below is byte-for-byte\n"
              " * identical to the original. Order is address order and must\n"
              " * stay that way -- the linker places this file's .text as one\n"
              " * contiguous block at %s.\n"
              " * %s\n */\n\n" % (index[run[0]]["addr_hex"], addrs))
    # New files are written LF. Some drafts mix in CRLF lines, which would
    # make the new file mixed too.
    text = header + "".join(decls) + ("\n" if decls else "") + "\n".join(bodies)
    return text.replace("\r\n", "\n"), None


def contiguous_runs(names, index):
    """Split names into runs where each function abuts the next in memory."""
    ordered = sorted(names, key=lambda n: index[n]["addr"])
    runs, cur = [], []
    for n in ordered:
        if cur:
            prev = index[cur[-1]]
            if prev["addr"] + prev["size"] != index[n]["addr"]:
                runs.append(cur)
                cur = []
        cur.append(n)
    if cur:
        runs.append(cur)
    return runs


def all_matched(index):
    """Every function with a candidate in work/ that currently matches."""
    if not os.path.isdir(WORK):
        return []
    already = {n for e in load_promoted() for n in e["functions"]}
    out = []
    for name in sorted(os.listdir(WORK)):
        if name not in index or name in already:
            continue          # checking a promoted function again is just noise
        if not os.path.exists(os.path.join(WORK, name, name + ".c")):
            continue
        ok, _, _ = verify(name)
        print("  %-22s %s" % (name, "match" if ok else "does not match, skipped"))
        if ok:
            out.append(name)
    return out


def promote(names, index):
    existing = load_promoted()
    already = {n for e in existing for n in e["functions"]}
    stale = [n for n in names if n in already]
    names = [n for n in names if n not in already]
    refreshed = refresh_promoted(stale, index, existing) if stale else []
    if not names:
        if refreshed:
            print("refreshed %d already-promoted file(s) from their drafts"
                  % len(refreshed))
            print("REBUILD -- src/decomp/ changed. See the commands below.")
        else:
            print("nothing to promote (all already promoted, no draft drift)")
            return 0

    os.makedirs(DECOMP_DIR, exist_ok=True)
    added = []
    for run in contiguous_runs(names, index) if names else []:
        text, err = merge(run, index)
        if err:
            print("error: %s" % err)
            return 1
        stem = "c_%s" % index[run[0]]["addr_hex"][2:]
        rel = "src/decomp/%s.c" % stem
        awlib.write_text(os.path.join(awlib.REPO, rel), text)
        entry = {
            "file": rel,
            "obj": "src/decomp/%s.o" % stem,
            "addr_hex": index[run[0]]["addr_hex"],
            "functions": run,
            "size": sum(index[n]["size"] for n in run),
        }
        # Pool words this unit now emits itself. tools/split_rodata.py carves
        # them out of data/rodata.s or data/data.s so the C's copy is the only
        # one; without this the bytes would be in the ROM twice.
        words = sorted({w for n in run for w in POOL_WORDS.get(n, [])},
                       key=lambda w: int(w, 16))
        if words:
            entry["rodata"] = words
            print("      .rodata pool word(s): %s" % ", ".join(words))
        existing.append(entry)
        added.append(entry)
        print("  %s  <-  %s (%d bytes)"
              % (rel, ", ".join(run), entry["size"]))

    existing.sort(key=lambda e: int(e["addr_hex"], 16))
    awlib.write_text(PROMOTED, json.dumps(existing, indent=1) + "\n")

    n_fn = sum(len(e["functions"]) for e in added)
    print("\npromoted %d function(s) into %d file(s)" % (n_fn, len(added)))
    # INTEGRATION CHECKS, run here rather than merely suggested. Both failures
    # below pass every per-function `trymatch` and only surface when the split
    # build links everything, so the moment just after promotion is the last
    # point where they are cheap. Wave 24 promoted 109 verified functions and
    # then spent four build cycles on exactly these two. Naming a tool in a
    # hint is not enough -- split_rodata.py was named here and skipped anyway.
    print("\nintegration checks (tools/proto_check.py):")
    try:
        import proto_check
        rc = proto_check.main([])
    except Exception as exc:                       # never block a promotion
        print("  could not run proto_check.py: %s" % exc)
        rc = 0

    # Line-ending churn, reported HERE because this is the last stop before the
    # build and the commit. Several repo files are mixed CRLF/LF in git, and an
    # agent that rewrites one wholesale re-terminates every line: wave 32 turned
    # a 198-line addition into 921/724 four separate times, and wave 30 turned a
    # five-entry parked.json change into 425/379. The content is fine; the diff
    # is unreviewable, which is how a real finding gets missed.
    print("\nline-ending churn (tools/fix_eol.py):")
    try:
        import fix_eol
        if fix_eol.main.__code__.co_argcount == 0:
            saved = sys.argv
            sys.argv = ["fix_eol.py", "--check"]
            try:
                if fix_eol.main():
                    print("  ^ run `python tools/fix_eol.py` -- it restores each")
                    print("    unchanged line's original ending and touches no "
                          "content.")
            finally:
                sys.argv = saved
    except Exception as exc:
        print("  could not run fix_eol.py: %s" % exc)
    if rc:
        print("\n  *** FIX THESE BEFORE BUILDING. ***")
        print("  A prototype/definition conflict or a duplicated type in a")
        print("  merged unit will fail `make SPLIT=1 compare` and the error")
        print("  points at the file that INCLUDES the problem, not the one")
        print("  that caused it.")

    print("\nnow rebuild and prove the ROM is unchanged:")
    print("  python tools/split_asm.py && python tools/split_rodata.py && "
          "python tools/gen_lds.py")
    print("  make SPLIT=1 compare   # then rm -f aw2bhr.gba aw2bhr.elf, then:")
    print("  make compare           # both must print 'aw2bhr.gba: OK'")
    print("  READ EACH BUILD'S STATUS SEPARATELY. `make ... | tail` makes $?")
    print("  tail's, so a FAILED split build followed by a passing assembly")
    print("  build displays as a green 'aw2bhr.gba: OK'. Use set -o pipefail.")
    # split_rodata.py was missing from this hint for three waves. Omitting it
    # links a ROM whose every differing word is a pool reference pointing at
    # 0x08800000+N -- the end of the image -- because the carved .rodata pieces
    # never got addresses. Wave 20 bisected that as a bad promotion; wave 21
    # hit it again. The `rm -f` between the two builds is load-bearing too:
    # both targets write the same aw2bhr.gba, so `make compare` will otherwise
    # re-checksum the ROM the SPLIT build produced.
    print("  make SPLIT=1 compare")
    return 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("names", nargs="*")
    ap.add_argument("--all-matched", action="store_true",
                    help="promote every function in work/ that currently matches")
    args = ap.parse_args()

    index = load_index()
    if args.all_matched:
        print("checking candidates in work/ ...")
        names = all_matched(index)
        if not names:
            print("no matching candidates found")
            return 1
    else:
        names = args.names
        if not names:
            ap.error("give function names or --all-matched")
        for n in names:
            if n not in index:
                print("error: %s is not in the index" % n)
                return 1
            ok, tail, _ = verify(n)
            if ok:
                continue
            # The per-function oracle can be WRONG in one direction only: it
            # compiles one function per object, so it cannot reproduce a call
            # the original resolved inside its own unit. Before refusing, ask
            # the stronger question -- does the whole unit reproduce the ROM?
            unit, run = unit_of(n)
            uok, utail, uwords = (False, [], []) if not run else verify_unit(n)
            if not uok:
                print("refusing to promote %s -- it does not match" % n)
                for ln in tail:
                    print("  %s" % ln)
                if run:
                    print("  the whole unit does not reproduce the ROM either:")
                    for ln in utail:
                        print("      %s" % ln)
                return 1
            print("%s does not match ALONE, but its whole unit reproduces the"
                  " ROM." % n)
            # An asm-resident member has no draft and must never be promoted as
            # C -- but the unit verifying whole PROVES the compiler emits its
            # bytes, so it is carried by the promotion rather than part of it.
            # Appending it here would send promote() looking for a draft that
            # does not exist. (Wave 28: sub_0802C604's unit holds sub_0802C62A.)
            import index_functions as ixf
            resident = set(ixf.asm_resident())
            promotable = [f for f in run if f not in resident]
            carried = [f for f in run if f in resident]
            print("  Promoting all %d function(s) in %s together -- a unit is"
                  " linked whole," % (len(promotable), unit["file"]))
            print("  so that is the stronger evidence, not a weaker one.")
            if carried:
                print("  Carried, not promoted (asm-resident, bytes emitted by"
                      " the compiler): %s" % ", ".join(carried))
            if uwords:
                POOL_WORDS[n] = uwords
            for f in promotable:
                if f not in names:
                    names.append(f)
    return promote(names, index)


if __name__ == "__main__":
    sys.exit(main())
