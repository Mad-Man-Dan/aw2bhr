#!/usr/bin/env python3
"""Say WHY agbcc's register allocator gave a draft different registers than the ROM.

    python tools/regprio.py <fn> [--src FILE] [--target FILE] [--keep DIR] [-v]
    python tools/regprio.py --self-test

Most parked residuals are size-exact with the ROM's control flow, and only
register names differ: two values trade hard registers, or one lands in a
different low register. This tool reads the allocator's own facts instead of
guessing respellings.

  1. Compile the draft with `-dl -dg` (same cpp/cc1 line as rtldump.py).
  2. From .greg take, per pseudo: refs (loop-depth weighted), live_length, the
     conflict set, preferences, the final hard register, and the order
     global-alloc took them in. From .lreg take which pseudos local-alloc
     already placed and, per insn, which pseudos it names.
  3. Align the draft's asm (out.s) to the ROM's target.s instruction by
     instruction (mnemonic + operand shape, register-blind, difflib when the
     sizes differ) and collect every operand whose register differs.
  4. Blame each differing operand on a pseudo: the pseudos named by the insns
     of the same source-line chunk (.LMn label <-> line note) whose final hard
     register is the draft's. No candidate means a reload scratch register.
  5. Print the mismatched pseudos, and for each the pseudo that HOLDS the
     register it wants in the draft, with the change that would flip them.

The priority is global.c's allocno_compare:  floor_log2(refs) * refs / live_length
(times size); ties go to the LOWER pseudo number (creation order). Global-alloc
takes the highest first. Local-alloc places block-local pseudos BEFORE any
global allocno, whatever their priority; it uses the same quotient. Registers
are offered in REG_ALLOC_ORDER (r3 r2 r1 r0 for a value that crosses no call,
then r4 upwards), so the first-allocated value gets the EARLIER register in
that order, which for r0-r3 is the HIGHER number.

Limits are listed at the end of the section this adds to docs/agbcc-codegen.md.
"""

import argparse
import difflib
import os
import re
import shlex
import shutil
import sys
import tempfile
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import agbenv  # noqa: E402
import awlib  # noqa: E402

FIRST_PSEUDO = 22
# ARM/Thumb REG_ALLOC_ORDER as documented in agbcc-codegen.md (wave 51): a value
# that crosses no call is offered r3, r2, r1, r0 first; call-crossers can only
# use r4 and up, in ascending order.
ALLOC_ORDER = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
ALIAS = {"sb": "r9", "sl": "r10", "fp": "r11", "ip": "r12"}


# ---------------------------------------------------------------- compile
def compile_dumps(fn, src, outdir):
    f = agbenv.flags(fn)
    od = shlex.quote(agbenv.wsl_path(outdir))
    script = ('set -eo pipefail\n'
              '{cpp} {cppflags} {src} | iconv -f UTF-8 -t CP932 > {od}/in.i\n'
              '{cc1} {cflags} -dl -dg -dA -dumpbase {od}/dump {od}/in.i -o {od}/out.s\n'
              ).format(od=od, cpp=f["CPP"], cppflags=f["CPPFLAGS"],
                       src=shlex.quote(src.replace("\\", "/")), cc1=f["CC1"],
                       cflags=f["CFLAGS"])
    rc, _, err = agbenv.run(script)
    if rc != 0:
        raise SystemExit(err or "regprio: the compile failed (exit %d)" % rc)


# ---------------------------------------------------------------- dumps
def parse_greg(text):
    g = {"refs": {}, "live": {}, "size": {}, "conf": {}, "pref": {}, "info": {},
         "disp": {}, "order": [], "insns": []}
    for m in re.finditer(r"^Register (\d+), refs = (\d+), live_length = (\d+), "
                         r"size = (\d+)", text, re.M):
        r, n, ll, sz = map(int, m.groups())
        g["refs"][r], g["live"][r], g["size"][r] = n, ll, sz
    m = re.search(r"^;; \d+ regs to allocate:(.*)$", text, re.M)
    if m:
        g["order"] = [int(x) for x in m.group(1).split()]
    for m in re.finditer(r"^;; (\d+) conflicts:(.*)$", text, re.M):
        g["conf"][int(m.group(1))] = set(int(x) for x in m.group(2).split())
    for m in re.finditer(r"^;; (\d+) preferences:(.*)$", text, re.M):
        g["pref"][int(m.group(1))] = [int(x) for x in m.group(2).split()]
    for m in re.finditer(r"^Register (\d+) used (\d+) times across (\d+) insns"
                         r"(?: in block (\d+))?;?(.*)$", text, re.M):
        g["info"][int(m.group(1))] = {"flags": m.group(5).strip(),
                                      "block": m.group(4)}
    d = re.search(r"^;; Register dispositions:\n(.*?)\n\n", text, re.M | re.S)
    if d:
        for r, h in re.findall(r"(\d+) in (-?\d+)", d.group(1)):
            g["disp"][int(r)] = int(h)
    g["insn_regs"] = {}
    for blk in re.split(r"\n\n(?=\()", text):
        m = re.match(r"\((?:insn|jump_insn|call_insn) (\d+) ", blk)
        if m:
            g["insn_regs"][int(m.group(1))] = set(
                int(x) for x in re.findall(
                    r"\(reg(?:/\w+)*:[A-Z]{2} (\d+) [a-z0-9]+\)", blk))
    g["spills"] = []
    for m in re.finditer(r"^Spilling for insn (\d+)\.$((?:\n^Spilling reg \d+\.$)*)",
                         text, re.M):
        for r in re.findall(r"Spilling reg (\d+)", m.group(2)):
            g["spills"].append((int(m.group(1)), int(r)))
    # Insn / line-note stream in final order.
    for m in re.finditer(r"^\((insn|jump_insn|call_insn) (\d+) |"
                         r"^\(note \d+ \d+ \d+ \(\"[^\"]*\"\) (\d+)\)",
                         text, re.M):
        if m.group(2):
            g["insns"].append(("insn", int(m.group(2))))
        else:
            g["insns"].append(("line", int(m.group(3))))
    return g


PSEUDO_RE = re.compile(r"\(reg(?:/[a-z]+)*:[A-Z]{2} (\d+)\)")


