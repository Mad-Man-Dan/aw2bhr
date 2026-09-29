#!/usr/bin/env python3
"""Enumerate semantics-preserving rewrites of a parked draft and score each one.

Wave 96/97 matches came from a few small respellings applied at the right site,
found by hand. This finds them by search: parse the draft's function, list every
site where a known rewrite applies, compile one candidate per (rule, site), and
rank by the project's own score. Then compose the best singles in pairs.

    python tools/levers.py sub_0806412C                    # search one draft
    python tools/levers.py sub_X --draft work/sub_X/old.c  # search another file
    python tools/levers.py sub_X --rules 1,2,4a --seconds 120 --max 300
    python tools/levers.py --all-parked --seconds 150      # every parked draft
    python tools/levers.py --self-test                     # rediscover the wave-97 matches

Rules (a number selects all its letters; --rules 1,4a):

  1a  factor/expand a scaled sum:  i * 16 + 0x30  ->  (i * 2 + 6) * 8    (and back)
  1b  x + 1 -> -~x,  x - 1 -> ~-x
  1c  copy a local into a fresh block-scoped local of the same (1c) or the opposite
      (1c') signedness for ONE use:  { s16 tn = t; ...tn... }
  1d  re-associate / re-order a + or - chain, or a * | & ^ pair (r + t + K <-> t + (r + K))
  2   flip the signedness of a local or parameter (u16 x -> s16 x)
  3a  loop step through a copy: for (..; ..; ix = nx) { nx = ix + 1; ... }
  3b  the same as a while loop, nx computed at the top, `ix = nx;` at the bottom
  3c  the same with nx computed at the bottom
  3d  3a, and every `ix + 1` in the body becomes nx
  3e  a statement `ix++;` -> `nx = ix + 1; ix = nx;`
  4a  swap the arms of if/else (test inverted), or `if (c) return X; return Y;`
  4b  comparison spelling: mirror (a < b -> b > a), `x != 0` <-> `x`, `!x` <-> `x == 0`
  4c  `<= K` <-> `< K+1` and the other three
  5a  split a subexpression into its own statement `lv = E;` (fresh local, typeof E)
  5b  the same into an ALREADY-DECLARED int-family local (needs wrongc)
  5c  inline a local that is assigned once and read once
  5d  do { ... } while (0) around a block
  5f  5a for every identical copy of E in the statement at once
  7   write the linker ALIAS of a global (aw2bhr.lds `a = b;`) at one use: the compiler
      sees two symbols, so cse stops reusing an earlier address load there

Each candidate is written to build/levers/<fn>/, compiled and scored on the
project's path (tools/drafts.py: the Makefile recipe plus the function's compiler
override). Nothing in work/ is overwritten: an IMPROVING candidate is written to
work/<fn>/levers/<rule>-<site>.c (site = the node number in the original tree;
levers/index.txt says what each one is) and gets a tools/wrongc.py verdict.

Honesty: every rewrite is exact for two's-complement int arithmetic and for the
ordinary evaluation of pure expressions. The rewrites that are exact only under a
range or liveness condition (1c', 2, 5b, 5c on a narrow local, 5a in a loop
that changes E) are tagged `?` in the table; the wrongc differential run is the
judge for those, and it runs on every improving candidate (the best --wrongc-max).
A candidate that reports MATCH is byte-exact by tools/drafts.py's arithmetic,
which is trymatch's; copy it over the draft and run trymatch to confirm.

Exit status: 0 if any candidate MATCHes, 1 otherwise, 2 on a usage error.
"""

import argparse
import concurrent.futures
import copy
import hashlib
import itertools
import json
import os
import re
import shutil
import sys
import time

from pycparser import c_ast, c_generator

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import awlib  # noqa: E402
import drafts  # noqa: E402
import wrongc  # noqa: E402

REPO = awlib.REPO
OUT = "build/levers"
GEN = c_generator.CGenerator()
RULES_ON = {"7"}
FORCE_ALIAS = False

RULE_IDS = ["1a", "1b", "1c", "1d", "2", "3a", "3b", "3c", "3d", "3e",
            "4a", "4b", "4c", "5a", "5b", "5c", "5d", "5f", "7"]


def G(node):
    return GEN.visit(node)


# --------------------------------------------------------------- locating

_COMMENT_RX = wrongc._COMMENT_RX


def blank_keep_len(text):
    """Comments and preprocessor lines blanked to spaces, every offset kept."""
    def blank(m):
        t = m.group(0)
        return re.sub(r"[^\n]", " ", t) if t[0] == "/" else t
    t = _COMMENT_RX.sub(blank, text)
    out, cont = [], False
    for ln in t.split("\n"):
        if cont or ln.lstrip().startswith("#"):
            cont = ln.rstrip().endswith("\\")
            out.append(" " * len(ln))
        else:
            out.append(ln)
    return "\n".join(out)


def locate(blank, name):
    """(head_start, body_open, body_close) offsets of name's definition, or None."""
    for m in re.finditer(r"\b%s\s*\(" % re.escape(name), blank):
        i, depth = m.end(), 1
        while i < len(blank) and depth:
            depth += {"(": 1, ")": -1}.get(blank[i], 0)
            i += 1
        j = i
        while j < len(blank) and blank[j].isspace():
            j += 1
        if j < len(blank) and blank[j] == "{":
            depth, k = 0, j
            while k < len(blank):
                if blank[k] == "{":
                    depth += 1
                elif blank[k] == "}":
                    depth -= 1
                    if depth == 0:
                        break
                k += 1
            s = max(blank.rfind(";", 0, m.start()), blank.rfind("}", 0, m.start())) + 1
            return s, j, k
    return None


class Source:
    """A draft file with the function's body swappable."""

    def __init__(self, fn, path):
        self.fn, self.path = fn, path
        self.text = wrongc.read_text(path)
        blank = blank_keep_len(self.text)
        loc = locate(blank, fn)
        if loc is None:
            raise SystemExit("error: %s is not defined in %s" % (fn, path))
        self.hs, self.bo, self.bc = loc
        self.head = self.text[self.hs:self.bo]
        nm = re.search(r"\b%s\s*\(" % re.escape(fn), blank[self.hs:self.bo])
        self.decl_off = (self.text.rfind(chr(10), 0, self.hs + nm.start()) + 1 - self.hs) if nm else 0
        self.decl_off = max(self.decl_off, 0)
        fbody = blank[self.bo:self.bc + 1]
        if re.search(r"\b(?:asm|__asm__)\b", fbody):
            raise SystemExit("error: %s contains inline asm; the tree rewrite would drop it" % fn)
        self.typeofs = {}
        self.fd = wrongc.parse_function(self._protect(blank[self.hs:self.bc + 1]))
        if self.fd is None:
            raise SystemExit("error: %s in %s does not parse" % (fn, path))
        self.params = []
        args = self.fd.decl.type.args
        if args is not None:
            self.params = [p for p in args.params if isinstance(p, c_ast.Decl)]

    def _protect(self, txt):
        """`__typeof__(E)` cannot be parsed: stand in a type name, put the text back in render()."""
        out, i = [], 0
        while True:
            m = re.compile(r"__typeof__\s*\(").search(txt, i)
            if not m:
                out.append(txt[i:])
                break
            j, d = m.end(), 1
            while j < len(txt) and d:
                d += {"(": 1, ")": -1}.get(txt[j], 0)
                j += 1
            k = "TYPEOF_%d" % len(self.typeofs)
            self.typeofs[k] = txt[m.start():j]
            out.append(txt[i:m.start()] + k)
            i = j
        return "".join(out)

    def _restore(self, txt):
        for k, v in self.typeofs.items():
            txt = re.sub(r"\b%s\b" % k, lambda _m, v=v: v, txt)
        return txt

    def render(self, body, head_subs=()):
        head, pre = self.head, ""
        for rx, new in head_subs:
            if rx is None and new not in pre:
                pre += new
        head = head[:self.decl_off] + pre + head[self.decl_off:]
        for rx, new in head_subs:
            if rx is not None:
                head = re.sub(rx, new, head, count=1)
        return self.text[:self.hs] + head + self._restore(G(body).strip()) + self.text[self.bc + 1:]


# ------------------------------------------------------------------ tree

def preorder(node, out):
    out.append(node)
    for _, c in node.children():
        preorder(c, out)
    return out


def _slot(name):
    m = re.match(r"(\w+)\[(\d+)\]$", name)
    return (m.group(1), int(m.group(2))) if m else (name, None)


CONTAINERS = ((c_ast.Compound, "block_items"), (c_ast.Case, "stmts"), (c_ast.Default, "stmts"))


def normalize(body):
    """Give every if/loop arm braces; braces never change agbcc's output."""
    def wrap(n):
        return n if isinstance(n, c_ast.Compound) else c_ast.Compound([n])
    for n in preorder(body, []):
        if isinstance(n, c_ast.If):
            if n.iftrue is not None:
                n.iftrue = wrap(n.iftrue)
            if n.iffalse is not None:
                n.iffalse = wrap(n.iffalse)
        elif isinstance(n, (c_ast.For, c_ast.While, c_ast.DoWhile)):
            if n.stmt is not None:
                n.stmt = wrap(n.stmt)
    if body.block_items is None:
        body.block_items = []
    for n in preorder(body, []):
        if isinstance(n, c_ast.Compound) and n.block_items is None:
            n.block_items = []
    return body


# int-family types
_SIGN = {"u8": ("s8", 8, False), "s8": ("u8", 8, True), "u16": ("s16", 16, False),
         "s16": ("u16", 16, True), "u32": ("s32", 32, False), "s32": ("u32", 32, True)}
_LONG = {("unsigned", "char"): ("char_s", 8, False), ("unsigned", "short"): ("short", 16, False),
         ("short",): ("unsigned short", 16, True), ("unsigned", "int"): ("int", 32, False),
         ("int",): ("unsigned int", 32, True), ("unsigned",): ("int", 32, False),
         ("signed", "char"): ("unsigned char", 8, True), ("short", "int"): ("unsigned short", 16, True),
         ("unsigned", "short", "int"): ("short", 16, False)}


