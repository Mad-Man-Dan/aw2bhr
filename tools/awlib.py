"""Shared parsing for the aw2bhr assembly corpus.

Design constraint: the split must be byte-for-byte lossless. Every source file is
therefore modelled as a preamble chunk plus one chunk per function, where each
chunk is the *verbatim* line range from its `*_func_start` line up to (but not
including) the next one. Concatenating preamble + chunks reproduces the original
file exactly; tools/verify_split.py asserts this.

Bytes are decoded with surrogateescape and line endings are preserved, so any
non-UTF-8 content round-trips unchanged.
"""

import os
import re

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ASM_DIR = os.path.join(REPO, "asm")
DATA_DIR = os.path.join(REPO, "data")

# Where .rodata begins, per aw2bhr.lds. Bounds the final function.
ROM_CODE_END = 0x0808F098

FUNC_START_RE = re.compile(
    r'^\s*(arm_func_start|thumb_func_start|non_word_aligned_thumb_func_start)\s+(\S+)\s*$')
FUNC_ADDR_RE = re.compile(r'^(\S+):\s*@\s*(0x[0-9A-Fa-f]+)')
LOCAL_DEF_RE = re.compile(r'^(_[0-9A-Fa-f]{8}):')
LOCAL_REF_RE = re.compile(r'\b(_[0-9A-Fa-f]{8})\b')
# `bl sub_0801E4B0` / `b SomeFunc` — symbol targets only. `bx lr` and `bx r0`
# are returns/indirect jumps, so register operands must be excluded or nearly
# every function looks like a caller.
CALL_RE = re.compile(r'^\s*(bl|blx|b|bx)\s+([A-Za-z_][A-Za-z0-9_]*)\s*$')
REGISTERS = frozenset(
    [f"r{n}" for n in range(16)]
    + [f"a{n}" for n in range(1, 5)]
    + [f"v{n}" for n in range(1, 9)]
    + ["sp", "lr", "pc", "ip", "fp", "sb", "sl"]
)
# `ldr r0, _0801D3EC @ =gUnknown_03003040` — the pool comment names the global.
POOL_SYM_RE = re.compile(r'@\s*=([A-Za-z_][A-Za-z0-9_]*)')


def read_lines(path):
    """Read a file as lines with original line endings and byte-exact content."""
    with open(path, "rb") as fh:
        raw = fh.read()
    return raw.decode("utf-8", errors="surrogateescape").splitlines(keepends=True)


def byte_score(tgt_fn, cand, size):
    """(differing bytes, common length, percent identical) of a candidate's
    .text against the target function's `size` bytes. The score is taken over
    the LONGER of the two: bytes past the end of the shorter one count as
    differences, so a draft that compiles too long cannot reach 100%."""
    cand_fn = cand[:size]
    common = min(len(tgt_fn), len(cand_fn))
    total = max(size, len(cand))
    n_diff = sum(1 for a, b in zip(tgt_fn, cand_fn) if a != b) + (total - common)
    pct = (total - n_diff) / total * 100 if total else 0.0
    return n_diff, common, pct