def parse_lreg(text):
    lr = {"local": set(), "uid_pseudos": {}, "pos": {}, "pos_refs": defaultdict(list),
          "bbs": []}
    for m in re.finditer(r"^;; Register (\d+) in (-?\d+)\.", text, re.M):
        if int(m.group(2)) >= 0:
            lr["local"].add(int(m.group(1)))
    for i, m in enumerate(re.finditer(
            r"^\((?:insn|jump_insn|call_insn|note|code_label|barrier) (\d+) ",
            text, re.M)):
        lr["pos"][int(m.group(1))] = i
    for blk in re.split(r"\n\n(?=\()", text):
        m = re.match(r"\((?:insn|jump_insn|call_insn) (\d+) ", blk)
        if m:
            ps = set(int(x) for x in PSEUDO_RE.findall(blk)
                     if int(x) >= FIRST_PSEUDO)
            uid = int(m.group(1))
            lr["uid_pseudos"][uid] = ps
            for q in ps:
                lr["pos_refs"][q].append(lr["pos"].get(uid, -1))
    for m in re.finditer(r"^BB (\d+), start (\d+), end (\d+)$.*?"
                         r"^Registers live at start:([^\n]*)$", text, re.M | re.S):
        live = set(int(x) for x in m.group(4).split() if int(x) >= FIRST_PSEUDO)
        lr["bbs"].append((lr["pos"].get(int(m.group(2)), -1),
                          lr["pos"].get(int(m.group(3)), -1), live))
    return lr


def live_at(lr, uid):
    """Pseudos live at insn uid, at block granularity plus in-block refs."""
    pos = lr["pos"].get(uid)
    if pos is None:
        return set()
    for k, (a, b, live) in enumerate(lr["bbs"]):
        lo, hi = min(a, b), max(a, b)
        if lo <= pos <= hi:
            nxt = lr["bbs"][k + 1][2] if k + 1 < len(lr["bbs"]) else set()
            out = set()
            cand = set(live)
            for q, ps in lr["pos_refs"].items():
                if any(lo <= x <= hi for x in ps):
                    cand.add(q)
            for q in cand:
                inb = [x for x in lr["pos_refs"].get(q, ()) if lo <= x <= hi]
                before = q in live or any(x <= pos for x in inb)
                after = q in nxt or any(x >= pos for x in inb)
                if before and after:
                    out.add(q)
            return out
    return set()


def priority(refs, live, size=1):
    if refs <= 0 or live <= 0:
        return 0.0
    return (refs.bit_length() - 1) * refs * size / live


def pri_of(g, r):
    return priority(g["refs"].get(r, 0), g["live"].get(r, 1), g["size"].get(r, 1))


# ---------------------------------------------------------------- asm
FLAGS_S = {"rsbs", "adds", "subs", "movs", "lsls", "lsrs", "asrs", "ands", "orrs",
           "eors", "bics", "muls", "negs", "mvns", "adcs", "sbcs", "rors"}
ALU3 = {"add", "sub", "lsl", "lsr", "asr"}
ALU2 = {"and", "orr", "eor", "bic", "adc", "sbc", "ror", "mul"}
REG_RE = re.compile(r"^(r\d+|sp|lr|pc|sb|sl|fp|ip)$")


def reg_name(t):
    t = t.strip()
    return ALIAS.get(t, t)


def parse_insn(line):
    line = line.split("@")[0].strip()
    if not line:
        return None
    parts = line.split(None, 1)
    mn = parts[0].lower()
    ops = parts[1] if len(parts) > 1 else ""
    if mn.startswith(".") or mn.endswith(":"):
        return None
    if mn in ("push", "pop"):
        regs = sorted(reg_name(x) for x in re.findall(r"[a-z0-9]+", ops.lower()))
        return {"mn": mn, "shape": (mn,), "regs": [], "set": regs, "imm": [],
                "text": line}
    # flatten operands; keep [ ] structure in the shape
    ops = ops.lower().replace(" ", "")
    toks = re.findall(r"\[[^\]]*\]|[^,]+", ops)
    if mn in FLAGS_S:
        mn = mn[:-1]
    ops_l = []
    for t in toks:
        ops_l.append(t)
    # normalise sub sp / add sp, sp
    if mn == "sub" and len(ops_l) >= 2 and ops_l[0] == "sp":
        n = int(ops_l[-1].lstrip("#"), 0)
        mn, ops_l = "add", ["sp", "sp", "#%d" % -n]
    elif mn == "add" and len(ops_l) == 3 and ops_l[0] == "sp" == ops_l[1]:
        ops_l[2] = "#%d" % int(ops_l[2].lstrip("#"), 0)
    if mn == "rsb" and len(ops_l) == 3 and ops_l[2] in ("#0", "#0x0"):
        mn, ops_l = "neg", ops_l[:2]
    mn = {"bhs": "bcs", "blo": "bcc"}.get(mn, mn)
    if mn in ALU3 and len(ops_l) == 2:
        ops_l = [ops_l[0], ops_l[0], ops_l[1]]
    if mn in ALU2 and len(ops_l) == 3:
        if mn == "mul" and ops_l[0] == ops_l[2]:
            ops_l = ops_l[:2]
        elif mn == "mul" and ops_l[0] == ops_l[1]:
            ops_l = [ops_l[0], ops_l[2]]
        elif mn != "mul" and ops_l[0] == ops_l[1]:
            ops_l = [ops_l[0], ops_l[2]]
    if mn == "add" and len(ops_l) == 3 and ops_l[2] in ("#0", "#0x0")             and REG_RE.match(ops_l[1]):
        mn, ops_l = "mov", ops_l[:2]
    mn = {"stm": "stmia", "ldm": "ldmia"}.get(mn, mn)
    shape, regs, imm = [mn], [], []
    for t in ops_l:
        if t.startswith("{"):
            lst = sorted(reg_name(x) for x in re.findall(r"[a-z0-9]+", t))
            regs.extend(lst)
            shape.append("S%d" % len(lst))
        elif t.endswith("!") and REG_RE.match(t[:-1]):
            regs.append(reg_name(t[:-1]))
            shape.append("R!")
        elif t.startswith("["):
            inner = [x for x in t.strip("[]").split(",")]
            s = []
            for x in inner:
                if REG_RE.match(x):
                    regs.append(reg_name(x))
                    s.append("R")
                elif x.startswith("#"):
                    v = int(x[1:], 0)
                    if v != 0:
                        s.append("#")
                        imm.append(v)
            shape.append("[" + ",".join(s) + "]")
        elif REG_RE.match(t):
            regs.append(reg_name(t))
            shape.append("R")
        elif t.startswith("#"):
            shape.append("#")
            try:
                imm.append(int(t[1:], 0))
            except ValueError:
                pass
        else:
            shape.append("L")
    return {"mn": mn, "shape": tuple(shape), "regs": regs, "set": None,
            "imm": imm, "text": line}


