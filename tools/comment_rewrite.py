#!/usr/bin/env python3
"""Rewrite the comments in a C file without touching its code.

Documentation passes (docs/writing-notes.md) rewrite thousands of comments.
Hand-editing each one risks editing code by accident, and costs one tool call
per comment. This moves the comments out to JSON and back instead:

    python tools/comment_rewrite.py extract include/unknown-globals.h \\
        --lines 1-1200 --out blocks.json
    # edit blocks.json: set each block's "new" to the replacement text,
    # "" to delete the comment, or leave "new" out to keep it unchanged
    python tools/comment_rewrite.py apply include/unknown-globals.h blocks.json

`extract` lists every comment that starts inside the line range, with the
code lines around it for context. Several files can go in one JSON; each
file's blocks are then under "files":

    python tools/comment_rewrite.py extract src/decomp/c_0800A000.c \\
        src/decomp/c_0800A0F0.c --out blocks.json
    python tools/comment_rewrite.py apply blocks.json

`apply` refuses to run if any file changed since extraction, replaces only
the comment text, and removes lines that a deleted comment leaves empty. Run
`python tools/comment_check.py <file>...` afterwards; it proves the code is
unchanged.
"""
import bisect
import hashlib
import json
import os
import sys

import awlib


def comment_spans(text):
    """[(start, end)] of every /* */ and // comment, skipping string and
    char literals."""
    spans, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if c == "/" and text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            spans.append((i, j))
            i = j
        elif c == "/" and text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            if j > i and text[j - 1] == "\r":
                j -= 1
            spans.append((i, j))
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            i = j + 1
        else:
            i += 1
    return spans


def read(path):
    with open(path, "rb") as fh:
        raw = fh.read()
    return raw, raw.decode("utf-8")


def _abs(path):
    return path if os.path.isabs(path) else os.path.join(awlib.REPO, path)


def uncommented_definitions(lines):
    """Line indexes of function definitions with no comment directly above.

    A definition here is the column-0 declarator line (it has a `(` and no
    `=` or `;`) above a line that is just `{`, the style every file in
    src/decomp uses.
    """
    out = []
    for i, ln in enumerate(lines):
        if ln.rstrip("\r") != "{":
            continue
        d = next((k for k in range(i - 1, max(-1, i - 6), -1)
                  if lines[k][:1].isalpha() or lines[k][:1] == "_"), None)
        if d is None:
            continue
        decl = " ".join(l.strip() for l in lines[d:i])
        if "(" not in decl or "=" in decl or decl.endswith(";"):
            continue
        j = d - 1
        while j >= 0 and not lines[j].strip():
            j -= 1
        prev = lines[j].rstrip("\r").rstrip() if j >= 0 else ""
        if prev.lstrip().startswith("//"):
            continue
        if prev.endswith("*/"):
            # The file's generated banner is not this function's comment.
            k = j
            while k > 0 and "/*" not in lines[k]:
                k -= 1
            if not lines[k].lstrip().startswith("/* Promoted from assembly;"):
                continue
        out.append(d)
    return out


def extract_doc(path, lines):
    raw, text = read(path)
    lo, hi = lines
    starts = [0]
    for k, ch in enumerate(text):
        if ch == "\n":
            starts.append(k + 1)
    line_of = lambda off: bisect.bisect_right(starts, off)
    all_lines = text.split("\n")
    blocks = []
    for start, end in comment_spans(text):
        ln = line_of(start)
        if not lo <= ln <= hi:
            continue
        end_ln = line_of(end - 1)
        before = [l.rstrip("\r") for l in all_lines[max(0, ln - 3):ln - 1]]
        after = [l.rstrip("\r") for l in all_lines[end_ln:end_ln + 4]]
        blocks.append({"id": len(blocks), "line": ln, "start": start, "end": end,
                       "before": before, "old": text[start:end].replace("\r\n", "\n"),
                       "after": after})
    # An empty slot above each function definition that has no comment right
    # above it, so a comment can be added where none was. "old" is "".
    for d in uncommented_definitions(all_lines):
        if not lo <= d + 1 <= hi:
            continue
        off = starts[d]
        blocks.append({"id": len(blocks), "line": d + 1, "start": off, "end": off,
                       "slot": "no comment above this function; set new to add one",
                       "before": [l.rstrip("\r") for l in all_lines[max(0, d - 2):d]],
                       "old": "",
                       "after": [l.rstrip("\r") for l in all_lines[d:d + 4]]})
    rel = os.path.relpath(path, awlib.REPO).replace("\\", "/")
    return {"file": rel, "sha1": hashlib.sha1(raw).hexdigest(),
            "lines": "%d-%d" % (lo, hi), "blocks": blocks}


