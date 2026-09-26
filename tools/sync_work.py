#!/usr/bin/env python3
"""Refresh stale work/<fn>/<fn>.c drafts from the promoted source.

Once a function is promoted, src/decomp/ is the truth and its work/ draft is
dead. The drafts do not stay harmless, though: most predate the shared headers
and still carry their own `extern`, so `trymatch` on one reports

    COMPILE FAILED ... conflicting types for `gUnknown_XXXXXXXX'

which reads as breakage and is not. That has now produced false alarms in three
separate sessions, and worse, it fires during the very check that is supposed
to prove a header widening safe -- `grep -rl <global> src/` names the promoted
files at risk, and running trymatch on them hits the stale draft rather than
the file you just edited.

So: run this after changing a shared type, then trymatch is meaningful again.

    python tools/sync_work.py                 # every promoted function
    python tools/sync_work.py sub_08016E74    # just these
    python tools/sync_work.py --dry-run       # count stale drafts, write nothing

RUN IT AFTER EVERY HEADER RENAME OR MERGE (wave 90). PR #3 renamed globals and
struct tags in src/decomp/ but not in the promoted functions' work/ drafts, so
trymatch's own --self-test failed on three promoted functions (AgbMain's unit,
sub_0802C604, sub_0802CDA4) with `undeclared` errors, and every regression
sweep over promoted units read as broken.

Multi-function files are handled too. That used to be impossible here -- the
claim was that a promoted file could not be split back without re-deriving
where each function starts -- but promote.py already locates exactly those
boundaries on the way in, so this just runs its splitter backwards. It matters
more than it sounds: a third of promoted functions live in multi-function
files, so leaving them out meant the check that is supposed to prove a header
change safe silently skipped a third of the corpus.

Hand-written files in src/*.c are covered as well as src/decomp/, because
both are listed in data/promoted.json (wave 90: 12 units, 318 functions, among
them AgbMain's in src/main.c). PR #3 renamed functions there and in src/decomp/.
A renamed function is found by its `.thumb_set sub_XXXXXXXX, NewName` line,
or by a `#define NewName sub_XXXXXXXX` macro in include/ or src/, and its
draft defines it under the new name, with that line kept. Before this,
any file holding a renamed function was reported as "could not be located"
and all of its drafts were left stale.

`make SPLIT=1 compare` is still the check that cannot be fooled, and is worth
running after any shared-type change regardless.
"""

import json
import os
import re
import sys

import awlib
import promote

PROMOTED = os.path.join(awlib.DATA_DIR, "promoted.json")

# The generated banner is dropped from every draft. promote.merge() writes a
# fresh one, so a copy kept here would end up in src/ twice.
BANNER = promote.BANNER


def _is_definition(lines, i):
    """Does the declarator starting at line `i` open a DEFINITION?

    Wave 24: matching `^\\S.*\\b<fn>\\s*\\(` on the first code line is not
    enough, because a promoted multi-function file routinely FORWARD-DECLARES
    its own functions above the definitions. In c_080824D4.c the prototype
    `void sub_08082660(struct Unk8082660 *);` sits at line 58 and the
    definition at line 231, so the boundary landed on the prototype and the
    draft came out as the doc comment plus a struct with NO BODY -- which
    `trymatch` then reports as `candidate is 0 bytes, original is 848`, i.e. a
    catastrophic-looking regression on a function that is perfectly fine. This
    is the same class as the comment-opener bug above and the third instance of
    it; the discriminator is what TERMINATES the declarator, so read forward to
    the first `{` or `;` rather than trusting the opening line's shape.
    """
    in_comment, text = False, []
    for ln in lines[i:i + 40]:
        codetext, in_comment = promote.strip_comments(ln, in_comment)
        text.append(codetext)
    s = "".join(text)

    # Walk to the declarator's MATCHING close paren. Scanning for the first
    # `{` or `;` anywhere is wrong: this tree contains K&R definitions --
    #     void sub_0803B524(a)
    #     int a;
    #     {
    # -- where a `;` legitimately precedes the body, and treating it as a
    # prototype loses the function. That mistake made c_0803B4EC.c unsplittable
    # the first time this guard was written.
    depth = 0
    for k, ch in enumerate(s):
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                rest = s[k + 1:].lstrip()
                # `;` right after the declarator is a prototype. `{` is an
                # ANSI definition. Anything else is a K&R parameter
                # declaration list, i.e. still a definition.
                return not rest.startswith(";")
    return False