def parse_draft_asm(text, fn):
    """Instructions of out.s with the .LMn chunk each sits in, plus LM->line."""
    lines = text.split("\n")
    lm_line, last_lm = {}, None
    for ln in lines:
        m = re.match(r"\s*\.4byte\s+\.LM(\d+)\s*$", ln)
        if m:
            last_lm = int(m.group(1))
            continue
        m = re.search(r"@ line (\d+)", ln)
        if m and last_lm is not None:
            lm_line.setdefault(last_lm, int(m.group(1)))
            last_lm = None
    out, chunk, on = [], 0, False
    # The draft may define the function under its renamed symbol, so take the
    # name from the first `.type X,function` rather than trusting `fn`.
    names = re.findall(r"^\s*\.type\s+(\S+),function", text, re.M)
    name = fn if fn in names or not names else names[0]
    for ln in lines:
        if re.match(r"%s:\s*$" % re.escape(name), ln):
            on = True
            continue
        if not on:
            continue
        if ln.strip().startswith(".size"):
            break
        m = re.match(r"\.LM(\d+):", ln)
        if m:
            chunk = int(m.group(1))
            continue
        if not ln.startswith("\t") or ln.strip().startswith("."):
            continue
        ins = parse_insn(ln)
        if ins:
            ins["chunk"] = chunk
            out.append(ins)
    return out, lm_line


def parse_target_asm(text):
    out, on = [], False
    for ln in text.split("\n"):
        s = ln.strip()
        if s.startswith("thumb_func_start"):
            on = True
            continue
        if s.startswith("thumb_func_end"):
            break
        if not on or not s or s.startswith(".") or re.match(r"^[\w.]+:", s):
            if not re.match(r"^[\w.]+:\s*\S", s):
                continue
            s = re.sub(r"^[\w.]+:\s*", "", s)
            if s.startswith("."):
                continue
        ins = parse_insn(s)
        if ins:
            out.append(ins)
    return out


def align(draft, rom):
    dk = [i["shape"] for i in draft]
    rk = [i["shape"] for i in rom]
    if dk == rk:
        return list(zip(range(len(draft)), range(len(rom)))), True
    sm = difflib.SequenceMatcher(None, dk, rk, autojunk=False)
    pairs = []
    for a, b, n in sm.get_matching_blocks():
        pairs.extend((a + k, b + k) for k in range(n))
    return pairs, False


# ---------------------------------------------------------------- chunks
def build_chunks(g, lm_line):
    """Map each greg insn uid to an .LM chunk via the line-note sequence."""
    notes = [v for k, v in g["insns"] if k == "line"]
    lms = sorted(lm_line)
    lm_lines = [lm_line[k] for k in lms]
    sm = difflib.SequenceMatcher(None, notes, lm_lines, autojunk=False)
    note_lm = {}
    for a, b, n in sm.get_matching_blocks():
        for k in range(n):
            note_lm[a + k] = lms[b + k]
    uid_chunk, ni, cur = {}, -1, 1
    for kind, v in g["insns"]:
        if kind == "line":
            ni += 1
            cur = note_lm.get(ni, cur)
        else:
            uid_chunk[v] = cur
    return uid_chunk


# ---------------------------------------------------------------- analysis
def analyse(fn, src_text, draft_asm, rom_asm, g, lr):
    draft, lm_line = parse_draft_asm(draft_asm, fn)
    rom = parse_target_asm(rom_asm)
    pairs, exact = align(draft, rom)
    uid_chunk = build_chunks(g, lm_line)
    chunk_ps = defaultdict(set)
    for uid, ps in lr["uid_pseudos"].items():
        c = uid_chunk.get(uid)
        if c is not None:
            chunk_ps[c] |= ps
    ps_chunks = defaultdict(set)
    for c, ps in chunk_ps.items():
        for p in ps:
            ps_chunks[p].add(c)

    spills_by_chunk = defaultdict(list)
    for insn, reg in g["spills"]:
        c = uid_chunk.get(insn)
        if c is not None:
            spills_by_chunk[c].append((insn, reg))
    chunk_uids = defaultdict(list)
    for uid, c in uid_chunk.items():
        chunk_uids[c].append(uid)
    for uid in g["insn_regs"]:
        c = uid_chunk.get(uid)
        if c is not None and uid not in chunk_uids[c]:
            chunk_uids[c].append(uid)
    reloads = defaultdict(float)
    reload_key, late = {}, []          # (insn, dreg, rreg) -> operands
    votes = defaultdict(Counter)          # pseudo -> Counter((dreg, rreg))
    scratch = Counter()                   # (dreg, rreg) with no pseudo
    other_diffs = 0
    amb = set()
    for di, ri in pairs:
        d, r = draft[di], rom[ri]
        if d["set"] is not None:
            continue
        if d["imm"] != r["imm"]:
            other_diffs += 1
        for a, b in zip(d["regs"], r["regs"]):
            if a == b:
                continue
            if not a.startswith("r") or not a[1:].isdigit():
                continue
            hd = int(a[1:])
            # Which RTL insns of this chunk carry every register the asm insn
            # names? All of them reload-made (uid unknown to .lreg) means the
            # operand is a reload scratch, not a pseudo.
            need = set(int(x[1:]) for x in d["regs"]
                       if x[0] == "r" and x[1:].isdigit())
            hits = [u for u in chunk_uids.get(d["chunk"], ())
                    if need <= g["insn_regs"].get(u, set())]
            is_reload = bool(hits) and all(u not in lr["uid_pseudos"] for u in hits)
            rl = ([s for s in spills_by_chunk.get(d["chunk"], ()) if s[1] == hd]
                  if is_reload else [])
            rr = int(b[1:]) if b[1:].isdigit() else -1
            if rl:
                for insn, _ in rl:
                    reloads[(insn, hd, rr)] += 1.0 / len(rl)
                    reload_key.setdefault((d["chunk"], hd, rr), insn)
                continue
            cand = [p for p in chunk_ps.get(d["chunk"], ())
                    if g["disp"].get(p) == hd]
            if not cand:
                rl_any = [s for s in spills_by_chunk.get(d["chunk"], ())
                          if s[1] == hd]
                if rl_any:
                    reloads[(rl_any[0][0], hd, rr)] += 1.0
                    reload_key.setdefault((d["chunk"], hd, rr), rl_any[0][0])
                else:
                    scratch[(a, b)] += 1
                continue
            if len(cand) > 1:
                amb.update(cand)
            late.append((d["chunk"], hd, rr, a, b, cand))
    # A use of a reload register inside the original insn looks like a use of
    # a pseudo in that register; when the chunk already has a reload with the
    # same draft->ROM pair, that use belongs to the reload.
    for chunk, hd, rr, a, b, cand in late:
        insn = reload_key.get((chunk, hd, rr))
        if insn is not None:
            reloads[(insn, hd, rr)] += 1.0
            continue
        for p in cand:
            votes[p][(a, b)] += 1.0 / len(cand)
    return {"draft": draft, "rom": rom, "pairs": pairs, "exact": exact,
            "votes": votes, "scratch": scratch, "amb": amb, "reloads": reloads,
            "uid_chunk": uid_chunk,
            "chunk_ps": chunk_ps, "ps_chunks": ps_chunks,
            "lm_line": lm_line, "other_diffs": other_diffs,
            "src_lines": src_text.split("\n")}


