#!/usr/bin/env python3
"""Differential testing for wrongc.py: run two compiled versions of a function
on the same random inputs in a small Thumb interpreter and compare what they DO.

Both versions are compiled with the project's own pipeline (agbenv.compile_c,
so per-function compiler overrides apply), loaded from the ELF .o, and run with

  * arguments drawn from a seeded generator, shaped by the declared parameter
    types (u8 -> 0..255, s16 -> -32768..32767, pointers -> anything);
  * a TOTAL memory model: every address reads a deterministic pseudo-random
    value (small numbers most of the time, so loop bounds stay small and
    branches on flags get both outcomes), writes overlay it, nothing faults;
  * every external call intercepted: it is logged with the arguments the
    callee's prototype says it takes and returns a pseudo-random value that
    depends only on (callee, how many times it was called, seed) -- so two
    versions that call the same things in the same order see the same world;
    __divsi3 and friends are real.

What is compared per seed: the ordered call log (callee, arguments), the
non-stack bytes written (final value per address) and the memory state at every
call (a store moved across a call shows here), and the return value. Stack
writes, callee-saved registers and register allocation are invisible, which is
the point: a `volatile` frame slot, a different register, a different spill do
not count; a dropped multiply, a clobbered loop counter, a shift moved before a
mask, a lost call do.

Limits, stated so nobody trusts a silence too far: only random paths are
covered (the report says what fraction of the base's instructions ran);
functions whose behaviour hangs on an exact magic value are only partly tested;
ARM-state code and unknown instructions stop the run (`SKIP`).
"""

import binascii
import os
import re
import struct
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)

M32 = 0xFFFFFFFF
SENTINEL = 0xFFFF0000
STACK_TOP = 0x5B7F0000
STACK_LO, STACK_HI = 0x5B700000, 0x5B800000
TEXT_BASE = 0x5A500000
SEC_STRIDE = 0x00100000
EXT_BASE, EXT_SLOTS, EXT_STRIDE = 0x02000000, 0x2000, 0x1000
MAX_STEPS = 20000


class EmuError(Exception):
    """Something the interpreter cannot model: the run is SKIPped, not judged."""


# ---------------------------------------------------------------- ELF loading