def split_unit(text, fns):
    """(shared declarations, {fn: body}) for one promoted file.

    Boundaries come from promote.doc_comment_start, the same function that put
    them there, so a comment written above a definition travels with it rather
    than being stranded on the previous function.
    """
    lines = text.splitlines(keepends=True)

    # Which lines are code. `^\S.*<fn>\s*\(` on its own also matches the FIRST
    # line of a block comment, because that line starts with `/`, and a doc
    # comment routinely names the function the one below it calls. In
    # c_0803B3C8.c the comment above sub_0803B3C8 opens
    # "/* `sub_0803B3D4(8)`: ..." and hijacked sub_0803B3D4's boundary, which
    # collapsed one draft to nothing and put two functions in the other -- and
    # `trymatch` then reports both as +12 bytes, i.e. exactly the false
    # regression this tool exists to prevent. Continuation lines were already
    # safe (they start with a space), so only the opener needed excluding; the
    # flag is tracked properly anyway because a `*/`-then-code line is legal.
    code = []
    in_comment = False
    for ln in lines:
        s = ln.lstrip()
        code.append(not in_comment and not s.startswith(("/*", "//", "*")))
        _, in_comment = promote.strip_comments(ln, in_comment)

    # A function renamed in C is defined under its new name, with
    # `asm(".thumb_set sub_XXXXXXXX, NewName")` after it or a header macro
    # `#define NewName sub_XXXXXXXX` (wave 90); promote.definition_names.
    starts = {}
    for fn in fns:
        hit = None
        for name in promote.definition_names(fn, text):
            pat = re.compile(r'^\S.*\b%s\s*\(' % re.escape(name))
            hit = next((i for i, ln in enumerate(lines)
                        if code[i] and pat.match(ln) and _is_definition(lines, i)),
                       None)
            if hit is not None:
                break
        if hit is None:
            return None, None
        starts[fn] = promote.doc_comment_start(lines, hit)

    order = sorted(starts, key=lambda f: starts[f])
    head = BANNER.sub("", "".join(lines[:starts[order[0]]]))
    bodies = {}
    for k, fn in enumerate(order):
        end = starts[order[k + 1]] if k + 1 < len(order) else len(lines)
        bodies[fn] = "".join(lines[starts[fn]:end]).rstrip() + "\n"
    return head, bodies


def main(argv):
    dry = "--dry-run" in argv
    wanted = set(a for a in argv[1:] if a != "--dry-run")
    with open(PROMOTED, encoding="utf-8") as fh:
        units = json.load(fh)

    synced, skipped, missing = [], [], []
    for unit in units:
        fns = unit["functions"]
        if wanted and not wanted & set(fns):
            continue
        src = os.path.join(awlib.REPO, unit["file"])
        if not os.path.isfile(src):
            missing.append(unit["file"])
            continue

        text = open(src, encoding="utf-8", newline="").read()
        if len(fns) == 1:
            drafts = {fns[0]: BANNER.sub("", text)}
        else:
            head, bodies = split_unit(text, fns)
            if head is None:
                skipped.append((unit["file"], len(fns)))
                continue
            drafts = {fn: head + body for fn, body in bodies.items()}

        for fn, body in drafts.items():
            if wanted and fn not in wanted:
                continue
            dst_dir = os.path.join(awlib.REPO, "work", fn)
            dst = os.path.join(dst_dir, fn + ".c")
            if os.path.isfile(dst) and open(dst, encoding="utf-8",
                                            newline="").read() == body:
                continue
            if not dry:
                os.makedirs(dst_dir, exist_ok=True)
                with open(dst, "w", encoding="utf-8", newline="\n") as fh:
                    fh.write(body)
            synced.append(fn)

    print("%s %d draft(s) from src/"
          % ("would sync (dry run, nothing written)" if dry else "synced", len(synced)))
    for fn in synced[:20]:
        print("  " + fn)
    if len(synced) > 20:
        print("  ... and %d more" % (len(synced) - 20))
    if skipped:
        print("\n%d file(s) whose function boundaries could not be located --"
              " for these the check is `make SPLIT=1 compare`:" % len(skipped))
        for path, n in skipped[:10]:
            print("  %s (%d functions)" % (path, n))
    if missing:
        print("\nmissing source file(s): %s" % ", ".join(missing))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