def hard_conflicts(g, p):
    return sorted(x for x in g["conf"].get(p, ()) if x < FIRST_PSEUDO)


def interferes(g, an, p, h):
    if p in g["conf"]:
        return h in g["conf"][p]
    return bool(an["ps_chunks"][p] & an["ps_chunks"][h])


def first_line(an, p):
    cs = sorted(an["ps_chunks"].get(p, ()))
    for c in cs:
        ln = an["lm_line"].get(c)
        if ln:
            return ln
    return None


def need_refs(pw, refs, live, size):
    """Smallest k with priority(refs + k) > pw, or None within a sane bound."""
    for k in range(1, 200):
        if priority(refs + k, live, size) > pw:
            return k
    return None


def max_live(pw, refs, size):
    """Largest live_length that still gives priority > pw."""
    if refs <= 1:
        return None
    lg = refs.bit_length() - 1
    x = lg * refs * size / pw if pw > 0 else None
    if x is None:
        return None
    n = int(x)
    if n >= x:
        n -= 1
    return n if n >= 1 else None


def rank(reg):
    return ALLOC_ORDER.index(reg) if reg in ALLOC_ORDER else 99


def kind_of(g, lr, p):
    return "local" if p in lr["local"] else "global"


def replay(g, lr, order, p):
    """Re-run find_reg for global pseudo p from the dump's conflict facts.

    Returns (candidates, why): candidates is the set of registers the replay
    could pick (pass 0 pick, and pass 1 pick when pass 0 skipped something),
    why maps each excluded register to the reason it was excluded.
    """
    flags = g["info"].get(p, {}).get("flags", "")
    conf = g["conf"].get(p, set())
    why = {}
    for x in conf:
        if x < FIRST_PSEUDO:
            why[x] = "in its hard-reg conflict set"
    for q in conf:
        if q < FIRST_PSEUDO or q == p or g["disp"].get(q) is None:
            continue
        if q in lr["local"] or (q in order and p in order and order[q] < order[p]):
            why.setdefault(g["disp"][q], "held by p%d (placed earlier)" % q)
    if "crosses" in flags:
        for x in range(4):
            why.setdefault(x, "call-clobbered and p%d crosses a call" % p)
    skip = {}
    if p in order:
        for q in conf:
            if q in order and order[q] > order[p]:
                for x in g["pref"].get(q, []):
                    skip.setdefault(x, q)
    prefs = g["pref"].get(p, [])
    seq = prefs + [x for x in ALLOC_ORDER if x not in prefs]
    cands = set()
    pass0 = next((x for x in seq if x not in why and x not in skip), None)
    pass1 = next((x for x in seq if x not in why), None)
    for x in (pass0, pass1):
        if x is not None:
            cands.add(x)
    for x, q in skip.items():
        why.setdefault(x, "pass 0 skips it: later conflicting p%d prefers it" % q)
    return cands, why


def line_text(an, p):
    ln = first_line(an, p)
    if ln and 0 < ln <= len(an["src_lines"]):
        return "line %d: %s" % (ln, an["src_lines"][ln - 1].strip()[:44])
    return "line ?"