def kind_of(decl):
    """('int', bits, signed, names) / ('ptr',) / ('arr',) / ('other',) for a Decl."""
    t = decl.type
    if isinstance(t, c_ast.PtrDecl):
        return ("ptr",)
    if isinstance(t, c_ast.ArrayDecl):
        return ("arr",)
    if not (isinstance(t, c_ast.TypeDecl) and isinstance(t.type, c_ast.IdentifierType)):
        return ("other",)
    names = tuple(n for n in t.type.names if n not in ("const", "register"))
    if "volatile" in names:
        return ("other",)
    if len(names) == 1 and names[0] in _SIGN:
        n = names[0]
        return ("int", _SIGN[n][1], n.startswith("s"), names)
    if names in _LONG:
        return ("int", _LONG[names][1], not _LONG[names][2], names)
    if names == ("int",):
        return ("int", 32, True, names)
    if names in (("char",), ("unsigned", "char"), ("signed", "char")):
        return ("int", 8, names != ("unsigned", "char") and names != ("char",), names)
    return ("other",)


def flipped_names(names):
    if len(names) == 1 and names[0] in _SIGN:
        return [_SIGN[names[0]][0]]
    if names in _LONG and _LONG[names][0] != "char_s":
        return _LONG[names][0].split()
    return None


def const_int(e):
    if isinstance(e, c_ast.Constant) and e.type in ("int", "unsigned int", "long int", "unsigned long int"):
        s = re.sub(r"[uUlL]+$", "", e.value)
        try:
            return int(s, 8) if re.match(r"0[0-7]+$", s) else int(s, 0)
        except ValueError:
            return None
    return None


def K(v):
    return c_ast.Constant("int", hex(v) if v >= 10 else str(v))


def BO(op, a, b):
    return c_ast.BinaryOp(op, a, b)


def UO(op, a):
    return c_ast.UnaryOp(op, a)


def ID(n):
    return c_ast.ID(n)


def mk_decl(name, tnames, init=None):
    td = c_ast.TypeDecl(name, [], None, c_ast.IdentifierType(list(tnames)))
    return c_ast.Decl(name, [], [], [], [], td, init, None)


def typeof_names(expr):
    return ["__typeof__(%s)" % G(expr)]


def is_pure(e):
    for n in preorder(e, []):
        if isinstance(n, (c_ast.FuncCall, c_ast.Assignment)):
            return False
        if isinstance(n, c_ast.UnaryOp) and n.op in ("p++", "p--", "++", "--"):
            return False
    return True


def names_in(e):
    return [n.name for n in preorder(e, []) if isinstance(n, c_ast.ID)]


def cmp_invert(op):
    return {"==": "!=", "!=": "==", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}.get(op)


def invert_cond(c):
    if isinstance(c, c_ast.BinaryOp) and cmp_invert(c.op):
        return BO(cmp_invert(c.op), c.left, c.right)
    if isinstance(c, c_ast.UnaryOp) and c.op == "!":
        return c.expr
    return UO("!", c)


# --------------------------------------------------------------- context

class Ctx:
    """A body tree with node numbering, parent links and the local type table."""

    def __init__(self, body, src, head_subs=()):
        self.body, self.src = body, src
        self.head_subs = list(head_subs)
        self.refresh()

    def refresh(self):
        self.nodes = preorder(self.body, [])
        self.pm = {}
        for n in self.nodes:
            for name, c in n.children():
                self.pm[id(c)] = (n, name)
        self.kinds = {}
        for p in self.src.params:
            self.kinds[p.name] = kind_of(p)
        self.decls = {}
        for n in self.nodes:
            if isinstance(n, c_ast.Decl) and n.name:
                self.kinds[n.name] = kind_of(n)
                self.decls[n.name] = n
        self.params = {p.name for p in self.src.params}

    def copy(self):
        return Ctx(copy.deepcopy(self.body), self.src, self.head_subs)

    def fresh(self):
        used = {n.name for n in self.nodes if isinstance(n, c_ast.ID)}
        used |= {n.name for n in self.nodes if isinstance(n, c_ast.Decl) and n.name}
        used |= self.params
        i = 0
        while "lv%d" % i in used:
            i += 1
        return "lv%d" % i

    # ---- edits (all keep pm valid enough for one more site lookup; call refresh())
    def replace(self, old, new):
        par, name = self.pm[id(old)]
        attr, idx = _slot(name)
        if idx is None:
            setattr(par, attr, new)
        else:
            getattr(par, attr)[idx] = new

    def container_of(self, stmt):
        """(list, index) holding stmt, or None."""
        par, name = self.pm.get(id(stmt), (None, None))
        if par is None:
            return None
        attr, idx = _slot(name)
        if idx is not None and any(isinstance(par, t) and attr == a for t, a in CONTAINERS):
            lst = getattr(par, attr)
            for k, m in enumerate(lst):     # positions shift as edits insert; find by identity
                if m is stmt:
                    return lst, k
        return None

    def anchor(self, node):
        """(statement, unconditional) for an expression node, or None if it cannot
        be hoisted to a statement boundary."""
        cur, uncond = node, True
        while True:
            par, name = self.pm.get(id(cur), (None, None))
            if par is None:
                return None
            attr, idx = _slot(name)
            if idx is not None and any(isinstance(par, t) and attr == a for t, a in CONTAINERS):
                return cur, uncond
            if isinstance(par, c_ast.BinaryOp) and par.op in ("&&", "||") and name == "right":
                uncond = False
            if isinstance(par, c_ast.TernaryOp) and name != "cond":
                uncond = False
            if isinstance(par, (c_ast.For, c_ast.While, c_ast.DoWhile, c_ast.Label,
                                c_ast.Switch, c_ast.Case, c_ast.Default)):
                return None
            if isinstance(par, c_ast.If) and name != "cond":
                return None
            cur = par

    def enclosing_compound(self, stmt):
        par, name = self.pm.get(id(stmt), (None, None))
        return par if isinstance(par, c_ast.Compound) else None

    def add_local(self, home, decl):
        """Insert a declaration after home's leading declarations."""
        items = home.block_items
        k = 0
        while k < len(items) and isinstance(items[k], c_ast.Decl):
            k += 1
        items.insert(k, decl)

    def top_home(self, stmt):
        """The Compound to declare a temp in for a statement (its container)."""
        h = self.enclosing_compound(stmt)
        return h if h is not None else self.body


class Site:
    __slots__ = ("rule", "nid", "key", "ord", "desc", "apply", "risky", "prio", "var")

    def __init__(self, rule, nid, key, desc, apply, risky=False, prio=5):
        self.rule, self.nid, self.key, self.desc = rule, nid, key, desc
        self.apply, self.risky, self.prio = apply, risky, prio
        self.ord = 0
        self.var = 0

    @property
    def sig(self):
        return (self.rule, self.key, self.ord, self.desc)

    @property
    def name(self):
        return "%s-%d%s" % (self.rule.replace("'", "p"), self.nid, chr(97 + self.var) if self.var else "")


# ------------------------------------------------------------ aliases

_ALIASES = {}
_DECLARED = None


def lds_aliases():
    """{name: other} both ways for the `a = b;` symbol aliases in aw2bhr.lds."""
    if not _ALIASES:
        try:
            for ln in open(os.path.join(REPO, "aw2bhr.lds"), encoding="utf-8", errors="replace"):
                m = re.match(r"\s*([A-Za-z_]\w*)\s*=\s*([A-Za-z_]\w*)\s*;", ln)
                if m:
                    _ALIASES[m.group(1)] = m.group(2)
                    _ALIASES[m.group(2)] = m.group(1)
        except OSError:
            pass
    return _ALIASES


def declared_words(src):
    """Identifiers visible to the draft: the words of its preprocessed text."""
    if not hasattr(src, "_words"):
        rc, so, se = drafts.preprocess(src.fn, drafts.rel(src.path))
        src._words = set(re.findall(r"[A-Za-z_]\w*", so if rc == 0 else src.text))
    return src._words


def pool_words(fn, name):
    """How many `.4byte` pool words of the ROM function resolve to name or its alias."""
    al = lds_aliases()
    names = {name, al.get(name, name)}
    p = os.path.join(REPO, "work", fn, "target.s")
    try:
        t = open(p, encoding="utf-8", errors="replace").read()
    except OSError:
        return None
    addrs = set()
    for n in names:
        m = re.match(r"gUnknown_([0-9A-Fa-f]{8})$", n)
        if m:
            addrs.add(int(m.group(1), 16))
    cnt = 0
    for m in re.finditer(r"\.4byte\s+(\w+)", t):
        w = m.group(1)
        if w in names or (w.lower().startswith("0x") and int(w, 16) in addrs):
            cnt += 1
    return cnt


def _alias_apply(new, decl):
    def f(c, node):
        c.replace(node, ID(new))
        if decl:
            c.head_subs.append((None, decl))
    return f


def alias_sites(ctx, add, force=False):
    al = lds_aliases()
    fn = ctx.src.fn
    words = declared_words(ctx.src)
    for n in ctx.nodes:
        if not isinstance(n, c_ast.ID) or n.name not in al or n.name in ctx.kinds:
            continue
        par, pname = ctx.pm[id(n)]
        if (isinstance(par, c_ast.FuncCall) and pname == "name") or                 (isinstance(par, c_ast.StructRef) and pname == "field"):
            continue
        pw = pool_words(fn, n.name)
        if pw is not None and pw < 2 and not force:
            continue
        new = al[n.name]
        decl = None if new in words else "extern __typeof__(%s) %s;" % (n.name, new) + chr(10)
        add("7", n, "%s -> %s (pool words in ROM: %s)" % (n.name, new, pw), _alias_apply(new, decl), prio=0,
            key="%s>%s" % (n.name, new))


# ------------------------------------------------------------ enumeration

def _lvalue_position(ctx, e):
    par, name = ctx.pm.get(id(e), (None, None))
    if isinstance(par, c_ast.Assignment) and name == "lvalue":
        return True
    if isinstance(par, c_ast.UnaryOp) and par.op in ("&", "sizeof", "++", "--", "p++", "p--"):
        return True
    if isinstance(par, c_ast.FuncCall) and name == "name":
        return True
    if isinstance(par, c_ast.StructRef) and name == "field":
        return True
    if isinstance(par, (c_ast.Decl, c_ast.TypeDecl, c_ast.Typename)):
        return name != "init"
    if isinstance(par, c_ast.Cast) and name == "to_type":
        return True
    return False


