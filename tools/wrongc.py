#!/usr/bin/env python3
"""Flag candidate C that changes what the function DOES, not just how it compiles.

The permuter scores by compiled text, so it happily keeps a mutation that scores
higher because the function is now different: a `volatile` that forces a stack
slot, a counter reassigned inside its own loop, a dropped multiply. Waves 93-96
lost a large share of their budget to these. This compares a candidate's
function against its base and reports every semantic change it can see.

    python tools/wrongc.py sub_0804CA98 work/sub_0804CA98/x.w96-perm1-WRONG-volatile.c
    python tools/wrongc.py sub_X cand.c --base work/sub_X/sub_X.c
    python tools/wrongc.py sub_X cand.c --no-emu      # patterns only, instant
    python tools/wrongc.py --self-test                # the benchmark; --no-emu for the quick half

Prints one line per finding, `OK` when there are none, and exits 1 if any
finding is a WRONG (0 for OK or WARN only).

    WRONG: <rule>: ...   the change is one that has always been wrong C here
    WARN:  <rule>: ...   suspicious, but legitimate rewrites also trigger it

The base is the file the candidate was derived from when its name says so
(`<fn>.wNN-permK-out.c` -> `<fn>.wNN-permK-start.c`), else work/<fn>/<fn>.c.

Two layers.

1. Pattern rules (no compile). The function is cut out of each file textually
   (headers are irrelevant, so raw header-expanded permuter output works),
   parsed with pycparser -- with `typedef int NAME;` invented for every type name
   the parse trips over -- and compared on features. WRONG rules: volatile added;
   the callee multiset changed (a helper defined in the same file, like the
   permuter's inline_fn, is transparent); a wide type used as an index; a
   variable the base had assigned a constant, or stepped inside an expression,
   in a loop; a local assigned but never read; a scalar local whose first use in
   the source is a read; statements after a return. WARN rules: any other
   change of operators, integer literals, parameter list, call order, or a call
   moving across a store.
2. Differential testing (tools/wrongc_emu.py). Both versions are compiled with
   the project's pipeline and run in a Thumb interpreter on the same random
   inputs, every callee stubbed to log its arguments. WRONG `behaviour`: the
   call log, the non-frame bytes written or the return value differ (the
   message names the first difference and how many runs show it). WRONG
   `uninit-dynamic`: the candidate's result changes when only the garbage it was
   never given is changed -- a read before set on a path the compiler's
   warning cannot see. The report says how much of the code the inputs reached;
   a `same` on 40% coverage is a weak `same`.

If a body will not parse or compile that layer says so and is skipped (exit 0):
no verdict is better than a wrong one. Reads before set are also checked with
the compiler's own warning by `--uninit` (permute.py and drafts.py do that
already).
"""

import argparse
import collections
import os
import re
import sys

from pycparser import CParser, c_ast, c_generator, c_parser

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
WORK = os.path.join(REPO, "work")

# Rules whose findings reject. The rest warn. Set from the benchmark (see
# docs/agbcc-codegen.md, "Wrong-C detector"); --self-test asserts the numbers.
RULES_WRONG = {
    "volatile", "callees", "wide-type", "loop-const", "loop-nested",
    "dead-local", "behaviour", "uninit-dynamic", "uninit-textual", "unreachable",
}
# The subset that is safe to apply to a file that is merely an OLDER draft (drafts.py
# bases): on 250 draft/start pairs the others also fire on ordinary respellings.
STRICT_RULES = {"volatile", "loop-const", "wide-type", "uninit-textual"}


# ---------------------------------------------------------------- extraction

def read_text(path):
    with open(path, "rb") as fh:
        raw = fh.read()
    for enc in ("utf-8", "cp932", "latin-1"):
        try:
            return raw.decode(enc).replace("\r\n", "\n")
        except UnicodeDecodeError:
            pass
    return raw.decode("latin-1")