def explain(w, an, g, lr, order, mism, p):
    a, b, _ = mism[p]
    hd, hr = int(a[1:]), int(b[1:])
    pp = pri_of(g, p)
    pk = kind_of(g, lr, p)
    w("\np%d  %s -> %s  [%s, refs %d, live %d, prio %.2f%s]  %s\n" % (
        p, a, b, pk, g["refs"].get(p, 0), g["live"].get(p, 0), pp,
        ", global order #%d" % (order[p] + 1) if p in order else "",
        line_text(an, p)))
    facts = g["info"].get(p, {}).get("flags", "")
    if facts:
        w("  facts: %s\n" % facts)
    if p in g["conf"]:
        hc = hard_conflicts(g, p)
        w("  hard-reg conflicts: %s   preferences: %s\n" % (
            " ".join("r%d" % x for x in hc) or "none",
            " ".join("r%d" % x for x in g["pref"].get(p, [])) or "none"))
    if pk == "local":
        w("  local-alloc placed this one (block-local): the dump gives no order "
          "among locals, so the replay is limited to the conflict set.\n")
        why = {x: "in its hard-reg conflict set" for x in hard_conflicts(g, p)
               if x != hd}
        cands = {hd}
    else:
        cands, why = replay(g, lr, order, p)
    if hd not in cands:
        w("  replay picks %s but the allocator gave %s: the model does not "
          "cover this pseudo, so the reasons below are only indicative.\n" % (
              " or ".join("r%d" % x for x in sorted(cands)) or "nothing", a))
    if hr in why:
        w("  ROM's %s is excluded in the draft: %s.\n" % (b, why[hr]))
    elif hr < hd:
        w("  ROM's %s is LOWER than the draft's %s and nothing excludes it in "
          "the replay: the ROM's pseudo must have had a different life or "
          "preferences (the replay would take %s).\n" % (
              b, a, " or ".join("r%d" % x for x in sorted(cands))))
    else:
        below = [x for x in ALLOC_ORDER if x < hr]
        w("  ROM's %s is HIGHER than the draft's %s: in the ROM every register "
          "below it (%s) was unavailable at this pseudo's turn. In the draft: "
          "%s.\n" % (
              b, a, "r0" if hr == 1 else "r0-r%d" % (hr - 1),
              "; ".join("r%d %s" % (x, why.get(x, "FREE")) for x in below)))
        free = [x for x in below if x not in why]

        def placed_before(q):
            return q in lr["local"] or (q in order and p in order and
                                        order[q] < order[p])

        def near(q):
            cs = an["ps_chunks"].get(p, set())
            return any(abs(c - d) <= 1 for c in cs
                       for d in an["ps_chunks"].get(q, ()))
        for x in (free if pk == "global" else []):
            qs = [q for q, r in g["disp"].items()
                  if r == x and q != p and not interferes(g, an, p, q)
                  and ((pk == "global" and placed_before(q)
                        and (q in order or near(q)))
                       or (pk == "local" and near(q)))]
            qs.sort(key=lambda q: (kind_of(g, lr, q) != "global",
                                   -pri_of(g, q), q))
            if qs:
                w("  to make r%d unavailable, one of these (%s, holding r%d "
                  "elsewhere) would have to be live across p%d's life in the "
                  "ROM:\n" % (x, "placed before p%d" % p if pk == "global"
                              else "nearby", x, p))
                for q in qs[:3]:
                    w("    p%d [%s, refs %d, live %d, prio %.2f, %s]\n" % (
                        q, kind_of(g, lr, q), g["refs"].get(q, 0),
                        g["live"].get(q, 0), pri_of(g, q), line_text(an, q)))
    # Earlier-placed holders of the ROM's register that a priority change could move.
    holders = [h for h, r in g["disp"].items()
               if r == hr and h != p and interferes(g, an, p, h)]
    for h in sorted(holders, key=lambda h: (-pri_of(g, h), h)):
        ph = pri_of(g, h)
        hk = kind_of(g, lr, h)
        earlier = (hk == "local") if hk != pk else (ph, -h) > (pp, -p)
        head = "  p%d holds %s [%s, refs %d, live %d, prio %.2f, %s]: " % (
            h, b, hk, g["refs"].get(h, 0), g["live"].get(h, 0), ph,
            line_text(an, h))
        if not earlier:
            w(head + "placed after p%d; a consequence of p%d leaving %s.\n" % (
                p, p, b))
        elif hk == "local":
            w(head + "local-alloc runs before global-alloc, whatever the "
              "priorities. p%d has to become block-local, or p%d has to span "
              "blocks.\n" % (p, h))
        else:
            k = need_refs(ph, g["refs"].get(p, 0), g["live"].get(p, 1),
                          g["size"].get(p, 1))
            ml = max_live(ph, g["refs"].get(p, 0), g["size"].get(p, 1))
            opts = []
            if ph == pp:
                opts.append("create p%d before p%d (equal priority: the lower "
                            "number wins)" % (p, h))
            if k:
                opts.append("+%d weighted use%s on p%d" % (
                    k, "" if k == 1 else "s", p))
            if ml is not None and ml < g["live"].get(p, 1):
                opts.append("p%d live_length <= %d (now %d)" % (
                    p, ml, g["live"].get(p, 0)))
            low = next((r2 for r2 in range(g["refs"].get(h, 0), 0, -1)
                        if priority(r2, g["live"].get(h, 1),
                                    g["size"].get(h, 1)) < pp), None)
            if low is not None and low < g["refs"].get(h, 0):
                opts.append("or p%d down to %d weighted refs" % (h, low))
            w(head + "allocated BEFORE p%d (%.2f vs %.2f). To flip: %s.\n" % (
                p, ph, pp, "; ".join(opts) or "no small change found"))
    if hd in g["pref"].get(p, []):
        w("  the draft's %s is one of p%d's PREFERENCES (a copy to or from a "
          "value already in %s, or an argument/return slot); preferences are "
          "tried before the offer order.\n" % (a, p, a))

def spill_costs(an, g, lr, insn):
    """Per hard register: weighted refs of the pseudos it would evict at insn.

    reload1.c picks a spill register by the lowest such cost, ties to the
    lowest register (count_pseudo / find_reg). Approximate: liveness comes from
    block live-at-start sets plus in-block references.
    """
    cost, who = defaultdict(int), defaultdict(list)
    for q in live_at(lr, insn):
        r = g["disp"].get(q)
        if r is None or r < 0:
            continue
        cost[r] += g["refs"].get(q, 0)
        who[r].append(q)
    return cost, who


def explain_reloads(w, an, g, lr):
    w("\nreload scratch registers (a value needed a low register for one "
      "insn; reload's find_reg spills the hard register whose live pseudos "
      "cost least, ties to the lowest number)\n")
    for (insn, hd, hr), n in sorted(an["reloads"].items()):
        c = an["uid_chunk"].get(insn)
        ln = an["lm_line"].get(c)
        txt = ""
        if ln and 0 < ln <= len(an["src_lines"]):
            txt = an["src_lines"][ln - 1].strip()[:50]
        cost, who = spill_costs(an, g, lr, insn)
        w("\ninsn %d, line %s: %s\n" % (insn, ln or "?", txt))
        w("  draft spilled r%d, ROM r%d  (%g operand%s)\n" % (
            hd, hr, round(n, 1), "" if round(n, 1) == 1 else "s"))
        w("  live pseudos by register at this insn (weighted refs): %s\n" % (
            "; ".join("r%d=%d [%s]" % (
                r, cost[r], ",".join("p%d" % q for q in who[r][:4]))
                for r in range(0, max(hd, hr) + 1) if cost.get(r))
            or "none in r0-r%d" % max(hd, hr)))
        cd, cr = cost.get(hd, 0), cost.get(hr, 0)
        if hr > hd:
            w("  the ROM took a HIGHER register, so in the ROM r%d cost MORE "
              "than r%d. Draft costs: r%d=%d, r%d=%d. Something must be live "
              "across this insn in r%d in the ROM and be referenced at least "
              "%d more weighted time%s (or r%d must hold less).\n" % (
                  hd, hr, hd, cd, hr, cr, hd, max(cr - cd + 1, 1),
                  "" if max(cr - cd + 1, 1) == 1 else "s", hr))
        else:
            w("  the ROM took a LOWER register, so in the draft r%d has a live "
              "pseudo the ROM lacks (draft costs: r%d=%d, r%d=%d); look for a "
              "value the ROM has already dead here.\n" % (hr, hd, cd, hr, cr))