def _chain(e, ops):
    """Leaves of a chain of the ops in `ops` -> [(sign, expr)] or None."""
    out = []

    def rec(n, s):
        if isinstance(n, c_ast.BinaryOp) and n.op in ops:
            rec(n.left, s)
            rec(n.right, s if n.op != "-" else -s)
        else:
            out.append((s, n))
    rec(e, 1)
    return out


def _build_left(terms):
    e = None
    for s, t in terms:
        if e is None:
            if s < 0:
                return None
            e = t
        else:
            e = BO("+" if s > 0 else "-", e, t)
    return e


def _build_right(terms):
    """t0 (+|-) (t1 ... ) with the group's signs folded into the outer operator."""
    if len(terms) < 3 or terms[0][0] < 0:
        return None
    s1 = terms[1][0]
    grp = [(s * s1, t) for s, t in terms[1:]]
    inner = _build_left(grp)
    if inner is None:
        return None
    return BO("+" if s1 > 0 else "-", terms[0][1], inner)


def _mulchain(e):
    return _chain(e, ("*",)) if isinstance(e, c_ast.BinaryOp) and e.op == "*" else None


def reassoc_variants(e):
    """Alternative spellings of a + - chain or a single-op chain, as expression nodes."""
    if not isinstance(e, c_ast.BinaryOp):
        return []
    out = {}
    if e.op in ("+", "-"):
        terms = _chain(e, ("+", "-"))
        n = len(terms)
        if n < 2 or n > 4:
            return []
        orders = (list(itertools.permutations(range(n))) if n <= 3
                  else [tuple(list(range(i)) + [i + 1, i] + list(range(i + 2, n))) for i in range(n - 1)])
        for o in orders:
            t2 = [terms[i] for i in o]
            for f in (_build_left, _build_right):
                x = f(t2)
                if x is not None:
                    out.setdefault(G(x), x)
        if n == 4:
            x = BO("+", _build_left(terms[:2]) or terms[0][1], _build_left([(1, terms[2][1]), terms[3]]) or terms[3][1]) \
                if terms[0][0] > 0 and terms[2][0] > 0 else None
            if x is not None and all(s > 0 for s, _ in terms[:2]):
                out.setdefault(G(x), x)
    elif e.op in ("*", "|", "&", "^"):
        terms = _chain(e, (e.op,))
        n = len(terms)
        if n < 2 or n > 3:
            return []
        for o in itertools.permutations(range(n)):
            l = terms[o[0]][1]
            for i in o[1:]:
                l = BO(e.op, l, terms[i][1])
            out.setdefault(G(l), l)
            if n == 3:
                r = BO(e.op, terms[o[0]][1], BO(e.op, terms[o[1]][1], terms[o[2]][1]))
                out.setdefault(G(r), r)
    orig = G(e)
    return [v for k, v in out.items() if k != orig]