def write_text(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as fh:
        fh.write(text.encode("utf-8", errors="surrogateescape"))


class Function:
    __slots__ = ("name", "addr", "mode", "directive", "src", "lines",
                 "lo", "hi", "size", "defs", "refs", "calls", "data_refs")

    def __init__(self, name, addr, directive, src, lo):
        self.name = name
        self.addr = addr
        self.directive = directive
        self.mode = "ARM" if directive == "arm_func_start" else "THUMB"
        self.src = src
        self.lo = lo          # index of the func_start line
        self.hi = None        # exclusive end index
        self.lines = None
        self.size = None
        self.defs = set()
        self.refs = set()
        self.calls = []
        self.data_refs = []

    @property
    def text(self):
        return "".join(self.lines)


class AsmFile:
    def __init__(self, path):
        self.path = path
        self.base = os.path.basename(path)
        self.lines = read_lines(path)
        self.funcs = []
        self.preamble_lines = []
        self.preamble_defs = set()
        self._parse()

    def _parse(self):
        starts = []
        pending = None
        for i, ln in enumerate(self.lines):
            m = FUNC_START_RE.match(ln)
            if m:
                pending = (i, m.group(1), m.group(2))
                continue
            if pending:
                a = FUNC_ADDR_RE.match(ln)
                if a and a.group(1) == pending[2]:
                    starts.append((pending[0], pending[1], pending[2],
                                   int(a.group(2), 16)))
                    pending = None
                elif ln.strip():
                    pending = None

        first = starts[0][0] if starts else len(self.lines)
        self.preamble_lines = self.lines[:first]
        for ln in self.preamble_lines:
            d = LOCAL_DEF_RE.match(ln)
            if d:
                self.preamble_defs.add(d.group(1))

        for n, (lo, directive, name, addr) in enumerate(starts):
            fn = Function(name, addr, directive, self.base, lo)
            fn.hi = starts[n + 1][0] if n + 1 < len(starts) else len(self.lines)
            fn.lines = self.lines[fn.lo:fn.hi]
            self._scan_body(fn)
            self.funcs.append(fn)

        # Size from address deltas (exact); the final function is estimated from
        # its own encoded length since there is no following label to subtract.
        for n, fn in enumerate(self.funcs):
            if n + 1 < len(self.funcs):
                fn.size = self.funcs[n + 1].addr - fn.addr
            else:
                fn.size = estimate_size(fn)

    def _scan_body(self, fn):
        for ln in fn.lines:
            code = ln.split("@")[0]
            d = LOCAL_DEF_RE.match(ln)
            if d:
                fn.defs.add(d.group(1))
            fn.refs.update(LOCAL_REF_RE.findall(code))
            c = CALL_RE.match(code.rstrip())
            if c:
                target = c.group(2)
                if target.lower() not in REGISTERS and not target.startswith("_0"):
                    fn.calls.append((c.group(1), target))
            p = POOL_SYM_RE.search(ln)
            if p:
                fn.data_refs.append(p.group(1))

    def reconstruct(self):
        """Must equal the original file text exactly."""
        return "".join(self.preamble_lines) + "".join(f.text for f in self.funcs)


DIRECTIVE_WIDTH = {"4byte": 4, "word": 4, "long": 4,
                   "2byte": 2, "short": 2, "hword": 2, "byte": 1}
DIR_RE = re.compile(r'^\s*\.(\w+)')
LABEL_RE = re.compile(r'^\S+:')


def estimate_size(fn):
    """Approximate encoded byte length from source lines.

    Only used for the last function in each file, where no following address
    exists to diff against. THUMB is 2 bytes/instruction except bl/blx (4).
    """
    total = 0
    for ln in fn.lines:
        s = ln.strip()
        if not s or s.startswith("@"):
            continue
        if FUNC_START_RE.match(ln) or FUNC_ADDR_RE.match(ln):
            continue
        d = DIR_RE.match(ln)
        if d:
            w = DIRECTIVE_WIDTH.get(d.group(1))
            if w:
                total += w * (s.count(",") + 1)
            continue
        if LABEL_RE.match(ln) and ":" in s.split()[0]:
            rest = s.split(":", 1)[1].strip()
            if not rest or rest.startswith("@"):
                continue
            s = rest
            # `_0806C99C: .4byte sym` is a pool WORD, not an instruction. The
            # DIR_RE test above never sees it because the line begins with the
            # label, so before wave 32 every such word was counted as 2 bytes
            # and each one made the function 2 bytes short. sub_0806C95C, the
            # last function of asm/code-0801D390.s, has three of them and was
            # indexed at 82 bytes instead of 88 -- a correct candidate could
            # not match, because trymatch slices the ROM to rec["size"].
            d = DIR_RE.match(s)
            if d:
                w = DIRECTIVE_WIDTH.get(d.group(1))
                if w:
                    total += w * (s.count(",") + 1)
                continue
        mnemonic = s.split()[0].lower().rstrip(",")
        total += 4 if (fn.mode == "ARM" or mnemonic in ("bl", "blx")) else 2
    return total


def load_all():
    """Parse every asm file, in linker order (see aw2bhr.lds)."""
    order = ["code.s", "code-0801D390.s", "code-0806CFC8.s", "code-0808D680.s"]
    files = []
    for base in order:
        p = os.path.join(ASM_DIR, base)
        if os.path.exists(p):
            files.append(AsmFile(p))
    return files


BRANCH_RE = re.compile(r'^\s*(b|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble)\b')
PUSH_RE = re.compile(r'^\s*push\s*\{([^}]*)\}')
SUBSP_RE = re.compile(r'^\s*sub\s+sp,\s*#(0x[0-9A-Fa-f]+|\d+)')
HIGHREG_RE = re.compile(r'\b(r8|r9|r10|r11|sb|sl|fp|ip)\b')
HELPER_RE = re.compile(r'\b(__divsi3|__udivsi3|__modsi3|__umodsi3|__muldi3|'
                       r'__lshrdi3|__negdi2|__fixunsdfsi|_call_via_)')
RETURN_RE = re.compile(r'^(bx\s+lr|mov\s+pc,\s*lr)\b')
MEMORY_RE = re.compile(r'^(ldr|ldrb|ldrh|ldrsb|ldrsh|ldm|str|strb|strh|stm|'
                       r'push|pop)\b')


# Assembler macros, not instructions. aw2bhr never emits the `*_func_end` forms
# but the Fire Emblem decomps do, and tools/fe_signatures.py parses both.
MACRO_RE = re.compile(r'^(arm|thumb|non_word_aligned_thumb)?_?func_(start|end)\b')


def instructions(fn):
    """The function body as bare instruction strings.

    Drops comments, directives, func macros, and label-only lines, while
    keeping any instruction that shares a line with a label.
    """
    out = []
    for ln in fn.lines:
        s = ln.split("@")[0].strip()
        if not s:
            continue
        if LABEL_RE.match(s):
            s = s.split(":", 1)[1].strip()
            if not s:
                continue
        if s.startswith(".") or MACRO_RE.match(s):
            continue
        out.append(s)
    return out


# `bhs`/`blo` are the unsigned aliases of `bcs`/`bcc`, and `asm/` is objdump
# output, which emits the alias spellings. Omitting them made a loop whose back
# edge is `cmp r4, r6; blo <top>` -- i.e. `for (i = 0; i < n; i++)` with an
# unsigned counter -- invisible, reporting `backward_branches == 0` for a
# function that has a loop. Found in wave 13 by agent A2, which measured the
# blast radius ROM-wide: 16 of the 3572 functions the index called straight-line
# actually have a back edge, four of them >=256 bytes, and `sub_080378A8` (two
# nested loops) was handed out as a straight-line target on the strength of it.
BRANCH_TARGET_RE = re.compile(
    r'^\s*(?:b|beq|bne|bcs|bcc|bhs|blo|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble)'
    r'(?:\.[nw])?\s+(\S+)')


def control_flow(fn):
    """(n_branches, n_backward, n_labels) for one function.

    `n_backward` is the count of branches whose target label is defined earlier
    in the body -- i.e. loops. It is the single best predictor of how hard a
    function is to match, and it is nearly independent of size.

    Wave 12 matched four functions of 280-368 bytes in five `try_match`
    attempts, having previously topped out at 124. Three of the four had no
    backward branch. A long straight-line function is many INDEPENDENT
    decisions, each of which fails with a diff pointing at itself; a short loop
    is one coupled decision where a wrong shape makes every later byte differ
    and teaches you nothing. So size is a bad difficulty proxy once control flow
    is known, and the scheduler should prefer `n_backward == 0` over `small`.
    """
    labels_seen, n_br, n_back, n_lab = set(), 0, 0, 0
    for ln in fn.lines:
        s = ln.split("@")[0].strip()
        if not s:
            continue
        if LABEL_RE.match(s):
            labels_seen.add(s.split(":", 1)[0])
            n_lab += 1
            s = s.split(":", 1)[1].strip()
            if not s:
                continue
        if s.startswith(".") or MACRO_RE.match(s):
            continue
        m = BRANCH_TARGET_RE.match(s)
        if m:
            n_br += 1
            # A target already defined above this point is a back edge. Targets
            # we have not seen yet are forward branches -- `if`/`else` and the
            # tail jumps every THUMB function is full of.
            if m.group(1) in labels_seen:
                n_back += 1
    return n_br, n_back, n_lab


TRIVIAL_MAX_INSNS = 4
# The only things allowed to precede the return in a stub. Deliberately narrow:
# arithmetic means the function computes something, however small, so it is
# real work and belongs in the normal queue.
STUB_OK_RE = re.compile(r'^(mov|movs|svc|nop)\b')


def is_trivial(fn):
    """True for a pure return stub -- `bx lr`, `movs r0, #0; bx lr`, and kin.

    These are free to match (`void f(void) {}`) but teach nothing about the
    game. Flagged so the scheduler can keep them out of the head of the queue
    and the fleet can sweep them as one bulk task rather than spending an agent
    apiece. Loads are excluded on purpose: `ldrh r0, [r0, #0x28]; bx lr` is a
    getter, and a getter reveals a struct field offset, which is worth having.
    """
    insns = instructions(fn)
    if not insns or len(insns) > TRIVIAL_MAX_INSNS:
        return False
    if not any(RETURN_RE.match(i) for i in insns):
        return False
    for i in insns:
        if RETURN_RE.match(i):
            continue
        if not STUB_OK_RE.match(i):
            return False
    return True


def difficulty(fn):
    """Heuristic 0-100 scheduling hint. Not a prediction of success --
    the Mizuchi benchmark found difficulty scoring correlates unevenly."""
    body = "".join(fn.lines)
    score = 0.0
    score += min(40.0, (fn.size or 0) / 32.0)             # size dominates
    branches = sum(1 for ln in fn.lines if BRANCH_RE.match(ln.split("@")[0]))
    score += min(20.0, branches * 0.8)
    score += min(10.0, len(fn.calls) * 1.0)
    stack = SUBSP_RE.search(body)
    if stack:
        score += min(10.0, int(stack.group(1), 0) / 8.0)
    saved = PUSH_RE.search(body)
    if saved:
        score += min(8.0, len(saved.group(1).split(",")) * 1.0)
    if HIGHREG_RE.search(body):
        score += 6.0                                       # register pressure
    if HELPER_RE.search(body):
        score += 6.0                                       # div/mul/float helpers
    return round(min(100.0, score), 1)