def report(fn, an, g, lr, out=sys.stdout):
    w = out.write
    votes = an["votes"]
    mism = {}
    for p, c in votes.items():
        # A pseudo's ROM register is its most frequent (draft->ROM) pair.
        (a, b), n = c.most_common(1)[0]
        if a != b:
            mism[p] = (a, b, c)
    npairs = len(an["pairs"])
    w("%s: %d draft insns, %d ROM insns, %d aligned%s\n" % (
        fn, len(an["draft"]), len(an["rom"]), npairs,
        "" if an["exact"] else
        " (opcode shapes differ in %d insns: their operand blame is unreliable)"
        % (max(len(an["draft"]), len(an["rom"])) - npairs)))
    if not mism and not an["scratch"] and not an["reloads"]:
        w("no register mismatches (0 pseudos, 0 reload scratch, 0 other "
          "operands); %d operand diffs of other kinds (imm/opcode)\n"
          % an["other_diffs"])
        return 0
    order = {p: i for i, p in enumerate(g["order"])}
    if an["reloads"]:
        explain_reloads(w, an, g, lr)
    if mism:
        w("\nmismatched pseudos (draft reg -> ROM reg; prio = "
          "floor_log2(refs)*refs/live)\n")
        w("%6s %-6s %-10s %4s %5s %5s %7s %5s  %s\n" % (
            "pseudo", "alloc", "draft->ROM", "ops", "refs", "live", "prio",
            "line", "source"))
    rows = sorted(mism, key=lambda p: (-pri_of(g, p), p))
    for p in rows:
        a, b, c = mism[p]
        ln = first_line(an, p)
        srctxt = ""
        if ln and 0 < ln <= len(an["src_lines"]):
            srctxt = an["src_lines"][ln - 1].strip()[:52]
        vs = ",".join("%s>%s x%g" % (x, y, round(n, 1)) for (x, y), n in
                      c.most_common(3))
        w("%6d %-6s %-10s %4g %5d %5d %7.2f %5s  %s%s\n" % (
            p, kind_of(g, lr, p), "%s->%s" % (a, b), round(sum(c.values()), 1),
            g["refs"].get(p, 0), g["live"].get(p, 0), pri_of(g, p),
            ln or "?", srctxt, " (~)" if p in an["amb"] else ""))
        if len(c) > 1:
            w("         all votes: %s\n" % vs)
    if an["scratch"]:
        w("\nno pseudo behind these operands (reload scratch / spill temp):\n")
        for (a, b), n in an["scratch"].most_common():
            w("  draft %s vs ROM %s: %d operand(s)\n" % (a, b, n))
    if not mism:
        return len(an["reloads"])
    w("\nwhy, per mismatched pseudo (replay = the allocator re-run from the dump's "
      "own conflict sets: lowest free register, r0 first; preferences first; "
      "pass 0 skips registers a LATER conflicting allocno prefers)\n")
    for p in rows:
        explain(w, an, g, lr, order, mism, p)
    # pair readout: who swaps with whom
    w("\nregister trades (draft reg -> ROM reg), the allocator's view:\n")
    by_pair = defaultdict(list)
    for p in rows:
        a, b, _ = mism[p]
        by_pair[(a, b)].append(p)
    for (a, b), ps in sorted(by_pair.items()):
        w("  %s -> %s: %s\n" % (a, b, " ".join("p%d" % p for p in ps)))
    w("\n(~) = the operand's chunk named several pseudos in that register; the "
      "votes are split between them.\n")
    return len(mism) + len(an["reloads"])


# ---------------------------------------------------------------- round-robin
# reload1.c (chain-based reload, gcc 2.95): find_reg gives each insn its own
# set of spill registers (chain->used_spill_regs, chosen by the spill cost
# regprio already models). choose_reload_regs then hands them out through
# allocate_reload_reg, which scans the global spill_regs[] array starting AFTER
# `last_spill_reg` and takes the first register that is free for this reload,
# in the reload's class, and (pass 0 only) already used in this insn and not
# for an inherited reload; pass 1 drops the sharing test. The winner's index
# becomes last_spill_reg. A reload that inherits a value or arrives with its
# register preassigned never calls allocate_reload_reg and leaves the pointer
# alone. last_spill_reg starts at -1 and is never reset inside a function.
# There is no dump of any of this, so the events are read out of a gdb run of
# the (debug-info) agbcc: needs `gdb` inside the WSL distro.
RR_NSPILL = 8


def rr_gdb_script():
    idx = range(RR_NSPILL)
    fmt = " ".join("%d" for _ in idx)
    spill_args = ", ".join("spill_regs[%d]" % i for i in idx)
    free_args = ", ".join("reload_reg_free_p(spill_regs[%d], reload_opnum[r], "
                          "reload_when_needed[r])" % i for i in idx)
    return """set pagination off
set confirm off
set $pr = -1
define rrflush
  if $pr >= 0
    printf "RESULT %%d %%d %%d\\n", $pr, reload_spill_index[$pr], last_spill_reg
    set $pr = -1
  end
end
break choose_reload_regs
commands
silent
rrflush
printf "CHOOSE %%d %%d %%d %%d %%d\\n", chain->insn->fld[0].rtint, n_reloads, n_spills, last_spill_reg, chain->used_spill_regs
continue
end
break allocate_reload_reg
commands
silent
rrflush
set $pr = r
printf "ALLOC %%d %%d %%d %%d %%d %%d %%d %%d %%d %%d SP {fmt} FREE {fmt} CLS %%d AT %%d INH %%d USED %%d\\n", chain->insn->fld[0].rtint, r, reload_in[r] != 0, reload_out[r] != 0, reload_when_needed[r], reload_opnum[r], reload_reg_class[r], reload_nregs[r], last_spill_reg, n_spills, {spill_args}, {free_args}, reg_class_contents[reload_reg_class[r]], reload_reg_used_at_all, reload_reg_used_for_inherit, reload_reg_used
continue
end
break emit_reload_insns
commands
silent
rrflush
set $j = 0
while $j < n_reloads
  printf "RELOAD %%d %%d %%d %%d %%d %%d %%d %%d\\n", chain->insn->fld[0].rtint, $j, reload_reg_rtx[$j] != 0 ? reload_reg_rtx[$j]->fld[0].rtint : -1, reload_inherited[$j], reload_override_in[$j] != 0, reload_in[$j] != 0, reload_out[$j] != 0, reload_when_needed[$j]
  set $j = $j + 1
end
continue
end
run
""".replace("%%", "%").replace("{fmt}", fmt).replace(
        "{spill_args}", spill_args).replace("{free_args}", free_args)