class Obj:
    def __init__(self, path):
        with open(path, "rb") as fh:
            d = self.data = fh.read()
        if d[:4] != b"\x7fELF" or d[4] != 1 or d[5] != 1:
            raise EmuError("not a 32-bit little-endian ELF: %s" % path)
        (shoff,) = struct.unpack_from("<I", d, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from("<HHH", d, 0x2E)
        self.sections = []
        for i in range(shnum):
            (name, typ, flags, addr, off, size, link, info, align, entsize) = \
                struct.unpack_from("<IIIIIIIIII", d, shoff + i * shentsize)
            self.sections.append(dict(name=name, type=typ, flags=flags, off=off,
                                      size=size, link=link, info=info, entsize=entsize))
        stroff = self.sections[shstrndx]["off"]
        for s in self.sections:
            s["name"] = self._cstr(stroff + s["name"])
        self.symbols = []
        self.symtab_idx = None
        for i, s in enumerate(self.sections):
            if s["type"] == 2:      # SHT_SYMTAB
                self.symtab_idx = i
        if self.symtab_idx is None:
            raise EmuError("no symbol table")
        st = self.sections[self.symtab_idx]
        stroff = self.sections[st["link"]]["off"]
        for k in range(st["size"] // 16):
            nm, val, size, info, other, shndx = struct.unpack_from("<IIIBBH", d, st["off"] + k * 16)
            self.symbols.append(dict(name=self._cstr(stroff + nm), value=val, size=size,
                                     type=info & 15, bind=info >> 4, shndx=shndx))

    def _cstr(self, off):
        end = self.data.index(b"\0", off)
        return self.data[off:end].decode("latin-1")

    def section_bytes(self, idx):
        s = self.sections[idx]
        if s["type"] == 8:      # NOBITS
            return bytes(s["size"])
        return self.data[s["off"]:s["off"] + s["size"]]

    def relocs(self):
        """Yield (target section index, offset, type, symbol index)."""
        for s in self.sections:
            if s["type"] == 9:          # SHT_REL
                for k in range(s["size"] // 8):
                    off, info = struct.unpack_from("<II", self.data, s["off"] + k * 8)
                    yield s["info"], off, info & 0xFF, info >> 8
            elif s["type"] == 4:
                raise EmuError("RELA relocations not supported")


def ext_addr(name):
    return EXT_BASE + (binascii.crc32(name.encode()) % EXT_SLOTS) * EXT_STRIDE


class Image:
    """Loaded program: a code/rodata memory plus the relocation-derived call map."""

    def __init__(self, path, fn):
        self.obj = o = Obj(path)
        self.base = {}
        self.mem = {}                 # section base -> bytearray
        n = 0
        for i, s in enumerate(o.sections):
            if s["flags"] & 2 and s["size"]:        # SHF_ALLOC
                b = TEXT_BASE + n * SEC_STRIDE
                self.base[i] = b
                self.mem[b] = bytearray(o.section_bytes(i))
                n += 1
        self.calls = {}               # offset-in-image address -> external name
        self.jumps = {}               # address of BL -> internal target address
        for tsec, off, typ, sym in o.relocs():
            if tsec not in self.base:
                continue
            addr = self.base[tsec] + off
            S = self._sym_addr(sym)
            sy = o.symbols[sym]
            if typ == 2:              # R_ARM_ABS32
                buf = self.mem[self.base[tsec]]
                A = struct.unpack_from("<I", buf, off)[0]
                struct.pack_into("<I", buf, off, (S + A) & M32)
            elif typ in (10, 30):     # THM_CALL / THM_JUMP24 (bl)
                if sy["shndx"] == 0:
                    self.calls[addr] = sy["name"]
                else:
                    buf = self.mem[self.base[tsec]]
                    h1, h2 = struct.unpack_from("<HH", buf, off)
                    dec = (((h1 & 0x7FF) << 12) | ((h2 & 0x7FF) << 1))
                    if dec & 0x400000:
                        dec -= 0x800000
                    self.jumps[addr] = (S + dec + 4) & M32 & ~1
            elif typ == 3:            # R_ARM_REL32
                pass
            else:
                raise EmuError("relocation type %d not supported" % typ)
        self.entry = None
        for sy in o.symbols:
            if sy["name"] == fn and sy["shndx"] in self.base and sy["shndx"] != 0:
                self.entry = self.base[sy["shndx"]] + (sy["value"] & ~1)
                self.fn_size = sy["size"]
                if not (sy["value"] & 1):
                    raise EmuError("%s is not Thumb code" % fn)
        if self.entry is None:
            raise EmuError("%s not found in the object" % fn)
        self.text_lo = min(self.mem)
        self.text_hi = max(self.mem) + SEC_STRIDE
        self.consts = self._gather_consts()

    def _gather_consts(self):
        """Small constants the code compares/adds with and its literal-pool
        words: values worth feeding it, with their neighbours, so that
        `if (x == 0x3F)` and `if (n > 5)` both get exercised."""
        base = self.entry & ~(SEC_STRIDE - 1)
        buf = self.mem.get(base)
        if buf is None:
            return ()
        lo = self.entry - base
        hi = min(len(buf), lo + max(self.fn_size, 2))
        out = set()
        for o in range(lo, hi - 1, 2):
            hw = buf[o] | (buf[o + 1] << 8)
            top = hw >> 11
            if 4 <= top <= 7:                  # mov/cmp/add/sub rd, #imm8
                out.add(hw & 0xFF)
            elif top == 3 and hw & 0x400:      # add/sub rd, rs, #imm3
                out.add((hw >> 6) & 7)
            elif top == 9:                     # ldr rd, [pc, #imm]: a pool word
                a = ((o + 4) & ~3) + ((hw & 0xFF) << 2)
                if a + 4 <= len(buf):
                    v = struct.unpack_from("<I", buf, a)[0]
                    if v < 0x10000 or v >= 0xFFFF0000:
                        out.add(v)
        vals = set()
        for c in out:
            for d in (-1, 0, 1):
                vals.add((c + d) & M32)
        return tuple(sorted(vals))

    def _sym_addr(self, idx):
        sy = self.obj.symbols[idx]
        if sy["shndx"] == 0:
            return ext_addr(sy["name"])
        if sy["shndx"] in self.base:
            return self.base[sy["shndx"]] + (sy["value"] & ~1 if sy["type"] != 3 else 0)
        return ext_addr(sy["name"])

    def read8(self, a):
        base = a & ~(SEC_STRIDE - 1)
        buf = self.mem.get(base)
        if buf is not None:
            o = a - base
            if o < len(buf):
                return buf[o]
        return None


# ------------------------------------------------------------------- memory

def _mix(x):
    x &= M32
    x ^= x >> 16
    x = (x * 0x7FEB352D) & M32
    x ^= x >> 15
    x = (x * 0x846CA68B) & M32
    x ^= x >> 16
    return x


def shape_word(h):
    """A pseudo-random word biased towards small values."""
    k = h & 7
    v = h >> 3
    if k < 2:
        return v & 1
    if k < 4:
        return v & 0xF
    if k < 6:
        return v & 0xFF
    if k == 6:
        return v & 0xFFFF
    return (v * 0x9E3779B1 ^ (h << 5)) & M32


class Mem:
    def __init__(self, image, seed, garbage=None, consts=()):
        self.img = image
        self.consts = consts
        self.seed = seed
        self.garbage = garbage       # None: unwritten frame bytes read 0; int: random
        self.w = {}                  # address -> byte, written bytes
        self.dirty = 0
        self.defined = set()         # frame bytes a callee is assumed to have filled
        self.tainted = False         # a never-written frame byte was read

    def word_default(self, a):
        h = _mix(self.seed * 0x9E3779B1 + (a >> 2) * 0x85EBCA6B + 0x1234567)
        if self.consts and (h >> 27) < 5:            # ~1 word in 6: a constant the code knows
            return self.consts[(h >> 4) % len(self.consts)]
        return shape_word(h)

    def r8(self, a):
        a &= M32
        b = self.w.get(a)
        if b is not None:
            return b
        b = self.img.read8(a)
        if b is not None:
            return b
        if STACK_LO <= a < STACK_HI:
            # A frame byte nothing wrote: reading it is undefined behaviour, so
            # the run is marked and not judged. It reads as zero meanwhile.
            if a in self.defined or self.garbage is None:
                return 0
            return (_mix(self.garbage * 0xA24BAED5 + (a >> 2) * 0x9FB21C65 + 7)
                    >> ((a & 3) * 8)) & 0xFF
        return (self.word_default(a) >> ((a & 3) * 8)) & 0xFF

    def r16(self, a):
        return self.r8(a) | (self.r8(a + 1) << 8)

    def r32(self, a):
        a &= M32
        if a & 3 == 0 and not self.w and not STACK_LO <= a < STACK_HI:
            b = self.img.read8(a)
            if b is None:
                return self.word_default(a)
        return self.r8(a) | (self.r8(a + 1) << 8) | (self.r8(a + 2) << 16) | (self.r8(a + 3) << 24)

    def wr(self, a, v, n):
        a &= M32
        for i in range(n):
            self.w[(a + i) & M32] = (v >> (8 * i)) & 0xFF
        self.dirty += 1


def in_stack(a):
    return STACK_LO <= a < STACK_HI


# --------------------------------------------------------------- interpreter

def _add(a, b, c):
    r = a + b + c
    res = r & M32
    cf = r >> 32
    vf = ((~(a ^ b) & (a ^ res)) >> 31) & 1
    return res, cf, vf


def _sx(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


class Regs(list):
    """The register file; remembers which of r0-r3 hold a value this run wrote
    (so an argument register nobody set is known to be leftover)."""

    def __init__(self, n):
        list.__init__(self, [0] * n)
        self.defined = [False] * 4

    def __setitem__(self, i, v):
        if i < 4:
            self.defined[i] = True
        list.__setitem__(self, i, v)


class Run:
    def __init__(self, image, seed, args, arity_of, ret_kind, garbage=None, consts=()):
        self.img = image
        self.mem = Mem(image, seed, garbage, consts)
        self.seed = seed
        self.r = Regs(16)
        self.n = self.z = self.c = self.v = 0
        self.log = []                 # ordered observable events
        self.stackviews = []          # frame data handed to callees (weak evidence)
        self.calls_by_name = {}
        self.arity_of = arity_of
        self.ret_kind = ret_kind
        self.steps = 0
        self.covered = set()
        self.r[13] = STACK_TOP
        for i, a in enumerate(args[:4]):
            self.r[i] = a & M32
        for i in range(4):
            self.r.defined[i] = i < len(args)
        # stack arguments sit at [sp, ...] on entry
        for i, a in enumerate(args[4:]):
            self.mem.wr(STACK_TOP + 4 * i, a, 4)
        self.r[14] = SENTINEL | 1
        self.r[15] = image.entry
        if garbage is not None:
            # What the function did not receive and did not set is garbage, and
            # a different garbage each time: a result that changes with it read
            # something uninitialised.
            for i in list(range(len(args), 4)) + list(range(4, 13)):
                self.r[i] = _mix(garbage * 0x2545F491 + i * 0x9E3779B1 + 5)
            for i in range(4):
                self.r.defined[i] = i < len(args)
            self.n, self.z, self.c, self.v = [(_mix(garbage + 31 * k) >> 7) & 1 for k in range(4)]
        self.timeout = False
        self.trace = None
        self.retval = None
        self.done = False

    # ---- helpers
    def setnz(self, v):
        self.n = v >> 31
        self.z = 1 if v == 0 else 0

    def cond(self, cc):
        n, z, c, v = self.n, self.z, self.c, self.v
        return (z == 1, z == 0, c == 1, c == 0, n == 1, n == 0, v == 1, v == 0,
                c == 1 and z == 0, c == 0 or z == 1, n == v, n != v,
                z == 0 and n == v, z == 1 or n != v)[cc]

    def stub(self, name):
        """An intercepted external call at the current point."""
        r = self.r
        if name in ("__divsi3", "__udivsi3", "__modsi3", "__umodsi3", "__aeabi_idiv",
                    "__aeabi_uidiv", "__aeabi_idivmod", "__aeabi_uidivmod"):
            a, b = r[0], r[1]
            if name in ("__divsi3", "__modsi3", "__aeabi_idiv", "__aeabi_idivmod"):
                a, b = _sx(a, 32), _sx(b, 32)
            if b == 0:
                q, m = 0, a
            else:
                q = abs(a) // abs(b)
                if (a < 0) != (b < 0):
                    q = -q
                m = a - q * b
            r[0] = (m if "mod" in name and "aeabi" not in name else q) & M32
            if "aeabi" in name and "mod" in name:
                r[1] = m & M32
            return
        if name.startswith("__call_via_r") or name.startswith("_call_via_r"):
            target = r[int(name.rsplit("r", 1)[1])]
            label = "*indirect"
            self._call_log(label, (1, True), extra=("target", target & ~1))
            return
        self._call_log(name, self.arity_of(name))

    def _call_log(self, name, arity, extra=None):
        r = self.r
        cnt = self.calls_by_name.get(name, 0)
        self.calls_by_name[name] = cnt + 1
        n, exact = arity
        regs = [r[i] for i in range(4)]
        k = 0
        while k < 4 and r.defined[k]:
            k += 1
        sp = r[13]
        stk = [self.mem.r32(sp + 4 * j) for j in range(max(0, n - 4))] if exact else []
        stackview = []
        for i, a in enumerate(regs):
            if in_stack(a):
                # the callee is assumed to fill what the pointer points at
                self.mem.defined.update(range(a, a + 64))
                # a pointer into the frame: its address depends on the frame's
                # size, its contents (the bytes this run wrote) do not
                stackview.append(tuple(self.mem.w.get(a + j) for j in range(32)))
                regs[i] = "STACK"
        for j, a in enumerate(stk):
            if in_stack(a):
                stk[j] = "STACK"
        nonstack = tuple(sorted((a, b) for a, b in self.mem.w.items() if not in_stack(a)))
        self.log.append(("call", name, tuple(regs), k, n, exact, tuple(stk), extra,
                         hash(nonstack)))
        self.stackviews.append(tuple(stackview))
        h = _mix(self.seed * 0x9E3779B1 + binascii.crc32(name.encode()) + cnt * 0x632BE5AB)
        r[0] = shape_word(h) & M32
        r[1] = shape_word(_mix(h + 1)) & M32
        r.defined[2] = r.defined[3] = False       # r2, r3: clobbered by the call
        return r[0]

    def push(self, v):
        self.r[13] = (self.r[13] - 4) & M32
        self.mem.wr(self.r[13], v, 4)

    # ---- main loop
    def run(self):
        img, mem, r = self.img, self.mem, self.r
        while True:
            pc = r[15]
            if pc & ~1 == SENTINEL:
                self.done = True
                break
            if self.steps >= MAX_STEPS:
                self.timeout = True
                break
            self.steps += 1
            self.covered.add(pc)
            if pc & 1:
                raise EmuError("odd pc")
            buf = img.mem.get(pc & ~(SEC_STRIDE - 1))
            o = pc & (SEC_STRIDE - 1)
            if buf is None or o + 1 >= len(buf):
                raise EmuError("pc outside the object: 0x%X" % pc)
            hw = buf[o] | (buf[o + 1] << 8)
            if self.trace is not None:
                self.trace(pc, hw, r)
            top = hw >> 11
            r[15] = pc + 2
            if top < 3:                       # shift by immediate
                rs, rd = (hw >> 3) & 7, hw & 7
                off = (hw >> 6) & 31
                v = r[rs]
                if top == 0:
                    if off:
                        self.c = (v >> (32 - off)) & 1
                        v = (v << off) & M32
                elif top == 1:
                    if off == 0:
                        self.c = v >> 31
                        v = 0
                    else:
                        self.c = (v >> (off - 1)) & 1
                        v >>= off
                else:
                    if off == 0:
                        off = 32
                    sv = _sx(v, 32)
                    self.c = (sv >> (off - 1)) & 1
                    v = (sv >> off) & M32
                r[rd] = v
                self.setnz(v)
            elif top == 3:                    # add/sub
                rs, rd = (hw >> 3) & 7, hw & 7
                x = (hw >> 6) & 7
                b = x if hw & 0x400 else r[x]
                a = r[rs]
                if hw & 0x200:
                    res, self.c, self.v = _add(a, (~b) & M32, 1)
                else:
                    res, self.c, self.v = _add(a, b, 0)
                r[rd] = res
                self.setnz(res)
            elif top < 8:                     # mov/cmp/add/sub imm8
                rd = (hw >> 8) & 7
                imm = hw & 0xFF
                op = (hw >> 11) & 3
                if op == 0:
                    r[rd] = imm
                    self.setnz(imm)
                elif op == 1:
                    res, self.c, self.v = _add(r[rd], (~imm) & M32, 1)
                    self.setnz(res)
                elif op == 2:
                    res, self.c, self.v = _add(r[rd], imm, 0)
                    r[rd] = res
                    self.setnz(res)
                else:
                    res, self.c, self.v = _add(r[rd], (~imm) & M32, 1)
                    r[rd] = res
                    self.setnz(res)
            elif top == 8:
                if hw & 0x400:                # hi register ops / bx
                    op = (hw >> 8) & 3
                    rs = (hw >> 3) & 15
                    rd = (hw & 7) | ((hw >> 4) & 8)
                    sv = r[rs] if rs != 15 else (pc + 4)
                    if op == 0:
                        dv = r[rd] if rd != 15 else pc + 4
                        r[rd] = (dv + sv) & M32
                        if rd == 15:
                            r[15] &= ~1
                    elif op == 1:
                        res, self.c, self.v = _add(r[rd], (~sv) & M32, 1)
                        self.setnz(res)
                    elif op == 2:
                        r[rd] = sv & M32
                        if rd == 15:
                            r[15] = sv & ~1 & M32
                    else:
                        if r[13] >= STACK_TOP:
                            # the frame is fully popped: this is the function's
                            # return (`pop {r0}; bx r0`), whatever the popped word
                            # is -- an out-of-range store may have overwritten it
                            r[15] = SENTINEL
                        elif rs != 14 and sv & ~1 != SENTINEL:
                            # `bx rN`: a call through a pointer that never returns
                            # here -- log it as the last thing the function does
                            self._call_log("*indirect", (1, True), extra=("target", sv & ~1))
                            r[15] = SENTINEL
                        else:
                            r[15] = sv & ~1 & M32
                else:                         # format 4 ALU
                    self.alu((hw >> 6) & 15, (hw >> 3) & 7, hw & 7)
            elif top == 9:                    # ldr pc-relative
                rd = (hw >> 8) & 7
                a = ((pc + 4) & ~3) + ((hw & 0xFF) << 2)
                r[rd] = mem.r32(a)
            elif top < 12:                    # load/store register offset / sign-ext
                ro, rb, rd = (hw >> 6) & 7, (hw >> 3) & 7, hw & 7
                a = (r[rb] + r[ro]) & M32
                if not hw & 0x200:
                    L, B = hw & 0x800, hw & 0x400
                    if L:
                        r[rd] = mem.r8(a) if B else mem.r32(a)
                    else:
                        mem.wr(a, r[rd], 1 if B else 4)
                else:
                    op = (hw >> 10) & 3
                    if op == 0:
                        mem.wr(a, r[rd], 2)
                    elif op == 1:
                        r[rd] = _sx(mem.r8(a), 8) & M32
                    elif op == 2:
                        r[rd] = mem.r16(a)
                    else:
                        r[rd] = _sx(mem.r16(a), 16) & M32
            elif top < 16:                    # load/store imm5 (word/byte)
                rb, rd = (hw >> 3) & 7, hw & 7
                off = (hw >> 6) & 31
                B, L = hw & 0x1000, hw & 0x800
                a = (r[rb] + (off if B else off << 2)) & M32
                if L:
                    r[rd] = mem.r8(a) if B else mem.r32(a)
                else:
                    mem.wr(a, r[rd], 1 if B else 4)
            elif top < 18:                    # halfword imm
                rb, rd = (hw >> 3) & 7, hw & 7
                a = (r[rb] + (((hw >> 6) & 31) << 1)) & M32
                if hw & 0x800:
                    r[rd] = mem.r16(a)
                else:
                    mem.wr(a, r[rd], 2)
            elif top < 20:                    # sp-relative
                rd = (hw >> 8) & 7
                a = (r[13] + ((hw & 0xFF) << 2)) & M32
                if hw & 0x800:
                    r[rd] = mem.r32(a)
                else:
                    mem.wr(a, r[rd], 4)
            elif top < 22:                    # add rd, pc/sp, imm
                rd = (hw >> 8) & 7
                base = ((pc + 4) & ~3) if not hw & 0x800 else r[13]
                r[rd] = (base + ((hw & 0xFF) << 2)) & M32
            elif top < 24:
                if hw & 0xFF00 == 0xB000:      # add sp, imm
                    d = (hw & 0x7F) << 2
                    r[13] = (r[13] - d if hw & 0x80 else r[13] + d) & M32
                elif hw & 0xF600 == 0xB400:    # push/pop
                    self.pushpop(hw)
                else:
                    raise EmuError("unsupported 0x%04X" % hw)
            elif top < 26:                    # ldm/stm
                rb = (hw >> 8) & 7
                a = r[rb]
                regs = [i for i in range(8) if hw & (1 << i)]
                if hw & 0x800:
                    for i in regs:
                        r[i] = mem.r32(a)
                        a += 4
                    if not (hw & (1 << rb)):
                        r[rb] = a & M32
                else:
                    for i in regs:
                        mem.wr(a, r[i], 4)
                        a += 4
                    r[rb] = a & M32
            elif top < 28:                    # conditional branch / swi
                cc = (hw >> 8) & 15
                if cc == 15:
                    raise EmuError("swi")
                if cc == 14:
                    raise EmuError("undefined")
                if self.cond(cc):
                    r[15] = (pc + 4 + (_sx(hw & 0xFF, 8) << 1)) & M32
            elif top == 28:
                r[15] = (pc + 4 + (_sx(hw & 0x7FF, 11) << 1)) & M32
            elif top == 29:
                raise EmuError("blx")
            else:                             # 11110 / 11111: bl
                self.bl(pc, hw)
        return self

    def pushpop(self, hw):
        r, mem = self.r, self.mem
        regs = [i for i in range(8) if hw & (1 << i)]
        extra = hw & 0x100
        if hw & 0x800:                        # pop
            a = r[13]
            for i in regs:
                r[i] = mem.r32(a)
                a += 4
            if extra:
                r[15] = mem.r32(a) & ~1 & M32
                a += 4
                if a >= STACK_TOP:
                    r[15] = SENTINEL
            r[13] = a & M32
        else:
            n = len(regs) + (1 if extra else 0)
            a = (r[13] - 4 * n) & M32
            r[13] = a
            for i in regs:
                mem.wr(a, r[i], 4)
                a += 4
            if extra:
                mem.wr(a, r[14], 4)

    def alu(self, op, rs, rd):
        r = self.r
        a, b = r[rd], r[rs]
        if op == 0:
            res = a & b
        elif op == 1:
            res = a ^ b
        elif op in (2, 3, 4, 7):
            amt = b & 0xFF
            res = a
            if amt:
                if op == 2:
                    if amt < 32:
                        self.c = (a >> (32 - amt)) & 1
                        res = (a << amt) & M32
                    else:
                        self.c = (a & 1) if amt == 32 else 0
                        res = 0
                elif op == 3:
                    if amt < 32:
                        self.c = (a >> (amt - 1)) & 1
                        res = a >> amt
                    else:
                        self.c = (a >> 31) if amt == 32 else 0
                        res = 0
                elif op == 4:
                    sa = _sx(a, 32)
                    if amt < 32:
                        self.c = (sa >> (amt - 1)) & 1
                        res = (sa >> amt) & M32
                    else:
                        self.c = a >> 31
                        res = M32 if a >> 31 else 0
                else:
                    amt &= 31
                    if amt:
                        res = ((a >> amt) | (a << (32 - amt))) & M32
                    self.c = res >> 31
        elif op == 5:
            res, self.c, self.v = _add(a, b, self.c)
        elif op == 6:
            res, self.c, self.v = _add(a, (~b) & M32, self.c)
        elif op == 8:
            self.setnz(a & b)
            return
        elif op == 9:
            res, self.c, self.v = _add(0, (~b) & M32, 1)
        elif op == 10:
            res, self.c, self.v = _add(a, (~b) & M32, 1)
            self.setnz(res)
            return
        elif op == 11:
            res, self.c, self.v = _add(a, b, 0)
            self.setnz(res)
            return
        elif op == 12:
            res = a | b
        elif op == 13:
            res = (a * b) & M32
        elif op == 14:
            res = a & (~b & M32)
        else:
            res = (~b) & M32
        r[rd] = res & M32
        self.setnz(res & M32)

    def bl(self, pc, hw):
        r, img = self.r, self.img
        if hw & 0x0800:                       # stray second half
            raise EmuError("stray BL half")
        name = img.calls.get(pc)
        hw2 = self.mem.r16(pc + 2)
        if name is not None:
            self.stub(name)
            r[15] = pc + 4
            return
        tgt = img.jumps.get(pc)
        if tgt is None:
            off = ((_sx(hw & 0x7FF, 11) << 12) + ((hw2 & 0x7FF) << 1))
            tgt = (pc + 4 + off) & M32
        r[14] = (pc + 4) | 1
        r[15] = tgt & ~1

    # ---- outcome
    def outcome(self):
        writes = tuple(sorted((a, b) for a, b in self.mem.w.items() if not in_stack(a)))
        ret = self.r[0]
        rk = self.ret_kind
        if rk == "void":
            ret = None
        elif rk == "u8":
            ret &= 0xFF
        elif rk == "s8":
            ret = _sx(ret & 0xFF, 8)
        elif rk == "u16":
            ret &= 0xFFFF
        elif rk == "s16":
            ret = _sx(ret & 0xFFFF, 16)
        return dict(log=tuple(self.log), writes=writes, ret=ret,
                    stack=tuple(self.stackviews))


# ------------------------------------------------------------- argument model

def gen_args(kinds, seed, consts=()):
    out = []
    for i, k in enumerate(kinds):
        h = _mix(seed * 0x2545F491 + i * 0x9E3779B1 + 77)
        w = shape_word(h)
        if consts and (h >> 27) < 6 and k not in ("ptr",):
            w = consts[(h >> 4) % len(consts)]
        if k == "u8":
            w &= 0xFF
        elif k == "s8":
            w = _sx(w & 0xFF, 8) & M32
            if h & 0x1000000:
                w = (-(h >> 25 & 0x7F)) & M32
        elif k == "u16":
            w &= 0xFFFF
        elif k == "s16":
            w = _sx(w & 0xFFFF, 16) & M32
            if h & 0x1000000:
                w = (-(h >> 25 & 0x7F)) & M32
        elif k == "s32" or k == "int":
            if h & 0x1000000:
                w = (-(h >> 25 & 0x7F)) & M32
        out.append(w & M32)
        extra = 1 if k == "ll" else (int(k[1:]) - 1 if re.match(r"w\d+$", k) else 0)
        for j in range(extra):  # a 64-bit or by-value struct argument takes several words
            out.append(shape_word(_mix(h + 99 + j)) & M32)
    return out


# ----------------------------------------------------------------- comparison

def differential(base_o, cand_o, fn, kinds, ret_kind, arity_of, seconds=3.0, min_seeds=8,
                 max_seeds=400):
    """Run both objects on shared seeds. Returns a result dict:
    verdict 'same' | 'differs' | 'skip'; seeds, conclusive, coverage, detail."""
    try:
        bi, ci = Image(base_o, fn), Image(cand_o, fn)
    except EmuError as e:
        return dict(verdict="skip", detail=str(e))
    t0 = time.time()
    consts = tuple(sorted(set(bi.consts) | set(ci.consts)))
    conclusive = timeouts = tainted = 0
    cov_b, cov_c = set(), set()
    seed = 0
    first_diff = first_uninit = weak = None
    ndiff = nuninit = nbaseub = 0

    def outcome_equal(x, y):
        return (logs_equal(x["log"], y["log"]) and x["writes"] == y["writes"]
                and x["ret"] == y["ret"])

    while seed < max_seeds and (seed < min_seeds or time.time() - t0 < seconds):
        seed += 1
        args = gen_args(kinds, seed, consts)
        try:
            rb = Run(bi, seed, args, arity_of, ret_kind, garbage=1, consts=consts).run()
            rc = Run(ci, seed, args, arity_of, ret_kind, garbage=1, consts=consts).run()
        except EmuError as e:
            return dict(verdict="skip", detail=str(e))
        cov_b |= rb.covered
        cov_c |= rc.covered
        if rb.timeout or rc.timeout:
            timeouts += 1
            fin = rc if rb.timeout else rb
            if rb.timeout != rc.timeout and fin.steps < MAX_STEPS // 8:
                ndiff += 1
                first_diff = first_diff or (
                    seed, args, "one version finishes in %d steps, the other is still going "
                    "after %d" % (fin.steps, MAX_STEPS))
            continue
        conclusive += 1
        ob, oc = rb.outcome(), rc.outcome()
        if outcome_equal(ob, oc):
            if weak is None and ob["stack"] != oc["stack"]:
                weak = (seed, args, "data handed to a callee through a frame pointer differs "
                        "(frame layout can also cause this)")
            continue
        # They disagree. Is either one just reading garbage? Run both again with
        # different garbage in the registers and frame it was not given.
        try:
            rb2 = Run(bi, seed, args, arity_of, ret_kind, garbage=2, consts=consts).run()
            rc2 = Run(ci, seed, args, arity_of, ret_kind, garbage=2, consts=consts).run()
        except EmuError as e:
            return dict(verdict="skip", detail=str(e))
        if rb2.timeout or rc2.timeout:
            continue
        ob2, oc2 = rb2.outcome(), rc2.outcome()
        if not outcome_equal(ob, ob2):
            nbaseub += 1                # the base itself reads uninitialised data here
            continue
        if not outcome_equal(oc, oc2):
            nuninit += 1
            first_uninit = first_uninit or (seed, args, describe_diff(oc, oc2))
            continue
        ndiff += 1
        if first_diff is None:
            first_diff = (seed, args, describe_diff(ob, oc))
    verdict = ("differs" if first_diff else "uninit" if first_uninit
               else "same" if conclusive > nbaseub else "skip")
    return dict(verdict=verdict,
                seeds=seed, conclusive=conclusive, timeouts=timeouts, tainted=tainted,
                ndiff=ndiff, nuninit=nuninit, nbaseub=nbaseub, uninit=first_uninit,
                cov_base=len(cov_b), cov_cand=len(cov_c),
                cov_pct=_cov_pct(bi, cov_b), diff=first_diff, weak=weak,
                detail=("the base reads uninitialised data on every run that disagreed"
                        if verdict == "skip" and conclusive else
                        "no run finished within %d steps" % MAX_STEPS if not conclusive else ""))


def _cov_pct(img, covered):
    """Rough share of the function's halfwords that some run executed (pool
    words and padding count against it, so 100 is rarely reached)."""
    if not img.fn_size:
        return None
    n = sum(1 for a in covered if img.entry <= a < img.entry + img.fn_size)
    return min(100, round(200.0 * n / img.fn_size))


def _call_view(e, other):
    """A call entry reduced to what is comparable against `other`: only the
    argument registers the callee can take (its prototype, else those both
    versions had set)."""
    _, name, regs, k, n, exact, stk, extra, mh = e
    if exact:
        m = min(n, 4)
    else:
        m = min(4, k, other[3])
    return (name, regs[:m], stk, extra, mh)


def logs_equal(la, lb):
    if len(la) != len(lb):
        return False
    for x, y in zip(la, lb):
        if _call_view(x, y) != _call_view(y, x):
            return False
    return True


def describe_diff(a, b):
    if a["ret"] != b["ret"]:
        return "return value %s vs %s" % (a["ret"], b["ret"])
    la, lb = a["log"], b["log"]
    for i in range(max(len(la), len(lb))):
        if i >= len(la):
            return "candidate makes an extra call: %s" % (lb[i][1],)
        if i >= len(lb):
            return "candidate never makes call %s" % (la[i][1],)
        vx, vy = _call_view(la[i], lb[i]), _call_view(lb[i], la[i])
        if vx != vy:
            x, y = la[i], lb[i]
            if x[1] != y[1]:
                return "call #%d is %s in the base, %s in the candidate" % (i + 1, x[1], y[1])
            if vx[1] != vy[1]:
                return "call #%d to %s has arguments %s vs %s" % (
                    i + 1, x[1], _hex(vx[1]), _hex(vy[1]))
            if vx[2] != vy[2]:
                return "call #%d to %s has stack arguments %s vs %s" % (
                    i + 1, x[1], _hex(vx[2]), _hex(vy[2]))
            return "call #%d to %s happens with different memory written before it" % (i + 1, x[1])
    wa, wb = dict(a["writes"]), dict(b["writes"])
    for k in sorted(set(wa) | set(wb)):
        if wa.get(k) != wb.get(k):
            return "byte 0x%08X written %s vs %s" % (
                k, "-" if k not in wa else "0x%02X" % wa[k],
                "-" if k not in wb else "0x%02X" % wb[k])
    return "unknown difference"


def _hex(t):
    return "(" + ", ".join(v if isinstance(v, str) else "0x%X" % v for v in t) + ")"


# ---------------------------------------------------------------- prototypes

_PROTO_CACHE = {}
_STRUCT_TEXT = {}


def _headers_text():
    parts = []
    inc = os.path.join(REPO, "include")
    for root, _, files in os.walk(inc):
        for f in sorted(files):
            if f.endswith(".h"):
                try:
                    parts.append(open(os.path.join(root, f), encoding="utf-8",
                                      errors="replace").read())
                except OSError:
                    pass
    return parts


def load_prototypes(extra_text=""):
    """name -> (parameter words, exact), from include/*.h (and any text given)."""
    if "base" not in _PROTO_CACHE:
        texts = _headers_text()
        _STRUCT_TEXT["all"] = "\n".join(re.sub(r"/\*.*?\*/", " ", t, flags=re.S) for t in texts)
        protos = {}
        for t in texts:
            _scan_protos(t, protos)
        _PROTO_CACHE["base"] = protos
    protos = dict(_PROTO_CACHE["base"])
    if extra_text:
        _scan_protos(extra_text, protos)
    return protos


_SCALAR_BYTES = {"u8": 1, "s8": 1, "char": 1, "bool8": 1, "bool": 1, "u16": 2, "s16": 2,
                 "short": 2, "u32": 4, "s32": 4, "int": 4, "long": 4, "float": 4,
                 "u64": 8, "s64": 8, "double": 8}


def _struct_bytes(name, depth=0):
    """Size of `struct name` as defined in the headers, or None."""
    if depth > 4:
        return None
    m = re.search(r"\bstruct\s+%s\s*\{(.*?)\}\s*(?:__attribute__|;|\w)" % re.escape(name),
                  _STRUCT_TEXT.get("all", ""), re.S)
    if not m:
        return None
    off = 0
    for fld in m.group(1).split(";"):
        fld = fld.strip()
        if not fld:
            continue
        fld = re.sub(r"\s*:\s*\d+$", "", fld)
        if "{" in fld or "(" in fld:
            return None
        dims = [int(x, 0) for x in re.findall(r"\[\s*(0x[0-9a-fA-F]+|\d+)\s*\]", fld)]
        if re.search(r"\[\s*[A-Za-z_]", fld):
            return None
        base = re.sub(r"\[.*?\]", "", fld).strip()
        if "*" in base:
            size = 4
        else:
            toks = [t for t in re.findall(r"[A-Za-z_]\w*", base)
                    if t not in ("const", "volatile", "unsigned", "signed")]
            if not toks:
                return None
            if toks[0] == "struct" and len(toks) >= 3:
                size = _struct_bytes(toks[1], depth + 1)
            elif toks[0] in _SCALAR_BYTES:
                size = _SCALAR_BYTES[toks[0]]
            else:
                size = 4
            if size is None:
                return None
        n = 1
        for d in dims:
            n *= d
        align = min(size, 4) or 1
        off = (off + align - 1) // align * align + size * n
    return (off + 3) // 4 * 4


_PROTO_RX = re.compile(r"\b([A-Za-z_]\w*)\s*\(([^()]*(?:\([^()]*\)[^()]*)*)\)\s*;")


def _scan_protos(text, protos):
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    text = re.sub(r"//[^\n]*", " ", text)
    for m in _PROTO_RX.finditer(text):
        name, params = m.group(1), m.group(2).strip()
        # only declarations: something with a type before the name
        pre = text[max(0, m.start() - 60):m.start()]
        if not re.search(r"[\w\*]\s*$", pre) or re.search(r"\b(return|else|=)\s*$", pre):
            continue
        parts, depth, cur = [], 0, ""
        for ch in params:
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
            if ch == "," and depth == 0:
                parts.append(cur)
                cur = ""
            else:
                cur += ch
        parts.append(cur)
        if params in ("", "void"):
            protos.setdefault(name, (0, True))
            continue
        exact, words = True, 0
        for prm in parts:
            prm = prm.strip()
            sm = re.match(r"(?:const\s+)?struct\s+(\w+)\b", prm)
            if prm == "...":
                exact = False
            elif sm and "*" not in prm:
                sz = _struct_bytes(sm.group(1))
                if sz is None:
                    exact = False
                    words += 2
                else:
                    words += max(1, sz // 4)
            elif re.match(r"(long\s+long|s64|u64|double)\b", prm) and "*" not in prm:
                words += 2
            else:
                words += 1
        protos.setdefault(name, (words, exact))


# ------------------------------------------------------------------- driver

def _kind_of(type_node):
    import wrongc
    from pycparser import c_ast
    names, _ = wrongc.type_names(type_node)
    if wrongc._ptr_depth(type_node) or isinstance(type_node, c_ast.ArrayDecl):
        return "ptr"
    ns = " ".join(names)
    if ns.startswith(("struct ", "union ")) and not wrongc._ptr_depth(type_node):
        load_prototypes()
        sz = _struct_bytes(ns.split(None, 1)[1])
        return "w%d" % (max(1, (sz or 8) // 4))     # passed by value: several words
    for k in ("u8", "s8", "u16", "s16"):
        if k in names:
            return k
    if "char" in names:
        return "u8" if "unsigned" in names else "s8"
    if "short" in names:
        return "u16" if "unsigned" in names else "s16"
    if "void" in names:
        return "void"
    if ("long" in names and names.count("long") >= 2) or "s64" in names             or "u64" in names or "double" in names:
        return "ll"
    return "s32" if ns in ("int", "s32") else "word"


def signature(path, fn):
    """(param kinds, return kind) of `fn` as written in the file, or None."""
    import wrongc
    from pycparser import c_ast
    src = wrongc.find_function(wrongc.read_text(path), fn)
    fd = wrongc.parse_function(src) if src else None
    if fd is None:
        return None
    ft = fd.decl.type
    kinds = []
    for p in (ft.args.params if ft.args else []):
        if isinstance(p, c_ast.Decl):
            kinds.append(_kind_of(p.type))
    ret = _kind_of(ft.type)
    return kinds, ret


def compile_obj(path, fn, tag=""):
    """Compile with the project pipeline into build/wrongc/. Returns the
    repo-relative .o path, or raises EmuError with the compiler's message."""
    sys.path.insert(0, HERE)
    import agbenv
    rel = os.path.relpath(os.path.abspath(path), REPO).replace(os.sep, "/")
    safe = re.sub(r"[^A-Za-z0-9_.-]", "_", rel)
    out = "build/wrongc/%s%s.o" % (tag, safe)
    full = os.path.join(REPO, out.replace("/", os.sep))
    if os.path.isfile(full) and os.path.getmtime(full) > os.path.getmtime(path):
        return full
    rc, so, se = agbenv.compile_c(rel, out, fn=fn)
    if rc != 0:
        raise EmuError("does not compile: " + (so + se).strip().splitlines()[-1][:160]
                       if (so + se).strip() else "does not compile")
    return os.path.join(REPO, out.replace("/", os.sep))


def emu_check(fn, base_path, cand_path, seconds=3.0):
    """Differential result dict (see differential()); verdict 'skip' carries detail."""
    try:
        sig = signature(base_path, fn) or signature(cand_path, fn)
        if sig is None:
            return dict(verdict="skip", detail="cannot read the signature")
        kinds, ret = sig
        csig = signature(cand_path, fn)
        if csig is not None and csig != sig:
            return dict(verdict="differs", seeds=0, conclusive=0, timeouts=0,
                        diff=(0, [], "signature changed: %s -> %s" % (sig, csig)),
                        cov_base=0, cov_cand=0)
        bo = compile_obj(base_path, fn)
        co = compile_obj(cand_path, fn)
    except EmuError as e:
        return dict(verdict="skip", detail=str(e))
    protos = load_prototypes(
        open(base_path, encoding="utf-8", errors="replace").read() +
        "\n" + open(cand_path, encoding="utf-8", errors="replace").read())

    def arity_of(name):
        return protos.get(name, (4, False))
    res = differential(bo, co, fn, kinds, ret, arity_of, seconds=seconds)
    res["base_obj"], res["cand_obj"] = bo, co
    return res


_ST_BASE = """
typedef unsigned char u8;
typedef int s32;
extern s32 g(s32);
extern u8 gArr[16];
s32 f(s32 a, s32 b)
{
    s32 i;
    s32 s;
    s = 0;
    for (i = 0; i < a; i++)
    {
        s += g(i) * b;
        gArr[i & 15] = s >> 2;
    }
    return s / 3;
}
"""

# (name, edit, expect "same" or "differs")
_ST_CASES = [
    ("operands swapped", ("g(i) * b", "b * g(i)"), "same"),
    ("temp for the product", ("s += g(i) * b;", "t = g(i) * b;\n        s += t;"), "same"),
    ("multiply dropped", ("g(i) * b", "g(i)"), "differs"),
    ("division changed", ("s / 3", "s / 2"), "differs"),
    ("shift moved before the mask", ("s >> 2", "(s & 0xFF) >> 2 & 0x3F"), "differs"),
    ("call duplicated", ("s += g(i) * b;", "g(i);\n        s += g(i) * b;"), "differs"),
    ("counter clobbered", ("gArr[i & 15]", "i = 0; gArr[i & 15]"), "differs"),
]


def self_test():
    """Seven small edits of one function, compiled with the project's compiler and
    run against the base: the neutral ones must come out SAME, the others DIFFERS."""
    d = os.path.join(REPO, "build", "wrongc", "selftest_emu")
    os.makedirs(d, exist_ok=True)
    base = os.path.join(d, "base.c")
    with open(base, "w", encoding="utf-8") as fh:
        fh.write(_ST_BASE)
    ok = True
    for name, (old, new), want in _ST_CASES:
        text = _ST_BASE.replace(old, new, 1)
        if name == "temp for the product":
            text = text.replace("s32 s;", "s32 s;\n    s32 t;", 1)
        cand = os.path.join(d, "cand.c")
        with open(cand, "w", encoding="utf-8") as fh:
            fh.write(text)
        r = emu_check("f", base, cand, seconds=1.0)
        good = r["verdict"] == want
        print("[self-test] %-32s %s%s" % (name, "PASS" if good else "FAIL",
                                          "" if good else "  got %s %s" % (r["verdict"], r.get("detail", ""))))
        ok = ok and good
    print("[self-test] %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("fn", nargs="?")
    ap.add_argument("base", nargs="?")
    ap.add_argument("cand", nargs="?")
    ap.add_argument("--seconds", type=float, default=3.0)
    ap.add_argument("--self-test", action="store_true")
    a = ap.parse_args(argv)
    if a.self_test:
        return self_test()
    if not (a.fn and a.base and a.cand):
        ap.error("need <fn> <base.c> <candidate.c>")
    r = emu_check(a.fn, a.base, a.cand, a.seconds)
    print({k: v for k, v in r.items() if k not in ("diff", "base_obj", "cand_obj")})
    if r.get("diff"):
        print("seed %d: %s" % (r["diff"][0], r["diff"][2]))
    return 1 if r["verdict"] == "differs" else 0


if __name__ == "__main__":
    sys.exit(main())