def factor_variants(e):
    """A*m +- k -> (A*(m/g) +- k/g) * g, and (A*a +- b) * g -> A*(a*g) +- b*g."""
    out = []
    if not isinstance(e, c_ast.BinaryOp):
        return out
    if e.op in ("+", "-"):
        k = const_int(e.right)
        l = e.left
        if k is not None and k != 0 and isinstance(l, c_ast.BinaryOp) and l.op in ("*", "<<") \
                and const_int(l.right) is not None:
            m = const_int(l.right)
            m = m if l.op == "*" else (1 << m)
            for g in (2, 4, 8, 16, 32):
                if m % g == 0 and k % g == 0 and m // g >= 1:
                    inner_a = l.left if m // g == 1 else BO("*", l.left, K(m // g))
                    out.append(BO("*", BO(e.op, inner_a, K(k // g)), K(g)))
    if e.op == "*":
        g = const_int(e.right)
        l = e.left
        if g and isinstance(l, c_ast.BinaryOp) and l.op in ("+", "-") and const_int(l.right) is not None:
            b = const_int(l.right)
            a = l.left
            if isinstance(a, c_ast.BinaryOp) and a.op == "*" and const_int(a.right) is not None:
                out.append(BO(l.op, BO("*", a.left, K(const_int(a.right) * g)), K(b * g)))
            else:
                out.append(BO(l.op, BO("*", a, K(g)), K(b * g)))
    return out


def _in_cond_context(ctx, e):
    par, name = ctx.pm.get(id(e), (None, None))
    if par is None:
        return False
    if isinstance(par, (c_ast.If, c_ast.While, c_ast.DoWhile, c_ast.For)) and name == "cond":
        return True
    if isinstance(par, c_ast.TernaryOp) and name == "cond":
        return True
    if isinstance(par, c_ast.BinaryOp) and par.op in ("&&", "||"):
        return True
    if isinstance(par, c_ast.UnaryOp) and par.op == "!":
        return True
    return False


def enumerate_sites(ctx):
    S = []
    nodes = ctx.nodes
    nid = {id(n): i for i, n in enumerate(nodes)}

    def add(rule, node, desc, apply, risky=False, prio=5, key=None):
        S.append(Site(rule, nid[id(node)], key if key is not None else G(node), desc, apply, risky, prio))

    # duplicate expression groups (pure, non-trivial), for site priority
    counts = {}
    for n in nodes:
        if isinstance(n, (c_ast.BinaryOp, c_ast.ArrayRef, c_ast.StructRef)) and is_pure(n) \
                and not _lvalue_position(ctx, n):
            counts[G(n)] = counts.get(G(n), 0) + 1
    dup = {k for k, v in counts.items() if v >= 2}

    def setexpr(new):
        def f(c, node):
            c.replace(node, new)
        return f

    for n in nodes:
        # -------- rule 1: arithmetic respells
        if isinstance(n, c_ast.BinaryOp) and is_pure(n) and not _lvalue_position(ctx, n):
            pr = 1 if G(n) in dup else 6
            for v in factor_variants(n):
                add("1a", n, "%s -> %s" % (G(n), G(v)), (lambda new: lambda c, nd: c.replace(nd, copy.deepcopy(new)))(v), prio=pr)
            if n.op in ("+", "-") and const_int(n.right) == 1:
                new = UO("-", UO("~", n.left)) if n.op == "+" else UO("~", UO("-", n.left))
                add("1b", n, "%s -> %s" % (G(n), G(new)), (lambda new: lambda c, nd: c.replace(nd, copy.deepcopy(new)))(new), prio=pr)
            if n.op in ("+", "-", "*", "|", "&", "^"):
                for v in reassoc_variants(n):
                    add("1d", n, "%s -> %s" % (G(n), G(v)), (lambda new: lambda c, nd: c.replace(nd, copy.deepcopy(new)))(v), prio=pr + 1)
        # -------- rule 4b/4c: comparisons
        if isinstance(n, c_ast.BinaryOp) and n.op in ("==", "!=", "<", ">", "<=", ">=") and is_pure(n):
            mirror = {"<": ">", ">": "<", "<=": ">=", ">=": "<=", "==": "==", "!=": "!="}[n.op]
            if not isinstance(n.left, c_ast.Constant):
                new = BO(mirror, n.right, n.left)
                add("4b", n, "%s -> %s" % (G(n), G(new)), (lambda new: lambda c, nd: c.replace(nd, copy.deepcopy(new)))(new), prio=3)
            k = const_int(n.right)
            if k is not None and _in_cond_context(ctx, n) or (k is not None and n.op in ("<", ">", "<=", ">=")):
                alt = None
                if n.op == "<=" and k < 0x7FFFFFFE:
                    alt = BO("<", n.left, K(k + 1))
                elif n.op == "<" and k >= 1:
                    alt = BO("<=", n.left, K(k - 1))
                elif n.op == ">=" and k >= 1:
                    alt = BO(">", n.left, K(k - 1))
                elif n.op == ">" and k < 0x7FFFFFFE:
                    alt = BO(">=", n.left, K(k + 1))
                if alt is not None:
                    add("4c", n, "%s -> %s" % (G(n), G(alt)), (lambda new: lambda c, nd: c.replace(nd, copy.deepcopy(new)))(alt), prio=3)
            if k == 0 and n.op in ("==", "!=") and _in_cond_context(ctx, n):
                new = n.left if n.op == "!=" else UO("!", n.left)
                add("4b", n, "%s -> %s" % (G(n), G(new)), (lambda new: lambda c, nd: c.replace(nd, copy.deepcopy(new)))(new), prio=3)
        if isinstance(n, c_ast.UnaryOp) and n.op == "!" and is_pure(n):
            new = BO("==", n.expr, K(0))
            add("4b", n, "%s -> %s" % (G(n), G(new)), (lambda new: lambda c, nd: c.replace(nd, copy.deepcopy(new)))(new), prio=3)
        if isinstance(n, (c_ast.ID, c_ast.ArrayRef, c_ast.StructRef)) and _in_cond_context(ctx, n) \
                and not (isinstance(ctx.pm[id(n)][0], c_ast.UnaryOp)) and not _lvalue_position(ctx, n):
            new = BO("!=", n, K(0))
            add("4b", n, "%s -> %s" % (G(n), G(new)), (lambda new: lambda c, nd: c.replace(nd, copy.deepcopy(new)))(new), prio=4)

        # -------- rule 4a: if/else arms
        if isinstance(n, c_ast.If):
            if n.iffalse is not None:
                def f(c, nd):
                    nd.cond = invert_cond(nd.cond)
                    nd.iftrue, nd.iffalse = nd.iffalse, nd.iftrue
                    if isinstance(nd.iftrue, c_ast.If):
                        nd.iftrue = c_ast.Compound([nd.iftrue])
                add("4a", n, "swap arms of if (%s)" % G(n.cond), f, prio=2, key=G(n.cond))
            else:
                cont = ctx.container_of(n)
                if cont is not None:
                    lst, i = cont
                    if i + 2 == len(lst) + 0 and isinstance(lst[i + 1], c_ast.Return) \
                            and len(n.iftrue.block_items) == 1 and isinstance(n.iftrue.block_items[0], c_ast.Return) \
                            and (n.iftrue.block_items[0].expr is None) == (lst[i + 1].expr is None):
                        def f(c, nd):
                            l, k = c.container_of(nd)
                            r1, r2 = nd.iftrue.block_items[0], l[k + 1]
                            nd.cond = invert_cond(nd.cond)
                            nd.iftrue = c_ast.Compound([r2])
                            l[k + 1] = r1
                        add("4a", n, "swap early return at if (%s)" % G(n.cond), f, prio=2, key=G(n.cond))

        # -------- rule 2: signedness
        if isinstance(n, c_ast.Decl) and n.name and not isinstance(n.type, c_ast.FuncDecl):
            kd = kind_of(n)
            fl = flipped_names(kd[3]) if kd[0] == "int" else None
            if fl:
                def f(c, nd, fl=fl):
                    nd.type.type.names = list(fl)
                add("2", n, "%s %s -> %s" % (" ".join(kd[3]), n.name, " ".join(fl)), f, risky=True, prio=1,
                    key=n.name)
    for p in ctx.src.params:
        kd = kind_of(p)
        fl = flipped_names(kd[3]) if kd[0] == "int" else None
        if fl and len(kd[3]) == 1:
            # a parameter's type lives in the head text: substitute there
            rx = r"\b%s(\s+)%s\b" % (re.escape(kd[3][0]), re.escape(p.name))
            new = "%s\\g<1>%s" % (fl[0], p.name)

            def f(c, nd, rx=rx, new=new):
                c.head_subs.append((rx, new))
            add("2", ctx.body, "param %s %s -> %s" % (kd[3][0], p.name, fl[0]), f, risky=True, prio=1,
                key="param " + p.name)

    if "7" in RULES_ON:
        alias_sites(ctx, add, FORCE_ALIAS)
    # -------- rule 1c: narrow copy of one use
    for n in nodes:
        if isinstance(n, c_ast.ID) and n.name in ctx.kinds and ctx.kinds[n.name][0] == "int" \
                and not _lvalue_position(ctx, n):
            par, pname = ctx.pm[id(n)]
            if isinstance(par, c_ast.UnaryOp) or isinstance(par, c_ast.Decl):
                continue
            a = ctx.anchor(n)
            if a is None or isinstance(a[0], (c_ast.Decl, c_ast.Compound)):
                continue
            kd = ctx.kinds[n.name]
            if kd[1] < 32:
                for flip in (False, True):
                    names = list(kd[3]) if not flip else flipped_names(kd[3])
                    if not names:
                        continue
                    add("1c'" if flip else "1c", n, "copy of %s as %s for this use" % (n.name, " ".join(names)),
                        _copy_use(names), risky=flip, prio=2, key=n.name)
    # -------- rule 3: loop steps
    for n in nodes:
        if isinstance(n, c_ast.For):
            x, sgn = _step_of(n.next)
            if x is None or _touches(n.stmt, x) or n.init is not None and isinstance(n.init, c_ast.DeclList):
                continue
            has_cont = any(isinstance(m, c_ast.Continue) for m in _own_jumps(n.stmt))
            add("3a", n, "for step %s through a copy (top)" % x, _for3(x, sgn, "a"), prio=2, key=x)
            add("3d", n, "3a and body `%s + 1` -> copy" % x, _for3(x, sgn, "d"), prio=2, key=x)
            if not has_cont and ctx.container_of(n) is not None:
                add("3b", n, "for -> while, copy computed at the top" % (), _for3(x, sgn, "b"), prio=2, key=x)
                add("3c", n, "for -> while, copy computed at the bottom", _for3(x, sgn, "c"), prio=2, key=x)
        if isinstance(n, c_ast.Assignment) or isinstance(n, c_ast.UnaryOp):
            x, sgn = _step_of(n)
            par, name = ctx.pm.get(id(n), (None, None))
            if x is not None and isinstance(par, c_ast.Compound) and ctx.container_of(n) is not None:
                add("3e", n, "statement %s step through a copy" % G(n), _step3e(x, sgn), prio=2, key=G(n))
    # -------- rule 5c: inline a local
    _rule5c(ctx, S, add)
    # -------- rule 5d: do { } while (0)
    for n in nodes:
        if isinstance(n, c_ast.Compound) and n.block_items:
            par, name = ctx.pm.get(id(n), (None, None))
            items = n.block_items
            k = 0
            while k < len(items) and isinstance(items[k], c_ast.Decl):
                k += 1
            rest = items[k:]
            if not rest or any(isinstance(m, (c_ast.Break, c_ast.Continue)) for r in rest for m in _own_jumps(r, True)):
                continue
            if len(rest) == 1 and isinstance(rest[0], c_ast.DoWhile):
                continue

            def f(c, nd, k=k):
                its = nd.block_items
                head, rest = its[:k], its[k:]
                nd.block_items = head + [c_ast.DoWhile(K(0), c_ast.Compound(rest))]
            add("5d", n, "do { } while (0) around a block of %d statements" % len(rest), f, prio=4,
                key="%d:%s" % (len(rest), G(rest[0])[:40]))
    # -------- rule 5a/5b/5f: split an expression into its own statement
    for n in nodes:
        if not isinstance(n, (c_ast.BinaryOp, c_ast.ArrayRef, c_ast.StructRef, c_ast.ID, c_ast.Cast)):
            continue
        if isinstance(n, c_ast.ID):
            if n.name in ctx.kinds or re.match(r"^[A-Z0-9_]+$", n.name) or n.name in ("NULL",):
                continue
            par, _ = ctx.pm[id(n)]
            if isinstance(par, c_ast.FuncCall):
                continue
        if isinstance(n, c_ast.Cast) and not isinstance(n.expr, (c_ast.BinaryOp, c_ast.ArrayRef, c_ast.StructRef)):
            continue
        if not is_pure(n) or _lvalue_position(ctx, n):
            continue
        if isinstance(ctx.pm[id(n)][0], c_ast.StructRef) and ctx.pm[id(n)][1] == "name" and \
                not isinstance(n, c_ast.ID):
            pass
        a = ctx.anchor(n)
        if a is None or not a[1]:
            continue
        stmt = a[0]
        if stmt is n or isinstance(stmt, (c_ast.Compound, c_ast.Decl)) and False:
            continue
        if isinstance(stmt, (c_ast.Break, c_ast.Continue, c_ast.Goto, c_ast.Compound)):
            continue
        # skip when E is already a whole `x = E` right-hand side of a plain local store
        par, pname = ctx.pm[id(n)]
        if isinstance(par, c_ast.Assignment) and pname == "rvalue" and par.op == "=" \
                and isinstance(par.lvalue, c_ast.ID) and par.lvalue.name in ctx.kinds:
            continue
        if isinstance(par, c_ast.Decl):
            continue
        pr = 3 if G(n) in dup else 7
        add("5a", n, "lv = %s;" % G(n), _split(reuse=None, every=False), prio=pr, risky=False)
        if counts.get(G(n), 0) >= 2:
            add("5f", n, "lv = %s; (all copies in the statement)" % G(n), _split(reuse=None, every=True), prio=pr)
        if _int_only(ctx, n):
            for name, kd in ctx.kinds.items():
                if kd[0] == "int" and kd[1] == 32 and name not in names_in(n) and name not in ctx.params:
                    add("5b", n, "%s = %s;" % (name, G(n)), _split(reuse=name, every=False), prio=8, risky=True,
                        key="%s<-%s" % (name, G(n)))
    # dedupe by (rule, key) -> ordinal
    seen, var = {}, {}
    for s in S:
        k = (s.rule, s.key, s.desc)
        s.ord = seen.get(k, 0)
        seen[k] = s.ord + 1
        k2 = (s.rule, s.nid)
        s.var = var.get(k2, 0)
        var[k2] = s.var + 1
    return S


def _int_only(ctx, e):
    for m in preorder(e, []):
        if isinstance(m, (c_ast.ArrayRef, c_ast.StructRef, c_ast.FuncCall)):
            return False
        if isinstance(m, c_ast.UnaryOp) and m.op in ("*", "&"):
            return False
        if isinstance(m, c_ast.ID):
            k = ctx.kinds.get(m.name)
            if k is None or k[0] != "int":
                return False
    return isinstance(e, c_ast.BinaryOp)


def _own_jumps(node, incl_switch_break=False):
    """Break/Continue nodes that belong to the enclosing loop of `node` itself."""
    out = []

    def rec(n, in_loop, in_switch):
        if isinstance(n, c_ast.Continue) and not in_loop:
            out.append(n)
        if isinstance(n, c_ast.Break) and not in_loop and not in_switch:
            out.append(n)
        loop = in_loop or isinstance(n, (c_ast.For, c_ast.While, c_ast.DoWhile))
        sw = in_switch or isinstance(n, c_ast.Switch)
        for _, c in n.children():
            rec(c, loop, sw if not isinstance(n, (c_ast.For, c_ast.While, c_ast.DoWhile)) else False)
    rec(node, False, False)
    return out


def _step_of(e):
    """(name, +1/-1) if e steps a plain variable by one."""
    if isinstance(e, c_ast.UnaryOp) and e.op in ("p++", "++", "p--", "--") and isinstance(e.expr, c_ast.ID):
        return e.expr.name, (1 if "+" in e.op else -1)
    if isinstance(e, c_ast.Assignment) and isinstance(e.lvalue, c_ast.ID):
        if e.op == "+=" and const_int(e.rvalue) == 1:
            return e.lvalue.name, 1
        if e.op == "-=" and const_int(e.rvalue) == 1:
            return e.lvalue.name, -1
        r = e.rvalue
        if e.op == "=" and isinstance(r, c_ast.BinaryOp) and r.op in ("+", "-") and \
                isinstance(r.left, c_ast.ID) and r.left.name == e.lvalue.name and const_int(r.right) == 1:
            return e.lvalue.name, (1 if r.op == "+" else -1)
    return None, 0


def _touches(node, x):
    for m in preorder(node, []):
        if isinstance(m, c_ast.Assignment) and isinstance(m.lvalue, c_ast.ID) and m.lvalue.name == x:
            return True
        if isinstance(m, c_ast.UnaryOp) and m.op in ("p++", "p--", "++", "--", "&") and \
                isinstance(m.expr, c_ast.ID) and m.expr.name == x:
            return True
    return False


def _nx_decl(c, x, nname):
    kd = c.kinds.get(x)
    if kd is not None and kd[0] == "int" and kd[1] < 32:
        return mk_decl(nname, ["int"])
    return mk_decl(nname, typeof_names(ID(x)))


def _for3(x, sgn, mode):
    op = "+" if sgn > 0 else "-"

    def f(c, node):
        nname = c.fresh()
        body = node.stmt
        c.add_local(c.body, _nx_decl(c, x, nname))
        set_nx = c_ast.Assignment("=", ID(nname), BO(op, ID(x), K(1)))
        back = c_ast.Assignment("=", ID(x), ID(nname))
        items = body.block_items
        k = 0
        while k < len(items) and isinstance(items[k], c_ast.Decl):
            k += 1
        if mode in ("a", "d"):
            if mode == "d":
                for m in preorder(body, []):
                    pass
                for m in preorder(body, []):
                    for cname, ch in m.children():
                        if isinstance(ch, c_ast.BinaryOp) and ch.op == op and isinstance(ch.left, c_ast.ID) \
                                and ch.left.name == x and const_int(ch.right) == 1:
                            a, i = _slot(cname)
                            if i is None:
                                setattr(m, a, ID(nname))
                            else:
                                getattr(m, a)[i] = ID(nname)
            items.insert(k, set_nx)
            node.next = back
            return
        cont = c.container_of(node)
        lst, i = cont
        if mode == "b":
            items.insert(k, set_nx)
            items.append(back)
        else:
            items.append(set_nx)
            items.append(back)
        cond = node.cond if node.cond is not None else K(1)
        new = [c_ast.While(cond, body)]
        init = node.init
        pre = []
        if init is not None:
            if isinstance(init, c_ast.ExprList):
                pre = list(init.exprs)
            else:
                pre = [init]
        lst[i:i + 1] = pre + new
    return f


def _step3e(x, sgn):
    op = "+" if sgn > 0 else "-"

    def f(c, node):
        lst, i = c.container_of(node)
        nname = c.fresh()
        c.add_local(c.body, _nx_decl(c, x, nname))
        lst[i:i + 1] = [c_ast.Assignment("=", ID(nname), BO(op, ID(x), K(1))),
                        c_ast.Assignment("=", ID(x), ID(nname))]
    return f


def _copy_use(names):
    def f(c, node):
        stmt, _ = c.anchor(node)
        nname = c.fresh()
        d = mk_decl(nname, names, ID(node.name))
        c.replace(node, ID(nname))
        lst, i = c.container_of(stmt)
        lst[i] = c_ast.Compound([d, stmt])
    return f


def _split(reuse, every):
    def f(c, node):
        stmt, _ = c.anchor(node)
        e = copy.deepcopy(node)
        if reuse is None:
            nname = c.fresh()
            home = c.top_home(stmt)
            c.add_local(home, mk_decl(nname, typeof_names(e)))
        else:
            nname = reuse
        text = G(node)
        if every:
            for m in preorder(stmt, []):
                for cname, ch in list(m.children()):
                    if isinstance(ch, type(node)) and G(ch) == text:
                        a, i = _slot(cname)
                        if i is None:
                            setattr(m, a, ID(nname))
                        else:
                            getattr(m, a)[i] = ID(nname)
        else:
            c.replace(node, ID(nname))
        lst, i = c.container_of(stmt)
        lst.insert(i, c_ast.Assignment("=", ID(nname), e))
    return f


def _rule5c(ctx, S, add):
    """Inline a local that is written once (`v = E;` or an initializer) and read once."""
    writes, reads = {}, {}
    for n in ctx.nodes:
        if isinstance(n, c_ast.Assignment) and isinstance(n.lvalue, c_ast.ID):
            writes.setdefault(n.lvalue.name, []).append(n)
        elif isinstance(n, c_ast.UnaryOp) and n.op in ("p++", "p--", "++", "--", "&") and isinstance(n.expr, c_ast.ID):
            writes.setdefault(n.expr.name, []).append(n)
        elif isinstance(n, c_ast.Decl) and n.init is not None and n.name in ctx.kinds:
            writes.setdefault(n.name, []).append(n)
        elif isinstance(n, c_ast.ID):
            par, name = ctx.pm.get(id(n), (None, None))
            if isinstance(par, c_ast.Assignment) and name == "lvalue":
                continue
            reads.setdefault(n.name, []).append(n)
    for v, ws in writes.items():
        if v not in ctx.kinds or v in ctx.params or len(ws) != 1 or len(reads.get(v, [])) != 1:
            continue
        w, use = ws[0], reads[v][0]
        if isinstance(w, c_ast.Decl):
            expr, stmt = w.init, w
        elif isinstance(w, c_ast.Assignment) and w.op == "=":
            expr = w.rvalue
            par, name = ctx.pm[id(w)]
            if not (isinstance(par, c_ast.Compound) and name.startswith("block_items")):
                continue
            stmt = w
        else:
            continue
        if not is_pure(expr):
            continue
        cont = ctx.container_of(stmt)
        if cont is None:
            continue
        lst, i = cont
        ua = ctx.anchor(use)
        if ua is None:
            continue
        # the use's statement must lie in a later sibling (or inside it), same list
        ustmt = ua[0]
        # walk up from ustmt to see whether it is inside lst[j>i]
        cur, j = ustmt, None
        while cur is not None:
            c2 = ctx.container_of(cur)
            if c2 is not None and c2[0] is lst:
                j = c2[1]
                break
            cur = ctx.pm.get(id(cur), (None, None))[0]
        if j is None or j <= i:
            continue
        # nothing between them may write a name in expr, nor call/store when expr reads memory
        exprnames = set(names_in(expr))
        mem = any(isinstance(m, (c_ast.ArrayRef, c_ast.StructRef)) or (isinstance(m, c_ast.UnaryOp) and m.op == "*")
                  for m in preorder(expr, [])) or any(k not in ctx.kinds for k in exprnames)
        bad = False
        for s2 in lst[i + 1:j + 1]:
            for m in preorder(s2, []):
                if isinstance(m, c_ast.Assignment):
                    if isinstance(m.lvalue, c_ast.ID) and m.lvalue.name in exprnames:
                        bad = True
                    if mem and not isinstance(m.lvalue, c_ast.ID):
                        bad = True
                if isinstance(m, c_ast.UnaryOp) and m.op in ("p++", "p--", "++", "--") and \
                        isinstance(m.expr, c_ast.ID) and m.expr.name in exprnames:
                    bad = True
                if isinstance(m, c_ast.FuncCall) and mem and s2 is not lst[j]:
                    bad = True
                if isinstance(m, (c_ast.For, c_ast.While, c_ast.DoWhile)) and m is not None and s2 is lst[j] and \
                        any(x is use for x in preorder(m, [])) and exprnames:
                    bad = True  # a loop re-evaluates the inlined expression each pass
        if bad:
            continue
        narrow = ctx.kinds[v][0] == "int" and ctx.kinds[v][1] < 32
        add("5c", use, "inline %s = %s" % (v, G(expr)), _inline(v), risky=narrow or ctx.kinds[v][0] != "int",
            prio=2, key=v)


def _inline(v):
    def f(c, use):
        # locate the single write again in the copy
        w = None
        for n in c.nodes:
            if isinstance(n, c_ast.Assignment) and isinstance(n.lvalue, c_ast.ID) and n.lvalue.name == v \
                    and n.op == "=":
                w = n
            elif isinstance(n, c_ast.Decl) and n.name == v and n.init is not None:
                w = n
        expr = w.rvalue if isinstance(w, c_ast.Assignment) else w.init
        c.replace(use, copy.deepcopy(expr))
        lst, i = c.container_of(w)
        if isinstance(w, c_ast.Decl):
            w.init = None
        else:
            del lst[i]
    return f


# ------------------------------------------------------------- evaluation

def rank_key(r):
    if r["state"] == "MATCH":
        return (-1, 0, 0)
    if r["state"] != "MISMATCH":
        return (9, 0, 0)
    return (0 if r["size_delta"] == 0 else 1, -r["pct"], -r["first"])


class Evaluator:
    def __init__(self, src, threads):
        self.src, self.fn = src, src.fn
        self.threads = max(1, threads)
        self.dir = os.path.join(REPO, OUT, self.fn)
        os.makedirs(self.dir, exist_ok=True)
        self.n = 0
        self.cache = {}

    def _run(self, label, text):
        path = os.path.join(self.dir, label + ".c")
        awlib.write_text(path, text)
        b = drafts.build(self.fn, drafts.rel(path), "lv-" + label)
        r = drafts.score(self.fn, b)
        r = dict(r)
        r["sha"] = hashlib.sha1(b.text + b"|" + b.rodata).hexdigest() if b.ok else None
        r["size"] = len(b.text) if b.ok else None
        base = os.path.join(REPO, "build", "drafts", self.fn, "lv-" + label)
        for ext in (".o", ".s", ".text.bin", ".rodata.bin"):
            try:
                os.remove(base + ext)
            except OSError:
                pass
        try:
            os.remove(path)
        except OSError:
            pass
        return r

    def evaluate(self, jobs):
        """jobs: [(label, text)] -> [result], compiled on `threads` workers."""
        with concurrent.futures.ThreadPoolExecutor(self.threads) as ex:
            return list(ex.map(lambda j: self._run(*j), jobs))


def apply_sites(base_ctx, sites):
    """Apply the first site on a copy of base_ctx, then re-find each later one by
    signature in the changed tree. Returns a Ctx, or None if a site was not found."""
    c = base_ctx.copy()
    first = True
    for s in sites:
        if first:
            node = c.nodes[s.nid]
            first = False
        else:
            cur = enumerate_sites(c)
            m = next((x for x in cur if x.sig == s.sig), None)
            if m is None:
                return None
            s, node = m, c.nodes[m.nid]
        try:
            s.apply(c, node)
        except Exception:
            return None
        c.refresh()
    return c


def parse_rules(spec):
    if not spec:
        return set(RULE_IDS)
    out = set()
    for tok in spec.split(","):
        tok = tok.strip()
        hits = [r for r in RULE_IDS if r == tok or (tok.isdigit() and r.startswith(tok))
                and (len(tok) < 2 or True)]
        if tok.isdigit():
            hits = [r for r in RULE_IDS if r.rstrip("abcdef") == tok]
        if not hits:
            raise SystemExit("error: unknown rule %r (rules: %s)" % (tok, ", ".join(RULE_IDS)))
        out.update(hits)
    return out


def rule_base(s):
    return s.rule.rstrip("'")


def search(fn, draft, rules, seconds, max_singles, threads, pairs=True, log=print):
    """Run the whole search for one function. Returns a dict of results."""
    t0 = time.time()
    src = Source(fn, draft)
    body0 = normalize(copy.deepcopy(src.fd.body))
    ctx0 = Ctx(body0, src)
    ev = Evaluator(src, threads)
    res = {"fn": fn, "draft": draft, "rows": [], "notes": []}
    base = ev._run("base", src.text)
    if base["state"] in ("COMPILE-FAIL", "ERROR"):
        res["error"] = "draft does not compile: %s" % base.get("error")
        return res
    res["base"] = base
    regen = ev._run("regen", src.render(body0))
    res["regen_ok"] = regen["sha"] == base["sha"]
    if not res["regen_ok"]:
        res["notes"].append("regenerated body differs from the draft's own bytes (%s); "
                            "scores are still against the ROM" % drafts.fmt(regen))
    sites = [s for s in enumerate_sites(ctx0) if rule_base(s) in rules or s.rule in rules
             or s.rule.replace("'", "") in rules]
    sites.sort(key=lambda s: (s.prio, s.nid))
    res["n_sites"] = len(sites)
    deadline = t0 + seconds
    limit = min(len(sites), max_singles or len(sites))
    done = []
    B = threads * 4
    i = 0
    while i < limit and time.time() < deadline * 1.0 - (seconds * 0.35 if pairs else 0):
        chunk = sites[i:i + B]
        i += len(chunk)
        jobs, meta = [], []
        for s in chunk:
            c = apply_sites(ctx0, [s])
            if c is None:
                continue
            jobs.append((s.name, src.render(c.body, c.head_subs)))
            meta.append((s, c))
        for (s, c), r in zip(meta, ev.evaluate(jobs)):
            done.append((s, r, c))
    res["n_singles"] = len(done)
    rows = []
    for s, r, c in done:
        rows.append({"label": s.name, "sites": [s], "r": r, "ctx": c})
    active = [x for x in rows if x["r"]["state"] in ("MATCH", "MISMATCH") and x["r"]["sha"] != base["sha"]]
    res["n_inert"] = sum(1 for x in rows if x["r"]["state"] in ("MATCH", "MISMATCH") and x["r"]["sha"] == base["sha"])
    res["n_fail"] = sum(1 for x in rows if x["r"]["state"] not in ("MATCH", "MISMATCH"))
    # ---- pairs of active singles
    if pairs and active and not any(x['r']['state'] == 'MATCH' for x in rows):
        active.sort(key=lambda x: rank_key(x["r"]))
        top = active[:40]
        cand = []
        for a in range(len(top)):
            for b in range(a + 1, len(top)):
                if top[a]["sites"][0].nid == top[b]["sites"][0].nid and top[a]["sites"][0].rule == top[b]["sites"][0].rule:
                    continue
                cand.append((a + b, a, b))
        cand.sort()
        # phase 2: the best few actives with every NEARBY site, inert alone or not
        # (sub_0802216C needs a re-association that is active plus a copy that is not)
        by_name = {x["sites"][0].name: x["sites"][0] for x in active}
        near = []
        for a in range(len(active)):
            sa = active[a]["sites"][0]
            for sb in sites:
                d = abs(sb.nid - sa.nid)
                if sb.nid != sa.nid and d <= 6 and sb.name != sa.name:
                    near.append((d * 1000 + min(a, 999), sa, sb))
        near.sort(key=lambda t: t[0])
        pair_list = [(top[a]["sites"][0], top[b]["sites"][0]) for _, a, b in cand] + [(sa, sb) for _, sa, sb in near]
        a7 = [x["sites"][0] for x in active if x["sites"][0].rule == "7"]
        others = [x["sites"][0] for x in active if x["sites"][0].rule != "7"][:25]
        pair_list = [(x, y) for x in a7 for y in a7 if x.nid < y.nid] + [(x, y) for x in a7 for y in others] + pair_list
        seen = set()
        k = 0
        while k < len(pair_list) and time.time() < deadline and not any(x['r']['state'] == 'MATCH' for x in rows):
            chunk = pair_list[k:k + B]
            k += len(chunk)
            jobs, meta = [], []
            for sa, sb in chunk:
                for first, second in ((sa, sb), (sb, sa)):
                    c = apply_sites(ctx0, [first, second])
                    if c is None:
                        continue
                    text = src.render(c.body, c.head_subs)
                    h = hashlib.sha1(text.encode("utf-8", "replace")).hexdigest()
                    if h in seen:
                        continue
                    seen.add(h)
                    jobs.append(("%s+%s" % (first.name, second.name), text))
                    meta.append(([first, second], c))
                    break
            for (ss, c), r in zip(meta, ev.evaluate(jobs)):
                rows.append({"label": "+".join(x.name for x in ss), "sites": ss, "r": r, "ctx": c})
        res["n_pairs"] = k
    res["rows"] = rows
    res["seconds"] = round(time.time() - t0, 1)
    res["src"] = src
    res["ctx0"] = ctx0
    res["ev"] = ev
    return res


def fmt_row(x, base):
    r = x["r"]
    tag = "".join("?" if s.risky else "" for s in x["sites"])
    if r["state"] == "MATCH":
        return "%-24s MATCH" % x["label"]
    if r["state"] != "MISMATCH":
        return "%-24s %s" % (x["label"], r["state"])
    return "%-24s %6.2f%% size%+d first+0x%X %s" % (x["label"], r["pct"], r["size_delta"], r["first"],
                                                      "?" if tag else "")


def describe(x):
    return " AND ".join("[%s] %s" % (s.rule, s.desc) for s in x["sites"])


def finish(res, wrongc_max, do_wrongc, log=print, write=True):
    """Rank, write improving candidates to work/<fn>/levers/, run wrongc."""
    fn, src, ev = res["fn"], res["src"], res["ev"]
    base = res["base"]
    bk = rank_key(base)
    ok = [x for x in res["rows"] if x["r"]["state"] in ("MATCH", "MISMATCH") and x["r"]["sha"] != base["sha"]]
    ok.sort(key=lambda x: (rank_key(x["r"]), len(x["sites"])))
    imp = [x for x in ok if rank_key(x["r"]) < bk]
    res["improving"] = imp
    wdir = os.path.join(REPO, "work", fn, "levers")
    index = []
    nchecked = 0
    for x in imp[:max(wrongc_max, 0) if do_wrongc else 0] if False else imp:
        x["path"] = None
    for j, x in enumerate(imp):
        if not write or j >= 40:
            break
        os.makedirs(wdir, exist_ok=True)
        path = os.path.join(wdir, x["label"].replace("+", "_") + ".c")
        awlib.write_text(path, src.render(x["ctx"].body, x["ctx"].head_subs))
        x["path"] = path
        index.append("%s\t%s\t%s" % (os.path.basename(path), fmt_row(x, base), describe(x)))
        if do_wrongc and (nchecked < wrongc_max or x["r"]["state"] == "MATCH"):
            nchecked += 1
            try:
                findings, notes = wrongc.analyze_all(fn, path, os.path.join(REPO, "work", fn, fn + ".c"), emu=True)
                v = wrongc.verdict(findings)
                bad = [f for f in v if f[0] == "WRONG"]
                warn = [f for f in v if f[0] == "WARN"]
                x["wrongc"] = ("WRONG: " + "; ".join("%s: %s" % (b[1], b[2][:80]) for b in bad)) if bad else \
                    ("OK (%d warn: %s)" % (len(warn), ",".join(sorted({w[1] for w in warn})))) if warn else "OK"
                x["wrongc_notes"] = notes
            except Exception as e:  # noqa: BLE001
                x["wrongc"] = "wrongc failed: %s: %s" % (type(e).__name__, e)
        else:
            x["wrongc"] = "not checked"
    if write and index:
        awlib.write_text(os.path.join(wdir, "index.txt"), "\n".join(index) + "\n")
    res["ranked"] = ok
    return res


def print_report(res, top=25, log=print):
    if res.get("error"):
        log("%s: %s" % (res["fn"], res["error"]))
        return
    base = res["base"]
    log("%s: draft %s   sites=%d singles=%d inert=%d nocompile=%d pairs_tried=%s in %ss" % (
        res["fn"], drafts.fmt(base), res["n_sites"], res["n_singles"], res["n_inert"], res["n_fail"],
        res.get("n_pairs", 0), res["seconds"]))
    for n in res["notes"]:
        log("  note: " + n)
    log("  %-24s %-7s %-6s %s" % ("candidate", "score", "first", "verdict / what")) if res["ranked"] else None
    for x in res["ranked"][:top]:
        mark = "*" if x in res["improving"] else " "
        w = x.get("wrongc")
        log(" %s%s%s" % (mark, fmt_row(x, base), "   [wrongc: %s]" % w if w else ""))
        log("        %s" % describe(x)[:200])


# ------------------------------------------------------------- all-parked

def parked_functions():
    with open(os.path.join(REPO, "data", "parked.json"), encoding="utf-8") as fh:
        return list(json.load(fh)["functions"].keys())


def record_of(res):
    """A JSON-able digest of one function's search, for the summary."""
    if res.get("error"):
        return {"fn": res["fn"], "error": res["error"]}
    def row(x):
        return {"label": x["label"], "r": {k: v for k, v in x["r"].items() if k != "sha"},
                "desc": describe(x), "wrongc": x.get("wrongc"), "path": x.get("path"),
                "risky": any(s.risky for s in x["sites"])}
    return {"fn": res["fn"], "base": {k: v for k, v in res["base"].items() if k != "sha"},
            "n_sites": res["n_sites"], "n_singles": res["n_singles"], "n_inert": res["n_inert"],
            "n_pairs": res.get("n_pairs", 0), "seconds": res["seconds"],
            "improving": [row(x) for x in res["improving"][:8]],
            "matches": [row(x) for x in res["ranked"] if x["r"]["state"] == "MATCH"]}


def summary_md(recs, skipped):
    L = ["# levers.py summary", "", "Generated %s" % time.strftime("%Y-%m-%d %H:%M"), ""]
    matches = [(r["fn"], x) for r in recs if not r.get("error") for x in r["matches"]]
    if matches:
        L += ["## MATCHES (byte-exact by drafts.py scoring, which is trymatch's arithmetic; confirm with trymatch)", ""]
        for fn, x in matches:
            L.append("- **%s**: `%s`  %s  file `%s`  wrongc: %s" % (fn, x["label"], x["desc"], x.get("path"), x.get("wrongc")))
        L.append("")
    else:
        L += ["## MATCHES", "", "none", ""]
    L += ["## Per function", "",
          "| function | draft | best candidate | score | size | first diff | rule / site | wrongc |",
          "|---|---|---|---|---|---|---|---|"]
    for r in recs:
        if r.get("error"):
            L.append("| %s | - | - | - | - | - | %s | - |" % (r["fn"], r["error"][:80].replace("|", "/")))
            continue
        b = r["base"]
        bd = "%.1f%% %+d first+0x%X" % (b.get("pct", 0), b.get("size_delta", 0), b["first"]) if b["state"] == "MISMATCH" else b["state"]
        if r["improving"]:
            x = r["improving"][0]
            q = x["r"]
            sc = "MATCH" if q["state"] == "MATCH" else "%.2f%% (draft %.2f%%)" % (q["pct"], b.get("pct", 0))
            L.append("| %s | %s | `%s` | %s | %+d (draft %+d) | 0x%X (draft 0x%X) | %s%s | %s |" % (
                r["fn"], bd, x["label"], sc, q.get("size_delta", 0), b.get("size_delta", 0),
                q["first"] or 0, b.get("first") or 0, x["desc"].replace("|", "/")[:130],
                " (?)" if x["risky"] else "", x.get("wrongc")))
        else:
            L.append("| %s | %s | none improves (%d sites, %d singles, %d inert, %d pairs, %ss) | - | - | - | - | - |" % (
                r["fn"], bd, r["n_sites"], r["n_singles"], r["n_inert"], r["n_pairs"], r["seconds"]))
    if skipped:
        L += ["", "Skipped: " + ", ".join(skipped)]
    L += ["", "## Improving candidates by function (top 5 each; files under work/<fn>/levers/)", ""]
    for r in recs:
        if r.get("error") or not r["improving"]:
            continue
        L.append("### %s" % r["fn"])
        for x in r["improving"][:5]:
            q = x["r"]
            L.append("- `%s` %s  wrongc: %s  -- %s" % (
                x["label"], "MATCH" if q["state"] == "MATCH" else "%.2f%% size%+d first+0x%X" % (q["pct"], q["size_delta"], q["first"]),
                x.get("wrongc"), x["desc"][:170]))
        L.append("")
    return chr(10).join(L) + chr(10)


def all_parked(a):
    fns = parked_functions()
    skip = set((a.skip or "").split(",")) | (set() if a.out_tag else {"sub_0802AA78", "sub_08037A78"})
    only = set(a.only.split(",")) if a.only else None
    out = os.path.join(REPO, OUT)
    rdir = os.path.join(out, "results" + (("-" + a.out_tag) if a.out_tag else ""))
    os.makedirs(rdir, exist_ok=True)
    rules = parse_rules(a.rules)
    for fn in fns:
        if fn in skip or (only and fn not in only):
            continue
        draft = os.path.join(REPO, "work", fn, fn + ".c")
        print("== %s" % fn, flush=True)
        try:
            res = search(fn, draft, rules, a.seconds, a.max, a.threads, pairs=not a.no_pairs)
            if not res.get("error"):
                finish(res, a.wrongc_max, not a.no_wrongc)
                print_report(res, top=3)
                if any(x["r"]["state"] == "MATCH" for x in res["ranked"]):
                    print("!!! MATCH in %s" % fn)
        except SystemExit as e:
            res = {"fn": fn, "error": str(e)}
            print("  " + str(e))
        except Exception as e:  # noqa: BLE001
            res = {"fn": fn, "error": "%s: %s" % (type(e).__name__, e)}
            print("  crashed: " + res["error"])
        with open(os.path.join(rdir, fn + ".json"), "w") as fh:
            json.dump(record_of(res), fh)
        recs = []
        for f in fns:
            pth = os.path.join(rdir, f + ".json")
            if os.path.exists(pth):
                recs.append(json.load(open(pth)))
        awlib.write_text(os.path.join(out, "summary%s.md" % (("-" + a.out_tag) if a.out_tag else "")), summary_md(recs, sorted(skip & set(fns))))
        sys.stdout.flush()
    print("wrote build/levers/summary.md")
    return 0


# ------------------------------------------------------------------ chain

def loop_indices(ctx):
    """Names a for loop steps or initialises."""
    out = set()
    for n in ctx.nodes:
        if isinstance(n, c_ast.For):
            for part in (n.init, n.next):
                if part is None:
                    continue
                for m in preorder(part, []):
                    if isinstance(m, c_ast.Assignment) and isinstance(m.lvalue, c_ast.ID):
                        out.add(m.lvalue.name)
                    if isinstance(m, c_ast.UnaryOp) and m.op in ("p++", "p--", "++", "--") \
                            and isinstance(m.expr, c_ast.ID):
                        out.add(m.expr.name)
    return out


def forbidden(x, ctx0):
    """Reason a candidate is never taken by the chain (measured genuinely wrong in wave 97)."""
    idx = loop_indices(ctx0)
    for s in x["sites"]:
        if s.rule == "5b" and s.key.split("<-")[0] in idx:
            return "5b at a loop index (%s)" % s.key.split("<-")[0]
        if s.rule == "1c'":
            kd = ctx0.kinds.get(s.key)
            if kd and kd[0] == "int" and kd[1] == 16 and not kd[2] and s.key in idx:
                return "1c' on the u16 loop index %s" % s.key
    return None


def wrongc_valid(fn, cand, base):
    """(ok, text). `loop-const` on a candidate that only wraps a block in do-while(0) is a warning."""
    try:
        findings, notes = wrongc.analyze_all(fn, cand, base, emu=True)
    except Exception as e:  # noqa: BLE001
        return True, "wrongc failed (%s); not judged" % type(e).__name__
    bad = [f for f in wrongc.verdict(findings) if f[0] == "WRONG"]
    return (not bad), ("; ".join("%s: %s" % (b[1], b[2][:70]) for b in bad) if bad else
                       "OK" + ("" if findings else ""))


def chain(fn, draft, rounds, seconds, threads, beam=1, rules=None, log=print, tries=6):
    """Greedy chain: adopt the best valid candidate, search again from it.

    Returns a dict with the start/final scores, the accepted steps and the final file."""
    wdir = os.path.join(REPO, "work", fn, "levers")
    os.makedirs(wdir, exist_ok=True)
    tmpdir = os.path.join(REPO, OUT, fn)
    os.makedirs(tmpdir, exist_ok=True)
    rules = rules or parse_rules(None)
    cur = os.path.join(wdir, "chain-0.c")
    shutil.copyfile(draft, cur)
    states = [{"path": cur, "steps": [], "score": None}]
    out = {"fn": fn, "steps": [], "matched": False}
    logl = ["chain for %s from %s, up to %d rounds, beam %d" % (fn, draft, rounds, beam)]
    best = states[0]
    for k in range(1, rounds + 1):
        nxt = []
        for st in states:
            res = search(fn, st["path"], rules, seconds, 0, threads, pairs=True)
            if res.get("error"):
                logl.append("round %d: %s" % (k, res["error"]))
                continue
            if st["score"] is None:
                st["score"] = res["base"]
                if "start" not in out:
                    out["start"] = {kk: v for kk, v in res["base"].items() if kk != "sha"}
            ok = [x for x in res["rows"] if x["r"]["state"] in ("MATCH", "MISMATCH")
                  and x["r"]["sha"] != res["base"]["sha"]]
            ok.sort(key=lambda x: (rank_key(x["r"]), len(x["sites"])))
            bk = rank_key(res["base"])
            taken = 0
            for j, x in enumerate([y for y in ok if rank_key(y["r"]) < bk][:tries]):
                why = forbidden(x, res["ctx0"])
                if why:
                    logl.append("round %d: skip %s: %s" % (k, x["label"], why))
                    continue
                cand = os.path.join(tmpdir, "chain-try-%d-%d.c" % (k, j))
                awlib.write_text(cand, res["src"].render(x["ctx"].body, x["ctx"].head_subs))
                valid, why = wrongc_valid(fn, cand, st["path"])
                logl.append("round %d: %s %s wrongc %s  [%s]" % (
                    k, x["label"], fmt_row(x, res["base"]).split(None, 1)[1] if x["r"]["state"] != "MATCH" else "MATCH",
                    why, describe(x)[:140]))
                if not valid:
                    continue
                dest = os.path.join(wdir, "chain-%d%s.c" % (k, "" if not nxt else "-%s" % chr(97 + len(nxt))))
                shutil.copyfile(cand, dest)
                nxt.append({"path": dest, "steps": st["steps"] + [{"round": k, "label": x["label"], "desc": describe(x),
                                                                    "r": {kk: v for kk, v in x["r"].items() if kk != "sha"}}],
                            "score": {kk: v for kk, v in x["r"].items() if kk != "sha"}, "key": rank_key(x["r"])})
                taken += 1
                if taken >= beam or x["r"]["state"] == "MATCH":
                    break
        if not nxt:
            logl.append("round %d: no valid improving candidate; stop" % k)
            break
        nxt.sort(key=lambda z: z["key"])
        states = nxt[:beam]
        best = states[0]
        if best["score"]["state"] == "MATCH":
            out["matched"] = True
            logl.append("round %d: MATCH" % k)
            break
    final = os.path.join(wdir, "chain-best.c")
    if best["steps"]:
        shutil.copyfile(best["path"], final)
        out["final"] = best["score"]
        out["steps"] = best["steps"]
        out["path"] = final
    logl.append("result: %s" % ("MATCH" if out["matched"] else ("%s" % (best["score"] and drafts.fmt(best["score"])))))
    awlib.write_text(os.path.join(wdir, "chain.log"), chr(10).join(logl) + chr(10))
    out["log"] = logl
    return out


def chain_row(o):
    if not o.get("steps"):
        s = o.get("start") or {}
        return "| %s | %s | no valid improvement | - | - |" % (
            o["fn"], "%.1f%% %+d" % (s.get("pct", 0), s.get("size_delta", 0)) if s.get("state") == "MISMATCH" else s.get("state", "?"))
    s, f = o["start"], o["final"]
    fs = "MATCH" if f["state"] == "MATCH" else "%.1f%% %+d first+0x%X" % (f["pct"], f["size_delta"], f["first"])
    return "| %s | %.1f%% %+d | %s | %d | %s |" % (
        o["fn"], s.get("pct", 0), s.get("size_delta", 0), fs, len(o["steps"]),
        " ; ".join(x["desc"].replace("|", "/")[:90] for x in o["steps"]))


def chain_all(a):
    fns = parked_functions()
    hold = set((a.skip or "").split(",")) | {
        "sub_08055768", "sub_080607E8", "sub_08057164", "sub_080303C8", "sub_0801FAC4", "sub_0801F234",
        "sub_0805D344", "sub_08046A84", "sub_0802F6A0", "sub_08026290", "sub_08020754", "sub_0801E9B0",
        "sub_08057BDC", "sub_0805D888", "sub_0804A760", "sub_0801E508"}
    only = set(a.only.split(",")) if a.only else None
    rdir = os.path.join(REPO, OUT, "chain")
    os.makedirs(rdir, exist_ok=True)
    for fn in fns:
        if fn in hold or (only and fn not in only):
            continue
        draft = os.path.join(REPO, "work", fn, fn + ".c")
        print("== %s" % fn, flush=True)
        try:
            o = chain(fn, draft, a.chain, a.seconds, a.threads, beam=a.beam)
        except SystemExit as e:
            o = {"fn": fn, "steps": [], "error": str(e)}
        except Exception as e:  # noqa: BLE001
            o = {"fn": fn, "steps": [], "error": "%s: %s" % (type(e).__name__, e)}
        print("  " + (o.get("error") or chain_row(o)), flush=True)
        if o.get("matched"):
            print("!!! MATCH in %s: %s" % (fn, o.get("path")), flush=True)
        o.pop("log", None)
        with open(os.path.join(rdir, fn + ".json"), "w") as fh:
            json.dump(o, fh)
        rows = []
        for f in fns:
            pth = os.path.join(rdir, f + ".json")
            if os.path.exists(pth):
                rows.append(json.load(open(pth)))
        good = sorted([r for r in rows if r.get("steps")], key=lambda r: -(
            101 if r["final"]["state"] == "MATCH" else r["final"]["pct"]) + r["start"].get("pct", 0))
        L = ["# levers.py chain summary", "", "| function | start | final | rounds | levers |", "|---|---|---|---|---|"]
        L += [chain_row(r) for r in good] + [chain_row(r) for r in rows if not r.get("steps") and not r.get("error")]
        awlib.write_text(os.path.join(REPO, OUT, "chain-summary.md"), chr(10).join(L) + chr(10))
    return 0


# --------------------------------------------------------------- self-test

SELF_CASES = [
    ("sub_08045FC8", "work/sub_08045FC8/sub_08045FC8.w97-start.c", "(i * 2 + 6) * 8 (rule 1a)"),
    ("sub_0806AB9C", "work/sub_0806AB9C/sub_0806AB9C.w97-start.c", "u16 x -> s16 x (rule 2)"),
    ("sub_080726E8", "work/sub_080726E8/sub_080726E8.w93-start.c", "nx/ux copies (rules 3, 5a, 1d)"),
    ("sub_0802216C", "work/sub_0802216C/sub_0802216C.w97-start.c", "s16 tn copy + r + tn + K (rules 1c, 1d)"),
]


def synthetic_checks():
    """Parse/regenerate/round-trip checks that need no compiler."""
    ok = True

    def check(name, cond):
        nonlocal ok
        print("[self-test] %s: %s" % (name, "PASS" if cond else "FAIL"))
        ok = ok and cond
    text = """#include "global.h"
void sub_TEST(u16 *dst, int n)
{
    int i;
    u16 t;
    t = 3;
    for (i = 1; i < n; i++)
    {
        if (t != 0)
            sub_X(i * 16 + 0x30, (t & 1) ? i + 1 : 3);
        if (t <= 5)
            t = 1;
        else
            dst[i] = i * 16 + 0x30 + t;
    }
}
"""
    p = os.path.join(REPO, OUT, "_synthetic.c")
    os.makedirs(os.path.dirname(p), exist_ok=True)
    awlib.write_text(p, text)
    src = Source("sub_TEST", p)
    ctx = Ctx(normalize(copy.deepcopy(src.fd.body)), src)
    S = enumerate_sites(ctx)
    rules = {s.rule for s in S}
    for r in ("1a", "1b", "1c", "1c'", "1d", "2", "3a", "3b", "3c", "3d", "4a", "4b", "4c", "5a", "5d"):
        check("a site for rule %s is enumerated" % r, r in rules)
    bad = 0
    for s in S:
        c = apply_sites(ctx, [s])
        if c is None:
            bad += 1
            continue
        out = src.render(c.body, c.head_subs)
        try:
            q = Source("sub_TEST", p)  # noqa: F841
            from pycparser import CParser
            wrongc.parse_function(out[out.index("void sub_TEST"):])
        except Exception:
            bad += 1
    check("every synthetic site applies and re-parses (%d sites)" % len(S), bad == 0)
    tests = {
        "1a": None,
    }
    descs = " ".join(s.desc for s in S)
    check("1a produces (i * 2 + 6) * 8", "((i * 2) + 6) * 8" in descs)
    check("1b produces -~i", "-(~i)" in descs)
    return ok


def self_test(a):
    ok = synthetic_checks()
    found = 0
    print()
    for fn, draft, what in SELF_CASES:
        p = os.path.join(REPO, draft)
        if not os.path.exists(p):
            print("[self-test] %s: pre-match draft %s is gone; skipped" % (fn, draft))
            continue
        try:
            res = search(fn, p, parse_rules(None), a.seconds, None, a.threads, pairs=True)
        except SystemExit as e:
            print("[self-test] %s: %s" % (fn, e))
            continue
        if res.get("error"):
            print("[self-test] %s: %s" % (fn, res["error"]))
            continue
        finish(res, 2, False, write=False)
        hit = [x for x in res["ranked"] if x["r"]["state"] == "MATCH"]
        best = res["ranked"][0] if res["ranked"] else None
        print("[self-test] %s: %s   (looking for %s; draft %s; %d sites, %d singles, %d pairs, %ss)" % (
            fn, "REDISCOVERED " + hit[0]["label"] + ": " + describe(hit[0]) if hit else "not found",
            what, drafts.fmt(res["base"]), res["n_sites"], res["n_singles"], res.get("n_pairs", 0), res["seconds"]))
        if best and not hit:
            print("            best: %s   %s" % (fmt_row(best, res["base"]), describe(best)[:160]))
        found += bool(hit)
    print("[self-test] rediscovered %d of %d matched functions" % (found, len(SELF_CASES)))
    return 0 if ok else 1


# ---------------------------------------------------------------------- main

def main():
    doc = __doc__.split("\n\n", 1)
    ap = argparse.ArgumentParser(description=doc[0], epilog=doc[1],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("fn", nargs="?")
    ap.add_argument("--draft", help="file to search (default work/<fn>/<fn>.c)")
    ap.add_argument("--rules", help="comma list, e.g. 1,2,4a (default: all)")
    ap.add_argument("--max", type=int, default=0, help="compile at most N single-lever candidates")
    ap.add_argument("--seconds", type=float, default=300, help="time bound for the function (default 300)")
    ap.add_argument("--threads", type=int, default=2, help="compile threads (default 2)")
    ap.add_argument("--no-pairs", action="store_true", help="singles only")
    ap.add_argument("--no-wrongc", action="store_true", help="skip the wrongc verdicts")
    ap.add_argument("--wrongc-max", type=int, default=8, help="wrongc on the best N improving candidates (default 8)")
    ap.add_argument("--top", type=int, default=25, help="rows to print")
    ap.add_argument("--all-parked", action="store_true", help="every function in data/parked.json")
    ap.add_argument("--skip", help="comma list of functions to skip with --all-parked")
    ap.add_argument("--only", help="comma list of functions to run with --all-parked")
    ap.add_argument("--force-alias", action="store_true", help="rule 7 even where the ROM pool has one word for the address")
    ap.add_argument("--out-tag", help="suffix for build/levers/results-<tag>/ and summary-<tag>.md")
    ap.add_argument("--chain", type=int, default=0, metavar="N", help="greedy chain: adopt the best VALID candidate, search again from it, up to N rounds")
    ap.add_argument("--beam", type=int, default=1, help="chain beam width (default 1)")
    ap.add_argument("--self-test", action="store_true")
    a = ap.parse_args()
    global FORCE_ALIAS
    FORCE_ALIAS = a.force_alias
    if a.self_test:
        return self_test(a)
    if a.all_parked and a.chain:
        return chain_all(a)
    if a.all_parked:
        return all_parked(a)
    if not a.fn:
        ap.error("need <fn>, --all-parked or --self-test")
    draft = os.path.abspath(a.draft) if a.draft else os.path.join(REPO, "work", a.fn, a.fn + ".c")
    if not os.path.exists(draft):
        print("error: %s does not exist" % draft)
        return 2
    if a.chain:
        o = chain(a.fn, draft, a.chain, a.seconds, a.threads, beam=a.beam, rules=parse_rules(a.rules))
        print(chr(10).join(o['log']))
        return 0 if o['matched'] else 1
    res = search(a.fn, draft, parse_rules(a.rules), a.seconds, a.max, a.threads, pairs=not a.no_pairs)
    if res.get("error"):
        print(res["error"])
        return 2
    finish(res, a.wrongc_max, not a.no_wrongc)
    print_report(res, top=a.top)
    return 0 if any(x["r"]["state"] == "MATCH" for x in res["ranked"]) else 1


if __name__ == "__main__":
    sys.exit(main())