_COMMENT_RX = re.compile(
    r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', re.S)


def strip_comments(s):
    """Blank comments and preprocessor lines, keeping newlines and strings."""
    def blank(m):
        t = m.group(0)
        return re.sub(r"[^\n]", " ", t) if t[0] == "/" else t
    text = _COMMENT_RX.sub(blank, s)
    res, cont = [], False
    for ln in text.split("\n"):
        if cont or ln.lstrip().startswith("#"):
            cont = ln.rstrip().endswith("\\")
            res.append("")
        else:
            res.append(ln)
    return "\n".join(res)


def find_function(text, name):
    """Source text of `name`'s definition (signature + body), or None."""
    t = strip_comments(text)
    for m in re.finditer(r"\b%s\s*\(" % re.escape(name), t):
        i = m.end()
        depth = 1
        while i < len(t) and depth:
            depth += {"(": 1, ")": -1}.get(t[i], 0)
            i += 1
        j = i
        while j < len(t) and t[j].isspace():
            j += 1
        if j < len(t) and t[j] == "{":
            depth, k = 0, j
            while k < len(t):
                if t[k] == "{":
                    depth += 1
                elif t[k] == "}":
                    depth -= 1
                    if depth == 0:
                        break
                k += 1
            s = max(t.rfind(";", 0, m.start()), t.rfind("}", 0, m.start()))
            return t[s + 1:k + 1]
    return None


_IDENT = re.compile(r"[A-Za-z_]\w*")
_KEYWORDS = {
    "unsigned", "signed", "int", "char", "short", "long", "const", "volatile",
    "struct", "union", "enum", "void", "sizeof", "return", "if", "while", "for",
    "do", "else", "goto", "switch", "case", "default", "break", "continue",
    "register", "static", "extern", "inline", "auto",
}


_STRING_RX = re.compile(r'"(?:\\.|[^"\\])*"')
_TOK = re.compile(r"[A-Za-z_]\w*|\d\w*|\S")
_QUAL = {"const", "volatile", "unsigned", "signed", "register", "static"}
_STD = re.compile(r"^(?:[usv]{1,2}\d+|[us]?[bB]ool)$")


def guess_typedefs(src):
    """Type names used in the text, found from token shapes."""
    toks = _TOK.findall(_STRING_RX.sub('""', src))
    names = set()
    isid = lambda t: bool(re.match(r"[A-Za-z_]", t)) and t not in _KEYWORDS
    n = len(toks)
    for i, t in enumerate(toks):
        if not isid(t):
            continue
        prev = toks[i - 1] if i else ";"
        nxt = toks[i + 1] if i + 1 < n else ""
        if prev in ("struct", "union", "enum"):
            continue
        # `T x`, `T *x`, `T **x`, also after qualifiers
        j = i + 1
        while j < n and toks[j] in ("*", "const", "volatile"):
            j += 1
        if j < n and isid(toks[j]) and (j > i + 1 or isid(nxt)) and                 toks[j] not in _QUAL:
            if j == i + 1 or prev in (";", "{", "}", ",", "(", "const", "volatile",
                                     "unsigned", "static", "register"):
                after = toks[j + 1] if j + 1 < n else ""
                if j == i + 1 or after in ("=", ";", ",", "[", ")"):
                    names.add(t)
        # casts / sizeof: `(T)` and `(T *)`
        if prev == "(" and j < n and toks[j] == ")" and (
                _STD.match(t) or (j > i + 1)):
            names.add(t)
    return names


def parse_function(src):
    """pycparser FuncDef for the text, inventing typedefs for unknown types."""
    src = re.sub(r"__attribute__\s*\(\(.*?\)\)", "", src)
    # inline asm statements (compiler barriers) and register pins carry no C semantics here
    src = re.sub(r"\b(?:__asm__|asm)\s*(?:volatile\s*)?\((?:[^()]|\([^()]*\))*\)\s*;", ";", src)
    src = re.sub(r'\basm\s*\(\s*"[^"]*"\s*\)', "", src)
    head, brace, rest = src.partition("{")
    src = re.sub(r"\b(?:static|inline|extern|__inline)\b", " ", head) + brace + rest
    typedefs = guess_typedefs(src)
    for _ in range(60):
        prelude = "".join("typedef int %s;" % t for t in sorted(typedefs))
        text = prelude + "\n" + src
        try:
            ast = CParser().parse(text)
        except c_parser.ParseError as e:
            m = re.match(r":(\d+):(\d+): before: (\S+)", str(e))
            if not m:
                return None
            line, col = int(m.group(1)), int(m.group(2))
            lines = text.split("\n")
            before = lines[line - 1][:max(col - 1, 0)] if line <= len(lines) else ""
            ids = _IDENT.findall(before)
            cand = ids[-1] if ids else None
            if cand is None or cand in typedefs or cand in _KEYWORDS:
                return None
            typedefs.add(cand)
            continue
        except Exception:
            return None
        for ext in ast.ext:
            if isinstance(ext, c_ast.FuncDef):
                return ext
        return None
    return None


# ------------------------------------------------------------------ features

MULDIV = {"*", "/", "%"}
SHIFT = {"<<", ">>"}
BITMASK = {"&", "|", "^", "~"}
WIDE = {"s64", "u64", "double", "float"}


def const_value(node):
    if isinstance(node, c_ast.Constant) and node.type in ("int", "char"):
        v = node.value.rstrip("uUlL")
        try:
            if node.type == "char":
                return ord(eval(v)) if len(v) > 2 else 0
            return int(v, 0)
        except (ValueError, SyntaxError, TypeError):
            try:
                return int(v, 8)
            except ValueError:
                return None
    if isinstance(node, c_ast.UnaryOp) and node.op == "-":
        v = const_value(node.expr)
        return None if v is None else -v
    if isinstance(node, c_ast.Cast):
        return const_value(node.expr)
    return None


def type_names(t):
    """(identifier/keyword names, qualifiers) of a type node."""
    names, quals = [], []
    while t is not None:
        if isinstance(t, (c_ast.TypeDecl, c_ast.PtrDecl)):
            quals += t.quals or []
            t = t.type
        elif isinstance(t, c_ast.ArrayDecl):
            quals += t.dim_quals or []
            t = t.type
        elif isinstance(t, c_ast.IdentifierType):
            names += t.names
            t = None
        elif isinstance(t, (c_ast.Struct, c_ast.Union, c_ast.Enum)):
            names.append("struct " + str(t.name))
            t = None
        elif isinstance(t, c_ast.FuncDecl):
            t = t.type
        else:
            t = None
    return names, quals


def _ptr_depth(t):
    d = 0
    while isinstance(t, (c_ast.PtrDecl, c_ast.TypeDecl, c_ast.ArrayDecl)):
        if isinstance(t, c_ast.PtrDecl):
            d += 1
        t = t.type
    return d


class Feat:
    def __init__(self):
        self.volatile = collections.Counter()      # what -> count
        self.volatile_total = 0
        self.callees = collections.Counter()
        self.ops = collections.Counter()
        self.lits = collections.Counter()
        self.wide = collections.Counter()
        self.loop_assign = collections.Counter()   # (var, kind) -> n
        self.assigned = collections.Counter()      # var -> assignment count
        self.reads = collections.Counter()         # var -> read count
        self.locals = set()
        self.names = set()                         # every identifier seen
        self.noinit = set()                        # scalar locals declared without a value
        self.seen = set()                          # ... whose first use has been passed
        self.early_reads = set()                   # ... first used by READING, outside a loop
        self.has_label = False
        self.noops = 0                             # `x += 0;` / `x = x;` statements
        self.unreachable = 0                       # statements after a return/goto/break
        self.wide_vars = set()                     # declared with a wide type
        self.subscripts = set()                    # identifiers used inside [ ]
        self.params = []
        self.events = []                           # calls and stores, in order


def _note_type(f, t, where, quals_extra=()):
    nm, q = type_names(t)
    q = list(q) + list(quals_extra)
    if "volatile" in q:
        f.volatile[where] += 1
        f.volatile_total += 1
    wide = False
    for n in nm:
        if n in WIDE:
            f.wide[n] += 1
            wide = True
    if nm.count("long") >= 2:
        f.wide["long long"] += 1
        wide = True
    if wide and where.startswith(("local:", "param:")):
        f.wide_vars.add(where.split(":", 1)[1])
    return nm


def analyze(fd):
    f = Feat()
    f.helpers = set()
    ft = fd.decl.type
    for p in (ft.args.params if ft.args else []):
        if not isinstance(p, c_ast.Decl):
            continue
        nm = _note_type(f, p.type, "param:%s" % p.name, p.quals or [])
        f.params.append(" ".join(nm) + "*" * _ptr_depth(p.type))
    _walk(fd.body, f, False, True)
    f.names |= set(f.assigned) | set(f.reads) | f.locals
    f.names |= {p.name for p in (ft.args.params if ft.args else [])
                if isinstance(p, c_ast.Decl) and p.name}
    return f


def _ids(node):
    if isinstance(node, c_ast.ID):
        yield node.name
    for _, ch in node.children():
        yield from _ids(ch)


_GEN = c_generator.CGenerator()


def _same_expr(a, b):
    try:
        return _GEN.visit(a) == _GEN.visit(b)
    except Exception:
        return False


def _lname(node):
    return node.name if isinstance(node, c_ast.ID) else None


def _walk(node, f, in_loop, top):
    """`top`: the value of this expression is discarded (statement position)."""
    if node is None:
        return
    t = type(node)

    if t is c_ast.Decl:
        if isinstance(node.type, c_ast.FuncDecl):
            return
        _note_type(f, node.type, "local:%s" % node.name, node.quals or [])
        if node.name:
            f.locals.add(node.name)
            scalar = isinstance(node.type, c_ast.PtrDecl) or (
                isinstance(node.type, c_ast.TypeDecl)
                and isinstance(node.type.type, c_ast.IdentifierType))
            if scalar and node.init is None and "static" not in (node.storage or []):
                f.noinit.add(node.name)
        if node.init is not None:
            if node.name:
                f.assigned[node.name] += 1
                if in_loop:
                    kind = "const" if const_value(node.init) is not None else "expr"
                    f.loop_assign[(node.name, kind)] += 1
            _walk(node.init, f, in_loop, False)
        return

    if t is c_ast.Cast:
        _note_type(f, node.to_type.type, "cast", node.to_type.quals or [])
        _walk(node.expr, f, in_loop, False)
        return

    if t is c_ast.FuncCall:
        nm = node.name.name if isinstance(node.name, c_ast.ID) else "*indirect"
        f.callees[nm] += 1
        f.events.append("call:" + nm)
        if not isinstance(node.name, c_ast.ID):
            _walk(node.name, f, in_loop, False)
        if node.args:
            for a in node.args.exprs:
                _walk(a, f, in_loop, False)
        return

    if t is c_ast.Assignment:
        name = _lname(node.lvalue)
        op = node.op
        rv = const_value(node.rvalue)
        if top and ((rv == 0 and op in ("+=", "-=", "|=", "^=", "<<=", ">>="))
                    or (rv == 1 and op in ("*=", "/="))
                    or (op == "=" and _same_expr(node.lvalue, node.rvalue))):
            f.noops += 1
        if op != "=":
            f.ops[op[:-1]] += 1
        if name is not None:
            f.assigned[name] += 1
            if op != "=" or not top:
                f.reads[name] += 1
            if in_loop:
                if not top:
                    kind = "nested"
                elif op != "=":
                    kind = "inc"
                elif const_value(node.rvalue) is not None:
                    kind = "const"
                else:
                    kind = "expr"
                f.loop_assign[(name, kind)] += 1
        else:
            f.events.append("store")
            _walk(node.lvalue, f, in_loop, False)
        _walk(node.rvalue, f, in_loop, False)
        if name is not None:
            f.seen.add(name)        # only now: `x = f(x)` reads x first
        return

    if t is c_ast.UnaryOp:
        if node.op in ("p++", "p--", "++", "--"):
            name = _lname(node.expr)
            if name is not None:
                if name in f.noinit and name not in f.seen:
                    f.seen.add(name)
                    if not in_loop:
                        f.early_reads.add(name)
                f.assigned[name] += 1
                f.reads[name] += 1
                if in_loop:
                    f.loop_assign[(name, "inc" if top else "nested")] += 1
                return
        if node.op in ("~", "!"):
            f.ops[node.op] += 1
        if node.op == "sizeof":
            return
        _walk(node.expr, f, in_loop, False)
        return

    if t is c_ast.BinaryOp:
        f.ops[node.op] += 1
        _walk(node.left, f, in_loop, False)
        _walk(node.right, f, in_loop, False)
        return

    if t is c_ast.Constant:
        v = const_value(node)
        if v is not None:
            f.lits[v] += 1
        return

    if t is c_ast.ID:
        f.reads[node.name] += 1
        f.names.add(node.name)
        if node.name in f.noinit and node.name not in f.seen:
            f.seen.add(node.name)
            if not in_loop:
                f.early_reads.add(node.name)
        return

    if t in (c_ast.Label, c_ast.Goto):
        f.has_label = True

    if t is c_ast.ArrayRef:
        for n in _ids(node.subscript):
            f.subscripts.add(n)

    if t is c_ast.ExprList:
        for e in node.exprs:
            _walk(e, f, in_loop, top)
        return

    if t is c_ast.For:
        _walk(node.init, f, in_loop, True)
        _walk(node.cond, f, True, False)
        _walk(node.next, f, True, True)
        _walk(node.stmt, f, True, True)
        return

    if t in (c_ast.While, c_ast.DoWhile):
        _walk(node.cond, f, True, False)
        _walk(node.stmt, f, True, True)
        return

    if t is c_ast.Compound:
        prev = None
        for it in node.block_items or []:
            if isinstance(prev, (c_ast.Return, c_ast.Goto, c_ast.Break, c_ast.Continue))                     and not isinstance(it, (c_ast.Label, c_ast.Case, c_ast.Default)):
                f.unreachable += 1
            _walk(it, f, in_loop, True)
            prev = it
        return

    if t is c_ast.If:
        _walk(node.cond, f, in_loop, False)
        _walk(node.iftrue, f, in_loop, True)
        _walk(node.iffalse, f, in_loop, True)
        return

    if t is c_ast.TernaryOp:
        _walk(node.cond, f, in_loop, False)
        _walk(node.iftrue, f, in_loop, False)
        _walk(node.iffalse, f, in_loop, False)
        return

    stmt_container = t in (c_ast.Switch, c_ast.Case, c_ast.Default, c_ast.Label)
    for _, child in node.children():
        _walk(child, f, in_loop, stmt_container)


# ---------------------------------------------------------------- comparison

def dead_locals(f):
    """Locals assigned but never read."""
    return {n for n in f.locals if f.assigned.get(n, 0) and not f.reads.get(n, 0)}


def _group(d, group):
    return {k: v for k, v in d.items() if k in group}


def compare(b, c):
    """List of (rule, message) findings for candidate features c vs base b."""
    out = []

    if c.volatile_total > b.volatile_total:
        new = sorted(k for k in c.volatile if c.volatile[k] > b.volatile[k])
        out.append(("volatile", "volatile added (%s)" % ", ".join(new or ["?"])))

    if c.callees != b.callees:
        new, gone = c.callees - b.callees, b.callees - c.callees
        parts = []
        if new:
            parts.append("added " + ", ".join("%s x%d" % kv for kv in sorted(new.items())))
        if gone:
            parts.append("removed " + ", ".join("%s x%d" % kv for kv in sorted(gone.items())))
        out.append(("callees", "callee multiset changed: " + "; ".join(parts)))
    elif [e for e in c.events if e != "store"] != [e for e in b.events if e != "store"]:
        out.append(("call-order", "calls appear in a different order"))
    elif _call_store_shape(b.events) != _call_store_shape(c.events):
        out.append(("call-store", "a call moved across a store to memory"))

    for name, group, rule in (("multiply/divide", MULDIV, "ops-muldiv"),
                              ("shift", SHIFT, "ops-shift"),
                              ("bit-mask", BITMASK, "ops-bitmask")):
        bo, co = _group(b.ops, group), _group(c.ops, group)
        if bo != co:
            out.append((rule, "%s operators changed: %s -> %s"
                        % (name, _fmt(bo), _fmt(co))))
    other = set("+ - < <= > >= == != && || !".split())
    bo, co = _norm_cmp(_group(b.ops, other)), _norm_cmp(_group(c.ops, other))
    if bo != co:
        out.append(("ops-other", "operators changed: %s -> %s" % (_fmt(bo), _fmt(co))))

    if b.lits != c.lits:
        out.append(("literals", "integer literals changed: -[%s] +[%s]"
                    % (_fmt(b.lits - c.lits, True), _fmt(c.lits - b.lits, True))))

    for k in sorted(set(c.wide)):
        if c.wide[k] > b.wide[k]:
            idx = sorted((c.wide_vars - b.wide_vars) & c.subscripts)
            if idx:
                out.append(("wide-type", "new wide type %s used as an index (%s)"
                            % (k, ", ".join(idx))))
            else:
                out.append(("wide-type-temp", "new wide type %s" % k))

    for (var, kind), n in sorted(c.loop_assign.items()):
        bn = b.loop_assign.get((var, kind), 0)
        if var not in b.names:
            continue            # a fresh temp cannot clobber anything
        if n > bn and kind == "const":
            out.append(("loop-const", "%s assigned a constant inside a loop "
                        "(base %d such, now %d)" % (var, bn, n)))
        elif n > bn and kind == "nested":
            out.append(("loop-nested", "%s assigned/stepped inside an expression "
                        "in a loop (base %d, now %d)" % (var, bn, n)))

    if not c.has_label:
        late = sorted(c.early_reads - b.early_reads)
        if late:
            out.append(("uninit-textual", "%s is read before any assignment to it appears "
                        "in the source" % ", ".join(late)))

    if c.noops > b.noops:
        out.append(("noop-stmt", "%d statement(s) that change nothing (`x += 0;`, `x = x;`) "
                    "added" % (c.noops - b.noops)))

    if c.unreachable > b.unreachable:
        out.append(("unreachable", "%d statement(s) after a return/goto/break can never run"
                    % (c.unreachable - b.unreachable)))

    extra = dead_locals(c) - dead_locals(b)
    if extra:
        out.append(("dead-local", "assigned but never read: " + ", ".join(sorted(extra))))

    if c.params != b.params:
        out.append(("params", "parameter list changed: (%s) -> (%s)"
                    % (", ".join(b.params), ", ".join(c.params))))
    return out


def _call_store_shape(events):
    shape, stores = [], 0
    for e in events:
        if e == "store":
            stores += 1
        else:
            shape.append((e, stores))
    return shape


_SWAP = {">": "<", ">=": "<="}


def _norm_cmp(d):
    r = collections.Counter()
    for k, v in d.items():
        r[_SWAP.get(k, k)] += v
    return r


def _fmt(d, lits=False):
    def key(k):
        return hex(k) if lits and isinstance(k, int) and abs(k) > 9 else str(k)
    return ",".join(key(k) + ("x%d" % v if v > 1 else "")
                    for k, v in sorted(d.items(), key=lambda kv: str(kv[0]))) or "-"


# ------------------------------------------------------------------ driver

_CACHE = {}


def features_of(path, fn):
    key = (os.path.abspath(path), fn, os.path.getmtime(path))
    if key not in _CACHE:
        _CACHE[key] = _features_of(path, fn)
    return _CACHE[key]


def _features_of(path, fn):
    text = read_text(path)
    src = find_function(text, fn)
    if src is None:
        return None, "%s not found in %s" % (fn, path)
    fd = parse_function(src)
    if fd is None:
        return None, "could not parse %s in %s" % (fn, path)
    f = analyze(fd)
    # A callee defined in the same file (the permuter's `inline_fn`) is not
    # an external call: its body is this function's own code.
    for name in list(f.callees):
        if name != fn and find_function(text, name) is not None:
            f.helpers.add(name)
            del f.callees[name]
    f.events = [e for e in f.events if not (e.startswith("call:")
                                            and e[5:] in f.helpers)]
    return f, None


def check(fn, cand_path, base_path):
    """(findings, skip_reason); findings is a list of (rule, message)."""
    bf, err = features_of(base_path, fn)
    if bf is None:
        return [], err
    cf, err = features_of(cand_path, fn)
    if cf is None:
        return [], err
    return compare(bf, cf), None


def default_base(fn, cand_path):
    """The file a candidate was derived from, when its name says so."""
    d = os.path.dirname(os.path.abspath(cand_path))
    stem = os.path.basename(cand_path)
    names = []
    m = re.match(r"(.*perm\d+)(?:-[^.]*)?\.c(?:\.wrongc\d?)?$", stem)
    if m:
        names.append(m.group(1) + "-start.c")
    names.append(fn + ".c")
    for n in names:
        p = os.path.join(d, n)
        if os.path.isfile(p) and os.path.abspath(p) != os.path.abspath(cand_path):
            return p
    return os.path.join(WORK, fn, fn + ".c")


def verdict(findings):
    return [("WRONG" if r in RULES_WRONG else "WARN", r, m) for r, m in findings]


def rejects(fn, cand, base, emu=True, seconds=2.0, only=None):
    """For permute.py / drafts.py: a reason string if the candidate is WRONG
    C relative to the base, else None. Never raises: a failure of this tool
    must not stop a search."""
    try:
        findings, _ = analyze_all(fn, cand, base, emu=emu, seconds=seconds)
    except Exception:
        return None
    bad = ["%s: %s" % (r, m) for r, m in findings
           if r in RULES_WRONG and (only is None or r in only)]
    return "; ".join(bad) if bad else None


def emu_findings(fn, cand, base, seconds=3.0):
    """Differential-testing findings and notes (see wrongc_emu.py)."""
    sys.path.insert(0, HERE)
    import wrongc_emu
    r = wrongc_emu.emu_check(fn, base, cand, seconds=seconds)
    f, notes = [], []
    if r["verdict"] == "skip":
        notes.append("emu: not run (%s)" % r.get("detail"))
        return f, notes
    stat = "%d seeds, %d finished, %s%% of the function's code reached" % (
        r["seeds"], r["conclusive"], r.get("cov_pct"))
    args_txt = lambda a: "(" + ", ".join("0x%X" % (x & 0xFFFFFFFF) for x in a) + ")"
    if r.get("diff"):
        seed, args, why = r["diff"]
        f.append(("behaviour", "%s (seed %d, args %s; differs on %d of %d runs)" % (
            why, seed, args_txt(args), r.get("ndiff", 0), r["conclusive"])))
    if r.get("uninit"):
        seed, args, why = r["uninit"]
        f.append(("uninit-dynamic", "the candidate's result changes with the garbage in "
                  "registers/frame it never set: %s (seed %d; %d of %d runs)" % (
                      why, seed, r["nuninit"], r["conclusive"])))
    if not f:
        notes.append("emu: same behaviour on %s" % stat)
    elif r.get("nbaseub"):
        notes.append("emu: the base itself reads uninitialised data on %d runs (not judged)"
                     % r["nbaseub"])
    if r.get("weak"):
        f.append(("frame-data", r["weak"][2]))
    return f, notes


def analyze_all(fn, cand, base, emu=True, uninit=False, seconds=3.0):
    """(findings, notes): pattern rules, then differential testing."""
    findings, notes = [], []
    pat, skip = check(fn, cand, base)
    findings += pat
    if skip:
        notes.append("patterns: not run (%s)" % skip)
    if emu:
        try:
            f, n = emu_findings(fn, cand, base, seconds)
        except Exception as e:      # an interpreter gap must never look like a verdict
            f, n = [], ["emu: failed (%s: %s)" % (type(e).__name__, e)]
        findings += f
        notes += n
    if uninit:
        sys.path.insert(0, HERE)
        import agbenv
        bad = sorted(set(agbenv.uninitialized_reads(cand, fn=fn))
                     - set(agbenv.uninitialized_reads(base, fn=fn)))
        if bad:
            RULES_WRONG.add("uninit")
            findings.append(("uninit", "may read %s before setting it" % ", ".join(bad)))
    return findings, notes


def main(argv=None):
    doc = __doc__.split("\n\n", 1)
    ap = argparse.ArgumentParser(description=doc[0], epilog=doc[1],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("fn", nargs="?")
    ap.add_argument("candidate", nargs="?")
    ap.add_argument("--base", help="base file (default: derived-from file, else work/<fn>/<fn>.c)")
    ap.add_argument("--no-emu", action="store_true",
                    help="pattern rules only (no compile, instant)")
    ap.add_argument("--emu-seconds", type=float, default=3.0,
                    help="time budget for the differential run (default 3)")
    ap.add_argument("--uninit", action="store_true",
                    help="also run the compile-based read-before-set check")
    ap.add_argument("--self-test", action="store_true")
    a = ap.parse_args(argv)
    if a.self_test:
        return self_test(emu=not a.no_emu)
    if not a.fn or not a.candidate:
        ap.error("need <fn> <candidate.c>")
    base = a.base or default_base(a.fn, a.candidate)
    findings, notes = analyze_all(a.fn, a.candidate, base, emu=not a.no_emu,
                                  uninit=a.uninit, seconds=a.emu_seconds)
    print("base: %s" % os.path.relpath(base, REPO))
    v = verdict(findings)
    for kind, r, m in v:
        print("%s: %s: %s" % (kind, r, m))
    for n in notes:
        print("note: " + n)
    if not any(k == "WRONG" for k, _, _ in v):
        print("OK" if not v else "OK (warnings only)")
    return 1 if any(k == "WRONG" for k, _, _ in v) else 0


CORPUS = os.path.join(HERE, "wrongc_corpus")

SYNTH_BASE = """
void f(int a, u8 *p)
{
    int i;
    int n;
    n = a * 3;
    for (i = 0; i < n; i++)
    {
        p[i] = a >> 2;
        g(i);
    }
}
"""

# (name, text transform, rule that must fire, or None when the edit is harmless)
SYNTH = [
    ("volatile local", lambda t: t.replace("int n;", "volatile int n;"), "volatile"),
    ("counter reset in its loop", lambda t: t.replace("g(i);", "g(i);\n        i = 0;"), "loop-const"),
    ("counter stepped in an expression", lambda t: t.replace("p[i] =", "p[i++] ="), "loop-nested"),
    ("call dropped", lambda t: t.replace("        g(i);\n", ""), "callees"),
    ("call added", lambda t: t.replace("g(i);", "g(i);\n        h(i);"), "callees"),
    ("wide index", lambda t: t.replace("int i;", "long long i;"), "wide-type"),
    ("dead local", lambda t: t.replace("int n;", "int n;\n    int z;\n    z = 5;"), "dead-local"),
    ("read before any assignment",
     lambda t: t.replace("    n = a * 3;", "    p[0] = q;\n    q = 1;\n    n = a * 3;")
     .replace("int n;", "int n;\n    int q;"), "uninit-textual"),
    ("code after return", lambda t: t.replace("g(i);", "return;\n        g(i);"), "unreachable"),
    ("harmless: operands swapped", lambda t: t.replace("a * 3", "3 * a"), None),
    ("harmless: extra temp",
     lambda t: t.replace("n = a * 3;", "t = a;\n    n = t * 3;").replace("int n;", "int n;\n    int t;"), None),
    ("harmless: fresh temp assigned inside an expression",
     lambda t: t.replace("p[i] = a >> 2;", "p[i] = (t = a) >> 2;").replace("int n;", "int n;\n    int t;"), None),
]


def _synthetic_tests():
    d = os.path.join(REPO, "build", "wrongc", "selftest")
    os.makedirs(d, exist_ok=True)
    base_path = os.path.join(d, "base.c")
    with open(base_path, "w", encoding="utf-8") as fh:
        fh.write(SYNTH_BASE)
    ok = True
    for name, edit, want in SYNTH:
        cand = os.path.join(d, "cand.c")
        with open(cand, "w", encoding="utf-8") as fh:
            fh.write(edit(SYNTH_BASE))
        findings, skip = check("f", cand, base_path)
        rules = {r for r, _ in findings if r in RULES_WRONG}
        good = (want in rules) if want else not rules
        if skip:
            good = False
        print("[self-test] %-46s %s%s" % (name, "PASS" if good else "FAIL",
                                          "" if good else "  got %s" % (sorted(rules) or skip)))
        ok = ok and good
    findings, _ = check("f", base_path, base_path)
    print("[self-test] %-46s %s" % ("a file against itself", "PASS" if not findings else "FAIL"))
    return ok and not findings


def _corpus_run(emu, seconds):
    """Per-rule benchmark over tools/wrongc_corpus. Returns (ok, table text)."""
    man = os.path.join(CORPUS, "manifest.json")
    if not os.path.exists(man):
        return True, "no corpus at %s; skipped" % os.path.relpath(CORPUS, REPO)
    import json
    m = json.load(open(man, encoding="utf-8"))
    hit_rules = collections.Counter()
    wrong_hits = wrong_n = pat_hits = emu_hits = 0
    misses = []
    for e in m["wrong"]:
        cand, base = (os.path.join(CORPUS, e[k].replace("/", os.sep)) for k in ("cand", "base"))
        findings, _ = analyze_all(e["fn"], cand, base, emu=emu, seconds=seconds)
        rules = {r for r, _ in findings if r in RULES_WRONG}
        wrong_n += 1
        pat = {r for r in rules if r not in ("behaviour", "uninit-dynamic")}
        dyn = rules - pat
        if rules:
            wrong_hits += 1
        pat_hits += bool(pat)
        emu_hits += bool(dyn)
        for r in rules:
            hit_rules[r] += 1
        if not rules:
            misses.append("%s/%s" % (e["fn"], e["cand"].split("/")[-1]))
    fp_files, fp_rules = 0, collections.Counter()
    for e in m["good"]:
        cand, base = (os.path.join(CORPUS, e[k].replace("/", os.sep)) for k in ("cand", "base"))
        findings, _ = analyze_all(e["fn"], cand, base, emu=emu, seconds=seconds)
        rules = {r for r, _ in findings if r in RULES_WRONG}
        if rules:
            fp_files += 1
        for r in rules:
            fp_rules[r] += 1
    lines = ["wrong-C corpus: %d files; flagged %d (%.0f%%); by patterns %d, by emulator %d%s" % (
        wrong_n, wrong_hits, 100.0 * wrong_hits / max(1, wrong_n), pat_hits, emu_hits,
        "" if emu else " (emulator not run)")]
    lines.append("  hits per rule: " + ", ".join("%s %d" % kv for kv in sorted(hit_rules.items())))
    lines.append("  good permuter outputs flagged WRONG: %d of %d %s" % (
        fp_files, len(m["good"]), dict(fp_rules) if fp_rules else ""))
    lines.append("  not flagged: " + (", ".join(misses) if misses else "-"))
    floor = m.get("floor", {})
    ok = True
    need = floor.get("pattern" if not emu else "any", 0)
    got = pat_hits if not emu else wrong_hits
    if got < need:
        ok = False
        lines.append("  FAIL: recall fell to %d, the recorded floor is %d" % (got, need))
    if fp_files > floor.get("good_flagged_max", 0):
        ok = False
        lines.append("  FAIL: %d good outputs flagged, the recorded ceiling is %d"
                     % (fp_files, floor.get("good_flagged_max", 0)))
    return ok, "\n".join(lines)


def self_test(emu=True):
    ok = _synthetic_tests()
    c_ok, text = _corpus_run(emu, 2.0)
    print(text)
    ok = ok and c_ok
    print("[self-test] %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