def rr_trace(fn, outdir):
    """Run agbcc under gdb on <outdir>/in.i and return the trace text."""
    f = agbenv.flags(fn)
    with open(os.path.join(outdir, "rr.gdb"), "w", newline="\n") as fh:
        fh.write(rr_gdb_script())
    od = shlex.quote(agbenv.wsl_path(outdir))
    script = ('command -v gdb >/dev/null || {{ echo NO_GDB >&2; exit 3; }}\n'
              'gdb -q -batch -x {od}/rr.gdb --args {cc1} {cflags} -dg '
              '-dumpbase {od}/rr {od}/in.i -o {od}/rr.s 2>&1\n'
              ).format(od=od, cc1=f["CC1"], cflags=f["CFLAGS"])
    rc, out, err = agbenv.run(script)
    if rc == 3 or "NO_GDB" in (err or ""):
        raise SystemExit("regprio --rr: gdb is not installed in WSL "
                         "(wsl -u root apt-get install -y gdb)")
    return out


def parse_rr(text):
    """Trace lines -> ordered allocs, the reload table per insn, used masks."""
    allocs, chain_mask = [], {}
    tables = defaultdict(list)
    for ln in text.split("\n"):
        w = ln.split()
        if not w:
            continue
        if w[0] == "CHOOSE":
            chain_mask[int(w[1])] = int(w[5]) & 0xffffffff
        elif w[0] == "ALLOC" and "FREE" in w and w[-2] == "USED":
            v = [int(x) for x in w[1:11]]
            n = v[9]
            sp = [int(x) for x in w[w.index("SP") + 1:w.index("FREE")]][:n]
            fr = [int(x) for x in w[w.index("FREE") + 1:w.index("CLS")]][:n]
            cls_mask, at, inh, used = [int(x) for x in w[w.index("CLS") + 1:]
                                       if x.lstrip("-").isdigit()]
            allocs.append({
                "uid": v[0], "r": v[1], "in": v[2], "out": v[3],
                "when": v[4], "opnum": v[5], "cls": v[6], "nregs": v[7],
                "last": v[8], "n": n, "S": sp, "free": fr,
                "cls_mask": cls_mask & 0xffffffff, "at_all": at & 0xffffffff,
                "inherit": inh & 0xffffffff, "used": used & 0xffffffff})
        elif w[0] == "RESULT":
            for a in reversed(allocs):
                if a["r"] == int(w[1]) and "reg" not in a:
                    a["reg"], a["after"] = int(w[2]), int(w[3])
                    break
        elif w[0] == "RELOAD":
            tables[int(w[1])].append({
                "j": int(w[2]), "reg": int(w[3]), "inherited": int(w[4]),
                "override": int(w[5]), "in": int(w[6]), "out": int(w[7]),
                "when": int(w[8])})
    return allocs, tables, chain_mask


def rr_pick(a, last=None):
    """allocate_reload_reg's scan; returns the spill index it takes (or None)."""
    last = a["last"] if last is None else last
    n = a["n"]
    for pas in (0, 1):
        i = last
        for _ in range(n):
            i = (i + 1) % n
            reg = a["S"][i]
            if not a["free"][i] or not (a["cls_mask"] >> reg) & 1:
                continue
            if a["nregs"] != 1:
                continue
            if pas or ((a["at_all"] >> reg) & 1 and not (a["inherit"] >> reg) & 1):
                return i
    return None


def rr_wants(an, allocs):
    """Pair each differing scratch operand with the alloc that made it."""
    by_chunk = defaultdict(list)
    for a in allocs:
        c = an["uid_chunk"].get(a["uid"])
        if c is not None and "reg" in a:
            by_chunk[c].append(a)
    want, done = {}, set()
    for di, ri in an["pairs"]:
        d, r = an["draft"][di], an["rom"][ri]
        if d["set"] is not None:
            continue
        for x, y in zip(d["regs"], r["regs"]):
            if x == y or not (x[0] == "r" and x[1:].isdigit()
                              and y[0] == "r" and y[1:].isdigit()):
                continue
            for a in by_chunk.get(d["chunk"], ()):
                if a["reg"] == int(x[1:]) and id(a) not in done:
                    want[id(a)] = int(y[1:])
                    done.add(id(a))
                    break
    return want


def rr_report(an, g, allocs, tables, w, manual=None):
    if not allocs:
        w("\nround-robin replay: no reload allocations were traced\n")
        return 0
    line_of, cur = {}, None
    for kind, v in g["insns"]:
        if kind == "line":
            cur = v
        else:
            line_of[v] = cur
    want = rr_wants(an, allocs)
    for uid, reg in (manual or {}).items():
        for a in allocs:
            if a["uid"] == uid:
                want[id(a)] = reg
    ptr = lambda a: "r%d" % a["S"][a["last"]] if a["last"] >= 0 else "none"  # noqa: E731
    w("\nreload round-robin replay (spill_regs = %s; last_spill_reg starts at "
      "-1; an allocation leaves the pointer ON the register it took)\n" % (
          " ".join("r%d" % r for r in allocs[0]["S"])))
    w("%3s %5s %5s  %-11s %6s %5s %5s  %s\n" % (
        "#", "insn", "line", "allowed", "before", "pick", "ROM", "replay"))
    bad = 0
    for k, a in enumerate(allocs):
        ok = rr_pick(a)
        pred = a["S"][ok] if ok is not None else None
        good = pred == a.get("reg")
        bad += not good
        allowed = " ".join("r%d" % r for r in a["S"] if not (a["used"] >> r) & 1)
        rom = want.get(id(a))
        w("%3d %5d %5s  %-11s %6s %5s %5s  %s\n" % (
            k, a["uid"], line_of.get(a["uid"], "?"), allowed or "-", ptr(a),
            "r%d" % a["reg"] if "reg" in a else "?",
            "r%d" % rom if rom is not None else "",
            "ok" if good else "MISMATCH (replay says %s)" % (
                "r%d" % pred if pred is not None else "none")))
    w("replay reproduced %d of %d picks\n" % (len(allocs) - bad, len(allocs)))
    order = [u for kind, u in g["insns"] if kind == "insn"]
    for k, a in enumerate(allocs):
        rom = want.get(id(a))
        if rom is None or rom == a.get("reg"):
            continue
        w("\nROM takes r%d where the draft takes r%d at insn %d (line %s)\n" % (
            rom, a["reg"], a["uid"], line_of.get(a["uid"], "?")))
        if rom not in a["S"]:
            w("  r%d is not in spill_regs (%s) at all, so the pointer cannot "
              "reach it: no chain ever put r%d in used_spill_regs. That is "
              "find_reload_regs' choice (fewest weighted refs of the pseudos "
              "live through the insn, ties to the LOWER register number), not "
              "the round-robin. The ROM needs r%d to look dearer than r%d at "
              "this insn, or this insn to need one more register.\n" % (
                  rom, " ".join("r%d" % r for r in a["S"]), rom,
                  a["reg"], rom))
            continue
        need = []
        for p in range(-1, a["n"]):
            i = rr_pick(a, p)
            if i is not None and a["S"][i] == rom:
                need.append("r%d" % a["S"][p] if p >= 0 else "none")
        w("  pointer needed before this alloc: %s (draft has %s)\n" % (
            " or ".join(need) or "unreachable with this allowed set", ptr(a)))
        prev = allocs[k - 1] if k else None
        if not prev:
            continue
        w("  the pointer is the register of the PREVIOUS alloc, insn %d "
          "(line %s), which took r%d: it is a position, not a count\n" % (
              prev["uid"], line_of.get(prev["uid"], "?"), prev["reg"]))
        try:
            seg = order[order.index(prev["uid"]) + 1:order.index(a["uid"])]
        except ValueError:
            seg = []
        alloc_uids = set(x["uid"] for x in allocs)
        silent = [(u, r) for u in seg if u not in alloc_uids
                  for r in tables.get(u, ()) if r["reg"] >= 0]
        if silent:
            w("  reloads in between that did NOT allocate (inherited or "
              "preassigned register); making one of them allocate would "
              "move the pointer:\n")
            for u, r in silent:
                w("    insn %d line %s: reload %d in r%d%s\n" % (
                    u, line_of.get(u, "?"), r["j"], r["reg"],
                    " (inherited)" if r["inherited"] else
                    " (override_in)" if r["override"] else ""))
    return bad