def extract(paths, lines, out):
    docs = [extract_doc(p, lines) for p in paths]
    doc = docs[0] if len(docs) == 1 else {"files": docs}
    with open(out, "w", encoding="utf-8", newline="\n") as fh:
        json.dump(doc, fh, indent=1, ensure_ascii=False)
    print("extracted %d comment(s) from %d file(s) -> %s"
          % (sum(len(d["blocks"]) for d in docs), len(docs), out))
    return 0


def apply(path, blocks_path):
    doc = json.load(open(blocks_path, encoding="utf-8"))
    docs = doc["files"] if "files" in doc else [dict(doc, file=path or doc["file"])]
    # Check every file before writing any, so a refusal leaves nothing half done.
    for d in docs:
        raw, _ = read(_abs(d["file"]))
        if d["sha1"] != hashlib.sha1(raw).hexdigest():
            print("REFUSED: %s changed since extraction; extract again" % d["file"])
            return 2
    for d in docs:
        rc = apply_doc(_abs(d["file"]), d)
        if rc:
            return rc
    return 0


def apply_doc(path, doc):
    raw, text = read(path)
    changed = deleted = 0
    for b in sorted(doc["blocks"], key=lambda b: -b["start"]):
        if "new" not in b:
            continue
        s, e = b["start"], b["end"]
        if text[s:e].replace("\r\n", "\n") != b["old"]:
            print("REFUSED: %s block %d no longer matches its text" % (path, b["id"]))
            return 2
        # Some headers mix CRLF and LF lines, so take the ending of the line
        # the comment ends on, not one style for the whole file.
        eol = text.find("\n", e)
        nl = "\r\n" if eol > 0 and text[eol - 1] == "\r" else "\n"
        new = b["new"].replace("\r\n", "\n").replace("\n", nl)
        if b["old"] == "":
            if new:                     # a new comment in an empty slot
                text = text[:s] + new + nl + text[s:]
                changed += 1
            continue
        if new == "":
            # Take the whole line(s) when the comment stood alone on them.
            ls = text.rfind("\n", 0, s) + 1
            le = text.find("\n", e)
            le = len(text) if le < 0 else le + 1
            if text[ls:s].strip() == "" and text[e:le].strip() == "":
                s, e = ls, le
            deleted += 1
        else:
            changed += 1
        text = text[:s] + new + text[e:]
    with open(path, "wb") as fh:
        fh.write(text.encode("utf-8"))
    print("applied to %s: %d rewritten, %d deleted, %d kept"
          % (os.path.relpath(path, awlib.REPO).replace("\\", "/"), changed, deleted,
             len(doc["blocks"]) - changed - deleted))
    return 0


def main(argv):
    if len(argv) >= 3 and argv[1] == "extract":
        args, lines, out = argv[2:], (1, 10 ** 9), "blocks.json"
        if "--lines" in args:
            k = args.index("--lines")
            a, b = args[k + 1].split("-")
            lines = (int(a), int(b))
            args = args[:k] + args[k + 2:]
        if "--out" in args:
            k = args.index("--out")
            out = args[k + 1]
            args = args[:k] + args[k + 2:]
        return extract([_abs(p) for p in args], lines, out)
    if len(argv) == 4 and argv[1] == "apply":
        return apply(_abs(argv[2]), argv[3])
    if len(argv) == 3 and argv[1] == "apply":
        return apply(None, argv[2])
    print(__doc__)
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