# ---------------------------------------------------------------- driver
def run(fn, src=None, target=None, keep=None, out=sys.stdout, rr=False,
        want=None):
    src = src or os.path.join(awlib.REPO, "work", fn, fn + ".c")
    target = target or os.path.join(awlib.REPO, "work", fn, "target.s")
    for p, what in ((src, "draft"), (target, "target.s")):
        if not os.path.exists(p):
            raise SystemExit("regprio: %s missing (%s)" % (p, what))
    outdir = keep or tempfile.mkdtemp(prefix="regprio_")
    os.makedirs(outdir, exist_ok=True)
    try:
        # cc1 runs in the repo root, so hand it a repo-relative or WSL path.
        rel = os.path.relpath(os.path.abspath(src), awlib.REPO)
        srcarg = rel if not rel.startswith("..") else agbenv.wsl_path(src)
        compile_dumps(fn, srcarg, outdir)
        rd = lambda n: open(os.path.join(outdir, n), encoding="utf-8",  # noqa: E731
                            errors="replace").read()
        g = parse_greg(rd("dump.greg"))
        lr = parse_lreg(rd("dump.lreg"))
        an = analyse(fn, open(src, encoding="utf-8", errors="replace").read(),
                     rd("out.s"),
                     open(target, encoding="utf-8", errors="replace").read(),
                     g, lr)
        rc = report(fn, an, g, lr, out)
        if rr:
            allocs, tables, _ = parse_rr(rr_trace(fn, outdir))
            rr_report(an, g, allocs, tables, out.write, want)
        return rc
    finally:
        if not keep:
            shutil.rmtree(outdir, ignore_errors=True)


def _pick_matched():
    """Promoted functions of 100-600 bytes, largest first: byte-exact drafts."""
    import json
    with open(os.path.join(awlib.REPO, "data", "promoted.json"),
              encoding="utf-8") as fh:
        prom = json.load(fh)
    out = [(e["size"], f) for e in prom for f in e["functions"]
           if 100 <= e["size"] <= 600]
    return [f for _, f in sorted(out, reverse=True)]


def self_test():
    """The matched case must report zero mismatches; a corrupted one must not."""
    import io
    tried = 0
    for fn in _pick_matched():
        d = os.path.join(awlib.REPO, "work", fn)
        if not (os.path.exists(os.path.join(d, fn + ".c")) and
                os.path.exists(os.path.join(d, "target.s"))):
            continue
        tried += 1
        buf = io.StringIO()
        try:
            run(fn, out=buf)
        except SystemExit:
            continue
        if "no register mismatches" not in buf.getvalue():
            continue
        print("self-test: matched %s reports zero mismatches" % fn)
        # Negative control: swap two register names in the ROM side.
        tgt = open(os.path.join(d, "target.s"), encoding="utf-8",
                   errors="replace").read()
        swapped = re.sub(r"\br1\b", "r@", tgt)
        swapped = re.sub(r"\br2\b", "r1", swapped).replace("r@", "r2")
        if swapped == tgt:
            continue
        tmp = tempfile.NamedTemporaryFile("w", suffix=".s", delete=False,
                                          encoding="utf-8")
        tmp.write(swapped)
        tmp.close()
        try:
            buf2 = io.StringIO()
            run(fn, target=tmp.name, out=buf2)
        finally:
            os.unlink(tmp.name)
        if "no register mismatches" in buf2.getvalue():
            print("self-test FAILED: r1/r2 swapped ROM still reports zero")
            return 1
        print("self-test: r1<->r2 swapped ROM is flagged")
        print("self-test OK")
        return 0
    print("self-test FAILED: no matched function found among %d tried" % tried)
    return 1


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("name", nargs="?")
    ap.add_argument("--src", help="draft (default work/<fn>/<fn>.c)")
    ap.add_argument("--target", help="ROM asm (default work/<fn>/target.s)")
    ap.add_argument("--keep", help="keep the dumps in this directory")
    ap.add_argument("--rr", action="store_true",
                    help="replay reload's round-robin spill-register choice "
                         "(needs gdb in WSL)")
    ap.add_argument("--want", action="append", metavar="UID:rN",
                    help="with --rr: the ROM's register for the alloc at insn UID")
    ap.add_argument("--self-test", action="store_true")
    a = ap.parse_args()
    if a.self_test:
        return self_test()
    if not a.name:
        ap.error("function name required")
    want = {int(u): int(r.lstrip("r")) for u, r in
            (x.split(":") for x in a.want or ())}
    run(a.name, a.src, a.target, a.keep, rr=a.rr, want=want)
    return 0


if __name__ == "__main__":
    sys.exit(main())
