#!/usr/bin/env python3
"""Compile a candidate C function and decide whether it matches the original.

The verdict is byte equality of the function's encoded range, plus equality of
its relocations. Bytes alone are not enough: two objects can hold identical
instruction encodings while one calls the wrong function, because an unresolved
`bl` is the same placeholder bytes whatever symbol it points at.

The target comes from the assembled original -- build/functions/<unit>.o -- not
from the ROM, so both sides are relocatable objects and are directly comparable.
A unit may hold several functions, so the target range is sliced out using the
function's address and size from the index.

    python tools/trymatch.py sub_08013AEC [--diff]

Exit status is 0 only on a match, so this can gate a loop.

The LAST line of every per-function run is one machine-readable verdict, so no
caller has to re-parse the prose above it (wave 90's orchestrator did, to rank
102 drafts, and preflight printed `still-fails` for 46 drafts that did not
even compile):

    TRYMATCH MATCH sub_X
    TRYMATCH MISMATCH sub_X pct=97.26 size=+0 first=+0x14
    TRYMATCH COMPILE-FAIL sub_X
    TRYMATCH NO-DRAFT sub_X
    TRYMATCH ERROR sub_X

Exit 1 still covers both MISMATCH and COMPILE-FAIL; the line tells them apart.
parse_result(text) reads it back. A failed compile now DELETES the previous
_cand.bin and candidate object first, so nothing stale is left that a hand
`cmp` could mistake for this run's output (wave 90, W90-P1).

Renamed symbols are the same symbol (wave 90). A relocation against a new
name matches one against the old name when both link to one address: the
`X = Y;` aliases in aw2bhr.lds (`gPlaySt` for gUnknown_03003FC0) and the
`.thumb_set sub_XXXXXXXX, NewName` lines in src/**/*.c
(`GetCampaignMissionId` for sub_08078E14). This holds for `bl` calls as well
as pool words, and in both the per-function and the --unit path. Before
this, `--unit` rejected any unit that used a PR #3 name.
"""

import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import time

import agbenv
import awlib

WORK = os.path.join(awlib.REPO, "work")
# check() compiles <WORK_REL>/<fn>/<fn>.c. Always "work" in real use; only
# the self-test points it (and WORK) at a scratch copy under build/, so the
# mismatch and compile-failure paths can be exercised without touching work/.
WORK_REL = "work"
FUNC_DIR = os.path.join(awlib.REPO, "build", "functions")


def load(path, what):
    if not os.path.exists(path):
        print("error: %s missing -- %s" % (os.path.relpath(path, awlib.REPO), what))
        return None
    with open(path, encoding="utf-8") as fh:
        return json.load(fh)


def promoted_file(fn):
    """The src/decomp path that DEFINES fn, or None if it is not promoted.

    A promoted function keeps its work/<fn>/<fn>.c draft, and this script
    compiles THAT, never the promoted definition. So the moment a promoted
    function's signature changes -- which happens whenever a later wave settles
    a type from newly promoted callers -- the untouched draft starts failing to
    compile with `conflicting types`, and that reads exactly like a regression
    caused by the retype. It is not: the draft is stale. Wave 26 hit this on
    five functions at once.
    """
    path = os.path.join(awlib.DATA_DIR, "promoted.json")
    if not os.path.exists(path):
        return None
    try:
        with open(path, encoding="utf-8") as fh:
            units = json.load(fh)
    except (ValueError, OSError):
        return None
    for u in units:
        if fn in u.get("functions", []):
            return u.get("file")
    return None


def resolve(name):
    recs = load(os.path.join(awlib.DATA_DIR, "functions.json"),
                "run tools/index_functions.py")
    if recs is None:
        return None, None
    key = name.strip().lower()
    rec = next((r for r in recs if r["name"].lower() == key), None)
    if rec is None:
        rec = next((r for r in recs if r["addr_hex"].lower() == key
                    or ("0x%08x" % r["addr"]) == key), None)
    if rec is None:
        print("error: no function %r in the index" % name)
        return None, None

    units = load(os.path.join(FUNC_DIR, "units.json"), "run tools/split_asm.py")
    if units is None:
        return rec, None
    unit = next((u for u in units if rec["name"] in u["functions"]), None)
    if unit is None:
        print("error: %s is in no split unit" % rec["name"])
    return rec, unit


def section_bytes(obj_rel, out_bin_rel):
    """Raw contents of .text, via objcopy."""
    prefix = agbenv.makefile_var("PREFIX") or "arm-none-eabi-"
    rc, _, se = agbenv.run(
        '%sobjcopy -O binary --only-section=.text "%s" "%s"'
        % (prefix, obj_rel, out_bin_rel))
    if rc != 0:
        return None, se
    path = os.path.join(awlib.REPO, out_bin_rel.replace("/", os.sep))
    if not os.path.exists(path):
        return None, "objcopy produced nothing"
    return open(path, "rb").read(), None


def relocations(obj_rel, lo, hi):
    """(offset, type, symbol) for relocations landing inside [lo, hi)."""
    prefix = agbenv.makefile_var("PREFIX") or "arm-none-eabi-"
    rc, so, se = agbenv.run('%sobjdump -r "%s"' % (prefix, obj_rel))
    if rc != 0:
        return None
    out, in_text = [], False
    for ln in so.splitlines():
        s = ln.strip()
        if s.startswith("RELOCATION RECORDS FOR"):
            in_text = "[.text]" in s
            continue
        if not in_text or not s or s.startswith("OFFSET"):
            continue
        parts = s.split()
        if len(parts) < 3:
            continue
        try:
            off = int(parts[0], 16)
        except ValueError:
            continue
        if lo <= off < hi:
            out.append((off - lo, parts[1], parts[2]))
    return out


MAP_SYM = re.compile(r"^\s+0x0*([0-9a-fA-F]{8})\s+([A-Za-z_][A-Za-z0-9_]*)\s*(=\s*\.)?\s*$")


LDS_ABS_SYM = re.compile(r"^([A-Za-z_]\w*)\s*=\s*(0[xX][0-9A-Fa-f]+|\d+)\s*;")


def lds_absolute_symbols():
    """name -> value for symbols the LINKER SCRIPT defines outright.

    aw2bhr.lds opens with two of these -- upstream's own, and passed through
    into aw2bhr.split.lds verbatim by tools/gen_lds.py, which rewrites only the
    `asm/*.o(.text);` lines:

        gNumMusicPlayers = 11;
        gMaxLines = 0;

    They are MP2K's two link-time constants and they are the reason four m4a
    functions POOL a small integer that agbcc would otherwise have emitted as
    `movs rN, #imm8`: the value is not known to the compiler, only to the
    linker. See the "Absolute symbols" chapter of docs/agbcc-codegen.md.

    Read from the script rather than from the ELF because the script is the
    DEFINITION and the ELF is a snapshot of the last link. Editing one of these
    values and then asking trymatch about it before a rebuild would otherwise
    be answered from the stale value -- which is precisely the moment the
    answer matters. Only column-0 assignments to a numeric constant are taken;
    every assignment inside SECTIONS sets a symbol from `.` and is a placement,
    not a constant.
    """
    if lds_absolute_symbols.cache is None:
        out = {}
        try:
            with open(os.path.join(awlib.REPO, "aw2bhr.lds"),
                      encoding="utf-8", errors="replace") as fh:
                for ln in fh:
                    m = LDS_ABS_SYM.match(ln)
                    if not m:
                        continue
                    text = m.group(2)
                    try:
                        # ld reads a leading 0 as octal; Python needs `0o` for
                        # that and raises on a bare `011`. Skip anything this
                        # cannot read rather than letting one odd line take
                        # every match in the repo down with it.
                        out[m.group(1)] = (int(text, 16) if text[:2] in ("0x", "0X")
                                           else int(text, 8) if text[:1] == "0"
                                           and len(text) > 1 else int(text, 10))
                    except ValueError:
                        pass
        except OSError:
            pass
        lds_absolute_symbols.cache = out
    return lds_absolute_symbols.cache


lds_absolute_symbols.cache = None


def symbol_addresses():
    """name -> final linked address, read from the last built ELF or the map.

    Used only to resolve relocations -- but an EMPTY result is not free, and the
    old docstring claiming it was cost wave 21 a false regression. A `.rodata`
    pool-word match is confirmed by resolving the candidate's relocation target
    to an address and comparing it with the ROM word; a symbol whose address is
    not spelled into its name (`gpKeySt`, and every other upstream-named global)
    cannot be resolved without this table, so an empty table silently downgrades
    a MATCH to a 99.2% near-miss that reads exactly like a bad decompilation.
    Four functions were re-derived against that ghost before the cause was found:
    `arm-none-eabi-nm` was simply not on PATH in the shell being used, while the
    MCP server -- which shells out to THIS script -- had it and reported MATCH.
    The two tools appeared to disagree and are the same code.

    So: fall back to aw2bhr.map when nm yields nothing, and say so on stderr
    when both come up empty rather than degrading quietly.
    """
    if symbol_addresses.cache is not None:
        return symbol_addresses.cache
    syms = {}
    if os.path.exists(os.path.join(awlib.REPO, "aw2bhr.elf")):
        prefix = agbenv.makefile_var("PREFIX") or "arm-none-eabi-"
        rc, so, _ = agbenv.run('%snm "aw2bhr.elf"' % prefix)
        if rc == 0:
            for ln in so.splitlines():
                parts = ln.split()
                if len(parts) == 3:
                    try:
                        syms[parts[2]] = int(parts[0], 16)
                    except ValueError:
                        pass
    if not syms:
        # nm missing, or the ELF mid-rebuild by a concurrent agent. The map
        # carries the same absolute addresses in plain text.
        mapfile = os.path.join(awlib.REPO, "aw2bhr.map")
        try:
            with open(mapfile, "r", errors="replace") as fh:
                for ln in fh:
                    m = MAP_SYM.match(ln.rstrip("\n"))
                    if m:
                        syms.setdefault(m.group(2), int(m.group(1), 16))
        except OSError:
            pass
    if not syms:
        sys.stderr.write(
            "trymatch: WARNING -- no symbol table (aw2bhr.elf via nm, then\n"
            "  aw2bhr.map, both empty). Relocations against symbols whose\n"
            "  address is not in their name cannot be resolved, so a .rodata\n"
            "  pool word will be reported as a differing byte even when it\n"
            "  matches. Put the ARM toolchain on PATH, or wait for a\n"
            "  concurrent build to finish, before believing a near-miss.\n")
    # After the warning, never before it: an lds constant is not a substitute
    # for the symbol table and must not suppress the "no symbol table" notice.
    # The script's value WINS over the ELF's, because a stale ELF is exactly
    # what this overlay exists to correct.
    syms.update(lds_absolute_symbols())
    # Wave 90: NAMES, not values. PR #3 renamed globals in aw2bhr.lds
    # (`gPlaySt = gUnknown_03003FC0;`) and functions in promoted C
    # (`.thumb_set sub_08078E14, GetCampaignMissionId`). The ELF comes from the
    # assembly build, so a function renamed only in C is not in it, and a
    # call spelled with the new name could not be resolved at all. Each alias
    # gets its target's address. An existing entry is never overwritten.
    for new, old in symbol_aliases().items():
        if new not in syms and old in syms:
            syms[new] = syms[old]
    symbol_addresses.cache = syms
    return syms


symbol_addresses.cache = None


LDS_ALIAS = re.compile(r"^([A-Za-z_]\w*)\s*=\s*([A-Za-z_]\w*)\s*;")
THUMB_SET = re.compile(r"\.(?:thumb_set|set)\s+([A-Za-z_]\w*)\s*,\s*([A-Za-z_]\w*)")


def symbol_aliases():
    """new name -> old name, for every alias the build defines by name.

    Two sources: column-0 `X = Y;` lines in aw2bhr.lds, and
    `.thumb_set sub_XXXXXXXX, NewName` in src/**/*.c. The second has the old
    name first. Both link the two names to one address, so a relocation
    against either one produces the same bytes.
    """
    if symbol_aliases.cache is None:
        out = {}
        try:
            with open(os.path.join(awlib.REPO, "aw2bhr.lds"),
                      encoding="utf-8", errors="replace") as fh:
                for ln in fh:
                    m = LDS_ALIAS.match(ln)
                    if m:
                        out[m.group(1)] = m.group(2)
        except OSError:
            pass
        for root, _, files in os.walk(os.path.join(awlib.REPO, "src")):
            for f in files:
                if not f.endswith(".c"):
                    continue
                try:
                    text = open(os.path.join(root, f), encoding="utf-8",
                                errors="replace").read()
                except OSError:
                    continue
                # A string literal broken across lines is joined first;
                # src/design.c splits one between the two names.
                text = re.sub(r'"\s*\n\s*"', "", text)
                for old, new in THUMB_SET.findall(text):
                    out.setdefault(new, old)
        symbol_aliases.cache = out
    return symbol_aliases.cache


symbol_aliases.cache = None


def sym_addr(name, syms):
    """Address of `name`, from the last ELF or from the name itself.

    `gen_lds.py` and the data blobs invent a `gUnknown_<addr>` symbol at exactly
    that address, so the name carries the answer. The fallback is not cosmetic:
    once a pool word is CARVED OUT of a data blob and emitted by promoted C
    instead (tools/split_rodata.py), the `gUnknown_<addr>` label goes with it and
    the symbol is absent from the split build's ELF entirely -- so a target
    relocation naming it could not be resolved at all, and every function in the
    class would silently fall back to "relocations differ".
    """
    if name in syms:
        return syms[name]
    m = re.fullmatch(r"gUnknown_(0[0-9A-F]{7})", name)
    return int(m.group(1), 16) if m else None


def section_relocs(obj_rel, section):
    """(offset, type, symbol) for relocations in one named section."""
    prefix = agbenv.makefile_var("PREFIX") or "arm-none-eabi-"
    rc, so, _ = agbenv.run('%sobjdump -r "%s"' % (prefix, obj_rel))
    if rc != 0:
        return []
    out, inside = [], False
    for ln in so.splitlines():
        s = ln.strip()
        if s.startswith("RELOCATION RECORDS FOR"):
            inside = "[%s]" % section in s
            continue
        if not inside or not s or s.startswith("OFFSET"):
            continue
        parts = s.split()
        if len(parts) >= 3:
            try:
                out.append((int(parts[0], 16), parts[1], parts[2]))
            except ValueError:
                pass
    return out


def _thumb_functions():
    """Names of THUMB functions, for the linker's T bit on ABS32 relocs.

    An R_ARM_ABS32 against a THUMB function links to S + A with bit 0 SET --
    the interworking T bit -- so a `.rodata` word holding `.word sub_XXXX`
    lands in the ROM as the address | 1. Resolving `base + addend` alone
    misses every such word by exactly one, which is how a finished
    function-pointer table read as a relocation mismatch for a whole wave
    (sub_0802CDA4, wave 39).
    """
    if _thumb_functions.cache is None:
        names = set()
        try:
            with open(os.path.join(awlib.DATA_DIR, "functions.json"),
                      encoding="utf-8") as fh:
                data = json.load(fh)
            for f in (data["functions"] if isinstance(data, dict) else data):
                if f.get("mode") == "THUMB":
                    names.add(f["name"])
        except (OSError, ValueError, KeyError, TypeError):
            pass
        # A renamed THUMB function (`GetCampaignMissionId` for sub_08078E14)
        # carries the T bit too.
        names.update(new for new, old in symbol_aliases().items() if old in names)
        _thumb_functions.cache = names
    return _thumb_functions.cache


_thumb_functions.cache = None


def pool_word_equivalent(cand_o, rodata_off, rom_addr):
    """ROM word addresses the candidate's `.rodata` reproduces, else None.

    A draft that names a global directly the way the original source did makes
    agbcc park the address in THIS unit's `.rodata` (a `-fforce-addr` pool word),
    so its `.text` pool load relocates against the local section symbol while the
    original relocates against the `gUnknown_<addr>` that data/rodata.s or
    data/data.s supplies. Both link to the same byte once the build places the
    word at that address, which tools/split_rodata.py now does.

    THE UNIT OF ACCEPTANCE IS THE WHOLE SECTION, NOT ONE WORD (wave 39). The
    build places the candidate object's ENTIRE `.rodata` at the carve address
    -- split_rodata.py's own build() comment says placing per word would emit
    the section once per word and overrun the next piece -- so what must equal
    the ROM is every byte the section will occupy, anchored where the `.text`
    reference says its start lands (`rom_addr - rodata_off`). That closes two
    holes at once:

    - A MULTI-WORD blob (sub_0802CDA4's `void (*[4])()` initialiser template,
      four `.word sub_XXXX` entries) was rejected by the old single-word check
      even at 0 differing `.text` bytes, because only the referenced word was
      examined and its T bit was not applied.
    - Two pool words whose ROM homes are NOT adjacent could each pass a
      per-word check and then fail at link time, since the section links
      contiguously. Whole-section comparison rejects that up front.

    Words carrying a relocation resolve the way the linker will (S + A from
    the inline addend, T bit for THUMB functions); words without one must
    match the ROM raw. On success returns the ROM address of EVERY word the
    section covers -- the full `"rodata"` list the promotion must carry.
    """
    rom_path = os.path.join(awlib.REPO, "baserom.gba")
    if not os.path.exists(rom_path):
        return None
    sect_base = rom_addr - rodata_off
    if not (0x08000000 <= sect_base < 0x0A000000):
        return None
    rel = {o: (t, s) for o, t, s in section_relocs(cand_o, ".rodata")}
    prefix = agbenv.makefile_var("PREFIX") or "arm-none-eabi-"
    # One file PER PROCESS. This was a single shared build/probe/_cand_rodata.bin,
    # so two agents' trymatch runs in the same second could read each other's
    # .rodata and reject a real match.
    out_rel = "build/probe/_cand_rodata.%d.bin" % os.getpid()
    out_abs = os.path.join(awlib.REPO, out_rel.replace("/", os.sep))
    os.makedirs(os.path.join(awlib.REPO, "build", "probe"), exist_ok=True)
    rc, _, _ = agbenv.run('%sobjcopy -O binary --only-section=.rodata "%s" "%s"'
                          % (prefix, cand_o, out_rel))
    if rc != 0:
        return None
    try:
        data = open(out_abs, "rb").read()
    except OSError:
        return None
    finally:
        try:
            os.remove(out_abs)
        except OSError:
            pass
    if not data or len(data) % 4 or rodata_off >= len(data):
        return None
    syms = symbol_addresses()
    with open(rom_path, "rb") as fh:
        fh.seek(sect_base - 0x08000000)
        rom = fh.read(len(data))
    if len(rom) != len(data):
        return None
    for off in range(0, len(data), 4):
        rom_word = int.from_bytes(rom[off:off + 4], "little")
        if off in rel:
            typ, sym = rel[off]
            if typ != "R_ARM_ABS32":
                return None
            name, extra = _split_sym(sym)
            # Wave 42, W42-M. A word pointing back into THIS section relocates
            # against the local `.rodata` section symbol, and no symbol table
            # can resolve that: nm does not name section symbols, and the
            # linked ELF's `.rodata` is the FINAL section, not this unit's. It
            # used to fall straight into `return None`, so an otherwise exact
            # function was reported as a differing pool word forever. The
            # section's own base is already known -- it is `sect_base` -- and
            # every word is still compared against the ROM below, so a wrong
            # base cannot pass. Found on sub_08032A00, whose `u8 *v[3]`
            # initialiser template is three pointers into the three string
            # literals emitted just before it in the same section.
            base = sect_base if name == ".rodata" else sym_addr(name, syms)
            if base is None:
                return None
            value = base + extra + int.from_bytes(data[off:off + 4], "little")
            if name in _thumb_functions():
                value |= 1
            if value != rom_word:
                return None
        elif data[off:off + 4] != rom[off:off + 4]:
            return None
    return [sect_base + off for off in range(0, len(data), 4)]


pool_word_equivalent.needed = []


def reloc_equivalent(tgt_fn, cand_fn, t_rel, c_rel, cand_o=None):
    """True if the two sides differ only in which symbol names the same address.

    asm/ is disassembled output, so a pool word holding an address gets
    symbolized as whichever symbol happens to sit at that address. A candidate
    spelling the same address as `base + N` emits a different symbol with the
    difference carried in the inline addend -- ARM .text uses REL, so the addend
    lives in the data word. Both link to the identical byte, but comparing
    symbol names alone calls it a mismatch.

    sub_08011B34 is the case that found this: the original's pool word relocates
    against gUnknown_03000040 with addend 0, the candidate's against
    gUnknown_03000000 with addend 0x40, and 0x03000000 + 0x40 == 0x03000040.
    It was rejected at 39 of 40 bytes despite producing an identical ROM.

    Deliberately strict: same offsets, same types, ABS32 only, both symbols
    known, and every differing byte must fall inside a relocation site.

    ONE ASYMMETRY, added in wave 60 -- the candidate may carry an ABS32 at an
    offset where the TARGET HAS NO RELOCATION AT ALL. That is an ABSOLUTE
    SYMBOL: `asm/` is disassembled ROM, so a pool word holding a link-time
    constant appears there as a bare `.4byte 0x0000000B` with nothing to
    relocate, while the C that produced it names the symbol and emits `.word 0`
    plus `R_ARM_ABS32 gNumMusicPlayers`. Pairing by POSITION made those
    functions unreachable twice over: the length gate rejected them outright,
    and even past it the offsets slipped by one. Four m4a functions sat at one
    differing byte for thirteen waves on this.

    It is accepted only when the symbol is in the real symbol table -- the
    linker script or the last ELF -- and the value the linker will store
    (S + A, T bit for a THUMB function) equals the ROM word exactly. THE
    NAME FALLBACK IN sym_addr() IS DELIBERATELY NOT USED HERE. It invents an
    address for any `gUnknown_<addr>` spelling, so honouring it would accept a
    word that NOTHING DEFINES and hand back a MATCH that cannot be linked --
    converting a visible near-miss into a build break blaming another file.
    Requiring a real definition is what welds this to aw2bhr.lds: the only way
    to make the check pass is to make the link work.

    The reverse shape stays rejected. A relocation on the TARGET side that the
    candidate lacks means the draft baked a ROM address in as a literal, which
    links to the right bytes only by luck and only at this layout.
    """
    if t_rel is None or c_rel is None or len(tgt_fn) != len(cand_fn):
        return False
    if not t_rel and not c_rel:
        return False

    t_by_off, c_by_off = {}, {}
    for side, rels in ((t_by_off, t_rel), (c_by_off, c_rel)):
        for r in rels:
            if r[0] in side:
                return False        # two relocations on one word: not our shape
            side[r[0]] = r
    if set(t_by_off) - set(c_by_off):
        return False

    syms = symbol_addresses()
    sites = []
    for c_off in sorted(c_by_off):
        _, c_typ, c_sym = c_by_off[c_off]
        if c_off not in t_by_off:
            # Candidate-only: an absolute symbol against a bare ROM literal.
            if c_typ != "R_ARM_ABS32" or c_off + 4 > len(tgt_fn):
                return False
            c_name, c_extra = _split_sym(c_sym)
            if c_name not in syms:
                return False
            value = syms[c_name] + c_extra + int.from_bytes(
                cand_fn[c_off:c_off + 4], "little")
            if c_name in _thumb_functions():
                value |= 1
            if value != int.from_bytes(tgt_fn[c_off:c_off + 4], "little"):
                return False
            reloc_equivalent.absolute_syms.append((c_off, c_name, syms[c_name]))
            sites.append(c_off)
            continue
        t_off, t_typ, t_sym = t_by_off[c_off]
        if t_typ != c_typ:
            return False
        if t_typ != "R_ARM_ABS32":
            # Only a literal-pool word can carry an addend difference. Anything
            # else -- in practice a `bl`'s R_ARM_THM_CALL -- has to name the
            # identical symbol, and its bytes are then compared like any other
            # non-relocation bytes by the loop below.
            #
            # This used to `return False` outright, which made the whole check
            # unreachable for any function containing a call: the ABS32 pool
            # words were never examined because the `bl` was rejected first.
            # sub_08011B34, the case this was written for, is a leaf, so the
            # hole survived until family F049 in wave 15 hit it -- four
            # functions reported at 99.1% with one differing byte, all four
            # instruction-for-instruction identical and linking to the same
            # ROM.
            #
            # Wave 90: the same symbol under another NAME is also the same
            # call. PR #3 renamed functions in C (`bl GetCampaignMissionId`
            # where the original has `bl sub_08078E14`), and both names link
            # to one address. Only names in the real symbol table count, and
            # the addend stays in the instruction bytes, which the loop below
            # still compares.
            if t_sym != c_sym:
                t_name, t_extra = _split_sym(t_sym)
                c_name, c_extra = _split_sym(c_sym)
                if (t_typ != "R_ARM_THM_CALL" or t_extra != c_extra
                        or syms.get(t_name) is None
                        or syms.get(t_name) != syms.get(c_name)):
                    return False
            continue
        t_name, t_extra = _split_sym(t_sym)
        c_name, c_extra = _split_sym(c_sym)

        # Both sides name the IDENTICAL symbol, so the two relocations resolve
        # to the same address exactly when the stored addends agree -- and the
        # byte loop below already enforces that, because this offset is never
        # added to `sites`. Nothing needs resolving, which matters because a
        # SECTION symbol has no address to look up: a `switch`'s jump table
        # relocates its words against `.text`, and sym_addr(".text") is None.
        #
        # Without this, one jump table sank the whole check before any pool
        # word was examined -- the same shape as the wave-15 R_ARM_THM_CALL
        # hole above, and the wave-42 `.rodata` one below. Wave 43 found
        # sub_080389D8 reported as "bytes: match, but relocations differ" at
        # 0 of 256 bytes differing, its only true difference the ordinary
        # -fforce-addr pool word that the `.rodata` branch below exists to
        # accept. Every function carrying both a jump table and a pool word
        # was unmatchable.
        if t_sym == c_sym:
            continue

        if t_off + 4 > len(tgt_fn):
            return False
        t_base = sym_addr(t_name, syms)
        if t_base is None:
            return False
        t_addr = t_base + t_extra + int.from_bytes(
            tgt_fn[t_off:t_off + 4], "little")

        # The candidate parked the address in its OWN .rodata -- a -fforce-addr
        # pool word, which is what the original source's spelling produces. The
        # section symbol has no address of its own, so resolve the word's
        # contents against the ROM instead. Requires the build to place that
        # .rodata at t_addr, which is a `rodata` entry in data/promoted.json;
        # see tools/split_rodata.py.
        if c_name == ".rodata" and cand_o is not None:
            rodata_off = c_extra + int.from_bytes(
                cand_fn[c_off:c_off + 4], "little")
            words = pool_word_equivalent(cand_o, rodata_off, t_addr)
            if words is None:
                return False
            pool_word_equivalent.needed.extend(
                (w, rodata_off) for w in words)
            sites.append(t_off)
            continue

        c_base = sym_addr(c_name, syms)
        if c_base is None:
            return False
        c_addr = c_base + c_extra + int.from_bytes(
            cand_fn[c_off:c_off + 4], "little")
        if t_addr != c_addr:
            return False
        sites.append(t_off)

    for i, (a, b) in enumerate(zip(tgt_fn, cand_fn)):
        if a != b and not any(off <= i < off + 4 for off in sites):
            return False
    return True


reloc_equivalent.absolute_syms = []


def _split_sym(field):
    """objdump prints `sym` or `sym+0xN`; return (name, extra)."""
    name, _, rest = field.partition("+")
    try:
        return name, int(rest, 16) if rest else 0
    except ValueError:
        return name, 0


def disassemble(obj_rel, lo, hi):
    prefix = agbenv.makefile_var("PREFIX") or "arm-none-eabi-"
    rc, so, _ = agbenv.run(
        '%sobjdump -d -r -z --start-address=0x%x --stop-address=0x%x "%s"'
        % (prefix, lo, hi, obj_rel))
    if rc != 0:
        return []
    keep = []
    for ln in so.splitlines():
        parts = ln.split("\t")
        if len(parts) < 3:
            continue
        # objdump columns are: address, encoding, mnemonic, operands, comment.
        # Drop the first two -- they are position-dependent and would diff on
        # every line for a function sitting at a different offset -- and drop
        # the trailing `@ (...)` comment, which is just a resolved address.
        text = " ".join(p.strip() for p in parts[2:] if p.strip())
        text = text.split("@")[0].strip()
        if text:
            keep.append(text)
    return keep


def _write_atomic(path, text):
    """awlib.write_text through a temp file and os.replace, so a concurrent
    reader of best.c / best.json never sees a half-written file."""
    tmp = "%s.tmp%d" % (path, os.getpid())
    awlib.write_text(tmp, text)
    os.replace(tmp, path)


# Scores are taken over the LONGER of the two functions. They used to be taken
# over the target only, so a draft that compiled too long was scored on its
# first `size` bytes and could read 100% while 4 bytes too long. best.json
# records written since carry "scored_over"; older ones are corrected on read.
SCORED_OVER = "max(candidate, target)"


byte_score = awlib.byte_score


def recorded_percent(meta):
    """best.json's percent on today's scale (see SCORED_OVER)."""
    pct = meta.get("percent", -1.0)
    cand, tgt = meta.get("candidate_bytes"), meta.get("target_bytes")
    if meta.get("scored_over") != SCORED_OVER and cand and tgt and cand > tgt:
        pct = pct * tgt / cand
    return pct


# Who wrote the source being scored; saved in best.json as "origin".
# permute.py sets "permuter" while it scores candidates, so a later run knows
# not to start from a mutation (which can be wrong C) as if a person wrote it.
RECORD_ORIGIN = "draft"


def record_best(workdir, name, pct, candidate_bytes=None, target_bytes=None,
                compiled=None):
    """Keep the highest-scoring candidate seen, beside the current one.

    An iteration that scores worse overwrites the source that scored better, so
    without this the best attempt is simply lost -- one agent reported reaching
    88.2% and left a 25% regression behind it, with no way back. best.c is never
    read by the build; it exists so a handoff starts from the best known point
    rather than the last one.

    `compiled` is the draft's text as read BEFORE the compile that produced
    `pct`. If the file on disk differs now, something rewrote it during the
    compile (the MCP server rewrites <fn>.c on every try_match; permute.py's
    verify loop does too), and saving it would file one source under another
    source's score. That is skipped with a note instead.
    """
    meta = os.path.join(workdir, "best.json")
    prev = -1.0
    if os.path.exists(meta):
        try:
            with open(meta, encoding="utf-8") as fh:
                prev = recorded_percent(json.load(fh))
        except (ValueError, OSError):
            prev = -1.0
    if pct <= prev:
        print("  best so far: %.1f%% (kept in best.c)" % prev)
        return
    src = os.path.join(workdir, name + ".c")
    text = "".join(awlib.read_lines(src))
    if compiled is not None and text != compiled:
        print("  NOTE: %s.c changed while it was compiling; best.c NOT updated "
              "(the %.1f%% belongs to the earlier text)" % (name, pct))
        return
    _write_atomic(os.path.join(workdir, "best.c"), text)
    # source_sha1 is the only thing that makes an entry self-verifying. Without
    # it best.json is five bare numbers, and nothing links them to the file
    # sitting beside them -- record_best is a non-atomic read-modify-write, and
    # mcp_server.py rewrites <fn>.c at the start of every try_match, so with
    # parallel agents run A can save run B's source under A's score. The
    # 2026-08-29 review could only ever prove that NEGATIVELY, by mtime
    # inversion, and found two (sub_0807E980, sub_0805A0EC) among the top five
    # parked functions. Hash exactly the bytes written: write_text encodes with
    # surrogateescape, and a plain .encode("utf-8") raises on any non-UTF-8
    # byte in a draft instead of hashing it.
    payload = {"percent": round(pct, 2),
               "scored_over": SCORED_OVER,
               "origin": RECORD_ORIGIN,
               "source_sha1": hashlib.sha1(
                   text.encode("utf-8", errors="surrogateescape")).hexdigest()}
    if candidate_bytes is not None and target_bytes is not None:
        payload.update({
            "candidate_bytes": candidate_bytes,
            "target_bytes": target_bytes,
            "size_delta": candidate_bytes - target_bytes,
            "exact_size": candidate_bytes == target_bytes,
        })
    _write_atomic(meta, json.dumps(payload, sort_keys=True) + "\n")
    print("  new best: %.1f%% (saved to best.c)" % pct)


def _looks_like_torn_header(text):
    """A compile failure whose error sits in include/ and names a type
    conflict is, mid-wave, almost always a half-written header seen while
    another agent's edit was in flight -- not a fault in the draft."""
    if not text:
        return False
    if "include" not in text:
        return False
    return bool(re.search(
        r"conflicting types|redefinition|previous declaration|parse error",
        text))


# Byte score of the most recent check(), or None if it never got as far as
# comparing bytes (compile failure, no draft, unreadable unit). check() returns
# only 0/1, and callers that need to know whether a NON-matching candidate was
# an improvement would otherwise have to recompile to find out -- at ~5s each,
# inside a loop over every harvested candidate. Read it immediately after the
# call; anything else that compiles will overwrite it.
LAST_PCT = None

# The whole verdict of the most recent check(), as printed on its TRYMATCH
# line: {"state", "fn", and for MISMATCH "pct", "size", "first"}.
LAST_RESULT = None


def format_result(r):
    """The one machine-readable line check() ends with."""
    line = "TRYMATCH %s %s" % (r["state"], r.get("fn"))
    if r["state"] == "MISMATCH":
        line += " pct=%.2f size=%+d first=+0x%x" % (r["pct"], r["size"], r["first"])
    if r.get("profile") and r["profile"] != "configured":
        line += " profile=%s" % r["profile"]
    return line


def parse_result(text):
    """The verdict dict from the LAST `TRYMATCH` line in text, or None.

    For callers that run trymatch as a subprocess (verify_batch.py,
    preflight.py). None means the run died before it could print one -- treat
    that as ERROR, never as a pass.
    """
    for ln in reversed((text or "").splitlines()):
        m = re.match(r'^TRYMATCH (\S+) (\S+)(.*)$', ln.strip())
        if not m:
            continue
        r = {"state": m.group(1), "fn": m.group(2)}
        for k, v in re.findall(r'(\w+)=(\S+)', m.group(3)):
            try:
                r[k] = (float(v) if k == "pct" else int(v, 16) if k == "first"
                        else int(v) if k == "size" else v)
            except ValueError:
                r[k] = v
        return r
    return None


def check(name, want_diff=False, keep_going=False, profile="configured"):
    """Judge work/<fn>/<fn>.c; 0 = match, 1 = no match or no compile, 2 = cannot test.

    Ends with the TRYMATCH line (format_result); LAST_RESULT holds the same.
    """
    global LAST_RESULT
    LAST_RESULT = {"state": "ERROR", "fn": name, "profile": profile}
    rc = _check(name, want_diff, keep_going, profile)
    if rc == 0:
        LAST_RESULT["state"] = "MATCH"
    print(format_result(LAST_RESULT))
    return rc


def _check(name, want_diff=False, keep_going=False, profile="configured"):
    global LAST_PCT
    LAST_PCT = None          # a stale score is worse than no score
    rec, unit = resolve(name)
    if rec is None or unit is None:
        return 2

    fn = rec["name"]
    LAST_RESULT["fn"] = fn
    workdir = os.path.join(WORK, fn)
    csrc = os.path.join(workdir, fn + ".c")
    if not os.path.exists(csrc):
        LAST_RESULT["state"] = "NO-DRAFT"
        print("error: %s missing -- run `python tools/newfunc.py %s` first"
              % (os.path.relpath(csrc, awlib.REPO), fn))
        return 2

    # Target: assemble the original unit if that has not been done already.
    # Promoted units live outside build/functions, so take the directory from
    # the manifest -- otherwise a function stops being re-verifiable the moment
    # it is promoted, which is precisely when regression checking matters.
    unit_dir = unit.get("dir", "build/functions")
    unit_s = "%s/%s" % (unit_dir, unit["file"])
    unit_o = "%s/%s.o" % (unit_dir, unit["unit"])
    if not os.path.exists(os.path.join(awlib.REPO, unit_o.replace("/", os.sep))):
        rc, _, se = agbenv.assemble(unit_s, unit_o)
        if rc != 0:
            print("error: could not assemble the original unit\n%s" % se[-1500:])
            return 2

    offset = rec["addr"] - int(unit["addr_hex"], 16)
    size = rec["size"]
    if offset < 0:
        print("error: %s sits before its unit start" % fn)
        return 2

    # Candidate.
    profile_suffix = "" if profile == "configured" else "." + profile
    cand_o = "%s/%s/%s%s.o" % (WORK_REL, fn, fn, profile_suffix)
    cand_bin = "%s/%s/_cand%s.bin" % (WORK_REL, fn, profile_suffix)
    # A failed compile used to leave the PREVIOUS object and _cand.bin in
    # place. Wave 90's hand check `cmp`'d that stale _cand.bin against a
    # snapshot and passed a draft that did not compile. None of this run's
    # outputs may exist until this run writes them.
    for stale in (cand_o, cand_bin):
        try:
            os.remove(os.path.join(awlib.REPO, stale.replace("/", os.sep)))
        except OSError:
            pass
    # The text actually compiled, so record_best can refuse to file a
    # different text (rewritten mid-compile) under this compile's score.
    compiled = "".join(awlib.read_lines(csrc))
    rc, so, se = agbenv.compile_c("%s/%s/%s.c" % (WORK_REL, fn, fn), cand_o, fn=fn,
                                  profile=profile)
    if rc != 0 and _looks_like_torn_header(se or so):
        # Mid-wave, several agents append to include/unknown-globals.h and
        # include/unknown-functions.h concurrently, and a compile that starts
        # while a write is in flight sees a half-written header. Wave 30
        # measured 27 such failures in one sweep -- every one passed on a
        # plain re-run -- and the same noise HID two real failures, so the
        # retry has to live here, once, not in every caller's shell loop.
        time.sleep(0.5)
        rc, so, se = agbenv.compile_c(
            "%s/%s/%s.c" % (WORK_REL, fn, fn), cand_o, fn=fn, profile=profile)
        if rc == 0:
            print("note: first compile hit a torn header read "
                  "(concurrent include/ edit); clean on retry")
    if rc != 0:
        LAST_RESULT["state"] = "COMPILE-FAIL"
        print("COMPILE FAILED")
        msg = (se or so).strip().splitlines()
        for ln in msg[-25:]:
            print("  " + ln)
        prom = promoted_file(fn)
        if prom:
            print("  NOTE: %s is already promoted, and this compiled "
                  "work/%s/%s.c, not %s." % (fn, fn, fn, prom))
            print("        A stale draft is the likely cause -- diff the two "
                  "before believing this is a regression. If a header retype "
                  "settled the signature, sync the draft to it.")
        return 1

    tgt, err = section_bytes(unit_o, "%s/%s/_target.bin" % (WORK_REL, fn))
    if tgt is None:
        print("error: could not read target .text: %s" % err)
        return 2
    cand, err = section_bytes(cand_o, cand_bin)
    if cand is None:
        print("error: could not read candidate .text: %s" % err)
        return 2

    tgt_fn = tgt[offset:offset + size]
    cand_fn = cand[:size]

    print("%s   %d bytes expected" % (fn, size))
    if profile != "configured":
        print("  compiler profile: %s (TEMPORARY; canonical artifacts and "
              "best.c are untouched)" % profile)
        if profile == "custom":
            print("  cflags: %s" % agbenv.flags(fn, profile=profile)["CFLAGS"])
    if len(cand) != size:
        print("  size:  candidate is %d bytes, original is %d  (%+d)"
              % (len(cand), size, len(cand) - size))
    else:
        print("  size:  match (%d bytes)" % size)

    same = tgt_fn == cand_fn and len(cand) == size
    pool_word_equivalent.needed = []
    reloc_equivalent.absolute_syms = []
    t_rel = relocations(unit_o, offset, offset + size)
    c_rel = relocations(cand_o, 0, size)
    equivalent = False

    if same:
        if t_rel is not None and c_rel is not None and t_rel != c_rel:
            if reloc_equivalent(tgt_fn, cand_fn, t_rel, c_rel, cand_o):
                equivalent = True
            else:
                same = False
                print("  bytes: match, but relocations differ")
                for side, rels in (("original ", t_rel), ("candidate", c_rel)):
                    for r in rels[:8]:
                        print("    %s +0x%03x %-18s %s" % (side, r[0], r[1], r[2]))
        else:
            print("  relocs: match")
    elif len(cand) == size and reloc_equivalent(tgt_fn, cand_fn, t_rel, c_rel, cand_o):
        same = equivalent = True

    if same:
        if equivalent:
            print("  relocs: name different symbols that resolve to the same"
                  " address")
            # Keyed by OFFSET, not zipped: since wave 60 the candidate may hold
            # one relocation MORE than the target (an absolute symbol against a
            # bare ROM literal), and a positional zip would print every later
            # row against the wrong pair.
            t_by = {r[0]: r[2] for r in t_rel}
            for c_off, _, c_sym in c_rel:
                t_sym = t_by.get(c_off)
                if t_sym is None:
                    print("    +0x%03x  original .4byte 0x%08x (no relocation)"
                          "  candidate %s"
                          % (c_off, int.from_bytes(tgt_fn[c_off:c_off + 4],
                                                   "little"), c_sym))
                elif t_sym != c_sym:
                    print("    +0x%03x  original %s  candidate %s"
                          % (c_off, t_sym, c_sym))
        # A match resting on a `-fforce-addr` pool word is conditional on the
        # build PLACING that word, and the verdict must say so: promoting
        # without the `rodata` entry drops the word or shifts every address
        # after it, and the failure would land in the build rather than here.
        # An ABSOLUTE SYMBOL is the one class where the object's bytes are NOT
        # the ROM's and the match is still exact: the ROM holds a bare literal
        # because the value is a link-time constant, and the object holds a
        # relocation the linker resolves to that same literal. Say which symbol
        # and which value, because the match is conditional on the linker script
        # continuing to define it -- it needs no data/promoted.json entry, but
        # it does need aw2bhr.lds.
        if reloc_equivalent.absolute_syms:
            print("  NOTE: this match resolves an ABSOLUTE SYMBOL defined by "
                  "the linker script.")
            for off, nm, val in reloc_equivalent.absolute_syms:
                print("          +0x%03x  %s = %d  (aw2bhr.lds); the ROM has "
                      "the bare literal" % (off, nm, val))
            print("        Nothing to add to data/promoted.json -- but the "
                  "promotion is correct\n"
                  "        only while aw2bhr.lds defines it at that value.")
        if pool_word_equivalent.needed:
            words = sorted({a for a, _ in pool_word_equivalent.needed})
            print("  NOTE: this match needs its .rodata pool word(s) PLACED.")
            print("        Add to this function's data/promoted.json entry:")
            print('          "rodata": [%s]'
                  % ", ".join('"0x%08X"' % a for a in words))
            print("        then re-run tools/split_rodata.py and "
                  "tools/gen_lds.py before building.")
            print("\nMATCH -- links to identical bytes. The pool word is the"
                  " same address\n  spelled against a different symbol, which"
                  " is a disassembly artefact,\n  not a difference in the ROM.")
        elif reloc_equivalent.absolute_syms:
            print("\nMATCH -- links to identical bytes. The differing word is a"
                  " LINK-TIME\n  constant: the compiler could not know its value"
                  " so it pooled a relocation,\n  and the linker stores exactly"
                  " the literal the ROM already holds.")
        else:
            print("\nMATCH -- byte-for-byte identical to the original")
        LAST_PCT = 100.0
        if profile != "configured":
            print("\nPROVISIONAL PROFILE MATCH -- not promotion-ready.")
            print("  Record an evidence-backed compiler override, regenerate "
                  "the build override file, then re-run with --profile "
                  "configured.")
            print("  override for data/compiler-overrides.json: %s"
                  % json.dumps(agbenv.profile_override(profile)))
        return 0

    n_diff, common, pct = byte_score(tgt_fn, cand, size)
    LAST_PCT = pct
    print("  bytes: %d of %d differ  (%.1f%% identical)"
          % (n_diff, max(size, len(cand)), pct))
    if profile == "configured":
        record_best(workdir, fn, pct, len(cand), size, compiled=compiled)
    else:
        print("  temporary profile result: best.c/best.json not updated")
    first = next((i for i, (a, b) in enumerate(zip(tgt_fn, cand_fn)) if a != b), common)
    LAST_RESULT.update({"state": "MISMATCH", "pct": pct, "size": len(cand) - size,
                        "first": first})
    print("  first difference at +0x%x" % first)

    if want_diff:
        import difflib
        a = disassemble(unit_o, offset, offset + size)
        b = disassemble(cand_o, 0, max(size, len(cand)))
        print("\n--- original / +++ candidate")
        for ln in list(difflib.unified_diff(a, b, lineterm="", n=3))[2:]:
            print("  " + ln)
    else:
        print("\n  re-run with --diff for an instruction-level comparison")
    return 1


def check_unit(name, want_diff=False):
    """Verify a whole linker UNIT: merge every draft in it, compile the merge as
    ONE translation unit, and compare the unit's entire .text with the ROM.

    THIS EXISTS BECAUSE THE PER-FUNCTION ORACLE HAS A BLIND SPOT IT CANNOT CLOSE
    BY CONSTRUCTION. `check()` compiles one function per object, so every call
    it makes is external and carries a relocation. The original source compiled
    the whole unit at once, so a call to a `static` helper defined beside it was
    resolved by the assembler and carries NO relocation at all. Wave 27 hit this
    on AgbMain: it calls a two-byte `for (;;) ;` at 0x08036B48 with a
    locally-resolved `bl`, 2 of 252 bytes differ, and NO draft can fix it --
    for `check()` to see no relocation the helper must be defined in AgbMain's
    own object, while the merged unit may define it exactly once. Seven
    already-matching siblings were stuck behind that, because a unit promotes
    whole or not at all.

    A unit that reproduces its whole .text is STRICTLY STRONGER evidence than
    every function in it matching separately -- it is precisely what the split
    build links. So `promote.py` may accept it. That is not relaxing the gate,
    which is the one thing wave 18 said never to do; it is pointing the gate at
    the thing that makes it true.

    Every function in the unit must have a draft -- EXCEPT an asm-resident one,
    which by definition never will (see below). A unit is all-or-nothing, so any
    other missing draft is a hard failure, not a skip.

    ASM-RESIDENT MEMBERS ARE TREATED AS ALREADY SATISFIED (wave 28). A unit
    holding a function listed in data/asm-resident.json used to be untestable by
    BOTH oracles: `check()` cannot see a locally-resolved call, and `check_unit()`
    demanded a draft that, for an asm-resident function, is never going to exist.
    sub_0802C604 sat behind exactly that -- its unit also holds sub_0802C62A, the
    two-byte `movs r0, r0` that upstream labelled as a function.

    Skipping them CANNOT produce a false pass, which is why this is safe. The
    comparison below is unchanged: the merged C still has to reproduce the unit's
    ENTIRE .text, asm-resident bytes included. So a pass is positive proof that
    the compiler emits those bytes itself (they were alignment padding all along,
    mislabelled by an upstream `thumb_func_start`), and a real hand-written
    assembly member simply makes the unit fail. This converts an exit-2 "cannot
    test" into an honest verdict; it does not lower the bar.
    """
    rec, unit = resolve(name)
    if rec is None or unit is None:
        return 2

    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import index_functions as ixf
    resident = set(ixf.asm_resident())

    run = list(unit["functions"])
    resident_here = [f for f in run if f in resident]
    drafted = [f for f in run if f not in resident]
    print("unit %s  --  %d function(s): %s"
          % (unit["file"], len(run), ", ".join(run)))
    if resident_here:
        print("  asm-resident, no draft required: %s" % ", ".join(resident_here))
        print("  The merged C must still reproduce their bytes as part of the")
        print("  unit's whole .text -- that is the test, not an exemption.")
    if not drafted:
        print("error: every function in this unit is asm-resident")
        print("  There is nothing to compile, so the unit oracle has no verdict")
        print("  to give. This is a splitter question, not a decompilation one.")
        return 2

    missing = [f for f in drafted
               if not os.path.exists(os.path.join(WORK, f, f + ".c"))]
    if missing:
        print("error: no draft for %s" % ", ".join(missing))
        print("  A unit is verified whole or not at all. Draft every function")
        print("  in it (tools/newfunc.py <fn>) before re-running.")
        return 2
    run = drafted

    # The merge is promote.py's, deliberately: verifying a differently-built
    # text than the one that would be promoted would prove nothing about the
    # promotion.
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import promote as promote_mod
    index = promote_mod.load_index()
    text, err = promote_mod.merge(run, index)
    if err:
        print("error: %s" % err)
        return 2

    stem = "u_%s" % run[0]
    src_rel = "build/unitcheck/%s.c" % stem
    obj_rel = "build/unitcheck/%s.o" % stem
    awlib.write_text(os.path.join(awlib.REPO, src_rel.replace("/", os.sep)), text)

    rc, so, se = agbenv.compile_c(src_rel, obj_rel, fn=run[0])
    if rc != 0 and _looks_like_torn_header(se or so):
        time.sleep(0.5)
        rc, so, se = agbenv.compile_c(src_rel, obj_rel, fn=run[0])
        if rc == 0:
            print("note: first compile hit a torn header read "
                  "(concurrent include/ edit); clean on retry")
    if rc != 0:
        print("COMPILE FAILED (the merged unit, not any single draft)")
        for ln in (se or so).strip().splitlines()[-25:]:
            print("  " + ln)
        print("  A merge that fails to compile while every draft compiles alone")
        print("  is usually two drafts defining one struct tag differently, or")
        print("  a declaration disagreeing with a sibling's definition.")
        return 1

    unit_dir = unit.get("dir", "build/functions")
    unit_s = "%s/%s" % (unit_dir, unit["file"])
    unit_o = "%s/%s.o" % (unit_dir, unit["unit"])
    if not os.path.exists(os.path.join(awlib.REPO, unit_o.replace("/", os.sep))):
        arc, _, ase = agbenv.assemble(unit_s, unit_o)
        if arc != 0:
            print("error: could not assemble the original unit\n%s" % ase[-1500:])
            return 2

    # Per unit, not one shared _target.bin/_cand.bin: two --unit runs on
    # different units at once read each other's bytes (wave 90).
    tgt, err = section_bytes(unit_o, "build/unitcheck/%s_target.bin" % stem)
    if tgt is None:
        print("error: could not read target .text: %s" % err)
        return 2
    cand, err = section_bytes(obj_rel, "build/unitcheck/%s_cand.bin" % stem)
    if cand is None:
        print("error: could not read candidate .text: %s" % err)
        return 2

    print("  size:  %s (%d bytes expected, %d produced)"
          % ("match" if len(cand) == len(tgt) else "MISMATCH",
             len(tgt), len(cand)))

    same = (cand == tgt)
    pool_word_equivalent.needed = []
    reloc_equivalent.absolute_syms = []
    t_rel = relocations(unit_o, 0, len(tgt))
    c_rel = relocations(obj_rel, 0, len(cand))
    if same:
        if t_rel is not None and c_rel is not None and t_rel != c_rel:
            pool_word_equivalent.needed = []
            if reloc_equivalent(tgt, cand, t_rel, c_rel, obj_rel):
                print("  relocs: name different symbols that resolve to the"
                      " same address")
            else:
                same = False
                print("  bytes: match, but relocations differ")
                for side, rels in (("original ", t_rel), ("candidate", c_rel)):
                    for r in rels[:8]:
                        print("    %s +0x%03x %-18s %s" % (side, r[0], r[1], r[2]))
        else:
            print("  relocs: match")
    # WAVE 60 (W60-B). This `elif` is the unit-level twin of the one in check(),
    # and its absence was a half-fixed oracle: an ABSOLUTE SYMBOL whose value is
    # not zero leaves the object's bytes DIFFERENT from the ROM's (the ROM holds
    # the resolved literal, the object holds `.word 0` plus the relocation), so
    # `cand == tgt` is False and the relocation logic above never ran. check()
    # would return MATCH for such a function and check_unit -- which is what
    # promotion gates on -- would reject the very same object. The three
    # gNumMusicPlayers readers are exactly that shape; gMaxLines is not, because
    # its value is 0 and the bytes coincide, which is how the gap stayed hidden.
    elif (len(cand) == len(tgt)
          and reloc_equivalent(tgt, cand, t_rel, c_rel, obj_rel)):
        same = True
        print("  relocs: name different symbols that resolve to the same"
              " address")

    if same:
        if reloc_equivalent.absolute_syms:
            print("  NOTE: this match resolves an ABSOLUTE SYMBOL defined by "
                  "the linker script.")
            for off, nm, val in reloc_equivalent.absolute_syms:
                print("          +0x%03x  %s = %d  (aw2bhr.lds); the ROM has "
                      "the bare literal" % (off, nm, val))
        # Same conditional-match rule as check(): a unit resting on a
        # `-fforce-addr` pool word is only correct once the build PLACES that
        # word, and promote.py parses this NOTE to record it.
        if pool_word_equivalent.needed:
            words = sorted({a for a, _ in pool_word_equivalent.needed})
            print("  NOTE: this match needs its .rodata pool word(s) PLACED.")
            print("        Add to this unit's data/promoted.json entry:")
            print('          "rodata": [%s]'
                  % ", ".join('"0x%08X"' % a for a in words))
            print("        then re-run tools/split_rodata.py and "
                  "tools/gen_lds.py before building.")
        print("\nUNIT MATCH -- the whole unit's .text is byte-for-byte"
              " identical.\n  This verifies ALL %d function(s) above, including"
              " any that cannot\n  match alone because the original resolved a"
              " call inside this unit." % len(unit["functions"]))
        if resident_here:
            print("  It also PROVES the compiler emits %s's bytes itself,"
                  % ", ".join(resident_here))
            print("  so that member is alignment padding an upstream")
            print("  thumb_func_start mislabelled, not hand-written assembly.")
            print("  KEEP its data/asm-resident.json entry -- split_asm.py reads")
            print("  that file to know the member is CARRIED by this unit rather")
            print("  than left behind as assembly. Removing it re-breaks the")
            print("  unit with 'mixes promoted and unpromoted functions'.")
            print("  Record the new evidence in the entry instead.")
        return 0

    n_diff = sum(1 for a, b in zip(tgt, cand) if a != b)
    common = min(len(tgt), len(cand))
    print("  bytes: %d of %d differ" % (n_diff, common))
    first = next((i for i, (a, b) in enumerate(zip(tgt, cand)) if a != b), common)
    print("  first difference at +0x%x" % first)
    # Attribute over EVERY member, not just the drafted ones. A first difference
    # landing inside an asm-resident member is the whole diagnosis: it says the
    # compiler does NOT emit those bytes, so the member is real hand-written
    # assembly rather than mislabelled padding, and no draft will ever close it.
    for f in unit["functions"]:
        r = index.get(f)
        if r:
            off = r["addr"] - int(unit["addr_hex"], 16)
            if off <= first < off + r["size"]:
                print("  which is inside %s (+0x%x into it)%s"
                      % (f, first - off,
                         "  <-- ASM-RESIDENT: the compiler does not emit these"
                         " bytes" if f in resident else ""))
    if want_diff:
        import difflib
        a = disassemble(unit_o, 0, len(tgt))
        b = disassemble(obj_rel, 0, len(cand))
        print("\n--- original / +++ candidate")
        for ln in list(difflib.unified_diff(a, b, lineterm="", n=3))[2:]:
            print("  " + ln)
    else:
        print("\n  re-run with --diff for an instruction-level comparison")
    return 1


def self_test():
    """Both halves of the unit oracle, on the case that motivated it.

    A verifier that cannot fail is worse than none, so this asserts the
    NEGATIVE as well: sub_08071918's unit, with the file-scope asm that emits
    its four leading `movs r0, r0` taken back out, must be rejected. The C body
    is byte-exact and it fails only on those 8 bytes of veneer-table padding --
    exactly the kind of near-miss a too-eager unit check would wave through.
    The real draft now carries that asm and matches (see its file comment).
    """
    ok = True

    rc = check_unit("AgbMain")
    good = (rc == 0)
    print("\n[self-test] AgbMain's unit verifies whole (a function that cannot "
          "match alone): %s" % ("PASS" if good else "FAIL"))
    ok &= good

    print()
    rc = _unit_without_padding_asm("sub_08071918", "sub_08071920")
    good = (rc == 1)
    print("\n[self-test] sub_08071918's unit without its padding asm is "
          "REJECTED (leading veneer padding): %s" % ("PASS" if good else "FAIL"))
    ok &= good

    print()
    rc = check_unit("sub_08071918")
    good = (rc == 0)
    print("\n[self-test] sub_08071918's unit WITH its padding asm verifies "
          "(the padding is emitted, not hidden): %s" % ("PASS" if good else "FAIL"))
    ok &= good

    # Wave 28: a unit holding an asm-resident member must produce a VERDICT.
    # Before the fix this returned 2 ("no draft for sub_0802C62A") -- a draft
    # that, for an asm-resident function, was never going to exist, so both
    # oracles were structurally unable to judge the unit and 38 bytes sat parked
    # from wave 24. Asserting == 0 rather than != 2 is deliberate: it also pins
    # the finding that the compiler really does emit those two padding bytes.
    print()
    rc = check_unit("sub_0802C604")
    good = (rc == 0)
    print("\n[self-test] a unit with an ASM-RESIDENT member is judged, not "
          "skipped (sub_0802C604 + sub_0802C62A): %s"
          % ("PASS" if good else "FAIL"))
    ok &= good

    # The exemption must not become a hole: it is scoped to members named in
    # data/asm-resident.json, and every OTHER missing draft is still a hard
    # failure. This used to name sub_08079EA4's unit, which was later drafted
    # whole and CLOSED -- after which the assertion failed on every run and
    # tested nothing. Now WORK points at an empty scratch directory, so every
    # member of a real multi-function unit is missing by construction.
    print()
    ok &= _self_test_missing_draft()

    # Wave 39: a MULTI-WORD .rodata blob (sub_0802CDA4's function-pointer
    # initialiser template -- four `.word sub_XXXX` entries whose linked
    # values carry the THUMB T bit) is accepted, and the promotion hint
    # carries EVERY word. The build places the whole section, so a partial
    # list would overrun the next rodata piece; asserting the exact four
    # addresses pins both the acceptance and the completeness of the hint.
    print()
    rc = check("sub_0802CDA4")
    words = sorted({a for a, _ in pool_word_equivalent.needed})
    good = (rc == 0 and
            words == [0x08090BE4, 0x08090BE8, 0x08090BEC, 0x08090BF0])
    print("\n[self-test] a multi-word .rodata blob is accepted with every "
          "word listed (sub_0802CDA4): %s" % ("PASS" if good else "FAIL"))
    ok &= good

    # Wave 43: a function carrying BOTH a jump table and a -fforce-addr pool
    # word. sub_080389D8's switch relocates its eleven table words against the
    # section symbol `.text`, which has no address for sym_addr() to return, so
    # relocs_equivalent() bailed on the FIRST table word and never reached the
    # `.rodata` branch that the pool word needed. It reported "bytes: match, but
    # relocations differ" at 0 of 256 bytes differing -- a byte-perfect function
    # called a miss, the same failure shape as the wave-15 R_ARM_THM_CALL hole
    # and the wave-42 `.rodata` one. Identically-named symbols are now skipped
    # and left to the byte comparison. Asserting the pool-word hint too keeps
    # the fix from degenerating into "ignore every relocation".
    print()
    rc = check("sub_080389D8")
    words = sorted({a for a, _ in pool_word_equivalent.needed})
    good = (rc == 0 and words == [0x08090F0C])
    print("\n[self-test] a jump table does not mask a .rodata pool word "
          "(sub_080389D8): %s" % ("PASS" if good else "FAIL"))
    ok &= good
    ok &= _self_test_verdict_line()
    ok &= _self_test_alias_call()

    print("\n[self-test] %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


def _unit_without_padding_asm(name, body_name):
    """check_unit(name) on the merged draft with its file-scope `asm(".text...`
    padding and `.thumb_set` alias lines removed and the body renamed back to
    `name` -- the draft as it stood before the padding could be spelled."""
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import promote as promote_mod
    real = promote_mod.merge

    def stripped(run, index):
        text, err = real(run, index)
        if text is not None:
            text = "".join(ln for ln in text.splitlines(keepends=True)
                           if not ln.startswith("asm(\""))
            text = text.replace(body_name, name)
        return text, err

    promote_mod.merge = stripped
    try:
        return check_unit(name)
    finally:
        promote_mod.merge = real


def _self_test_missing_draft():
    global WORK
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import index_functions as ixf
    resident = set(ixf.asm_resident())
    units = load(os.path.join(FUNC_DIR, "units.json"), "run tools/split_asm.py") or []
    unit = next((u for u in units if len(u["functions"]) >= 2
                 and not set(u["functions"]) & resident), None)
    if unit is None:
        print("[self-test] an ordinary missing draft is still a hard failure: "
              "SKIPPED (no multi-function unit)")
        return True
    saved = WORK
    WORK = os.path.join(awlib.REPO, "build", "trymatch-selftest", "empty")
    try:
        os.makedirs(WORK, exist_ok=True)
        rc = check_unit(unit["functions"][0])
    finally:
        WORK = saved
    good = (rc == 2)
    print("\n[self-test] an ordinary missing draft is still a hard failure "
          "(%s, drafts hidden): %s" % (unit["file"], "PASS" if good else "FAIL"))
    return good


def _self_test_alias_call():
    """Wave 90: a `bl` to a renamed function is the same call; a `bl` to a
    different function is not. sub_0802C604's unit (above) is the positive
    case in real objects; this pins the negative, on identical bytes."""
    code = bytes(8)
    alias = next(((new, old) for new, old in sorted(symbol_aliases().items())
                  if old.startswith("sub_") and new in symbol_addresses()
                  and old in symbol_addresses()), None)
    if alias is None:
        print("\n[self-test] a renamed call target is accepted: SKIPPED "
              "(no .thumb_set alias in src/)")
        return True
    new, old = alias
    other = "sub_08078E20" if old != "sub_08078E20" else "sub_08078E14"
    t_rel = [(4, "R_ARM_THM_CALL", old)]
    same = reloc_equivalent(code, code, t_rel, [(4, "R_ARM_THM_CALL", new)])
    diff = reloc_equivalent(code, code, t_rel, [(4, "R_ARM_THM_CALL", other)])
    good = same and not diff
    print("\n[self-test] `bl %s` equals `bl %s`, and `bl %s` does not: %s"
          % (new, old, other, "PASS" if good else "FAIL"))
    return good


def _self_test_verdict_line():
    """Wave 90: the TRYMATCH line, stale-output deletion and best.json's hash.

    Runs on COPIES under build/trymatch-selftest/ (WORK/WORK_REL redirected),
    so the mismatch and compile-failure paths are exercised without writing
    to work/. Built from a matched draft so it cannot rot: one extra function
    after it makes the size +N (MISMATCH), an #error makes it COMPILE-FAIL.
    """
    global WORK, WORK_REL
    fn = "sub_080389D8"
    base = "".join(awlib.read_lines(os.path.join(WORK, fn, fn + ".c")))
    saved = (WORK, WORK_REL)
    WORK_REL = "build/trymatch-selftest"
    WORK = os.path.join(awlib.REPO, "build", "trymatch-selftest")
    d = os.path.join(WORK, fn)
    ok = True
    try:
        os.makedirs(d, exist_ok=True)
        for f in ("best.c", "best.json"):
            if os.path.exists(os.path.join(d, f)):
                os.remove(os.path.join(d, f))

        awlib.write_text(os.path.join(d, fn + ".c"),
                         base + "\nvoid trymatch_selftest_pad(void) {}\n")
        print()
        rc = check(fn)
        r = LAST_RESULT
        meta = json.load(open(os.path.join(d, "best.json"), encoding="utf-8"))
        best = open(os.path.join(d, "best.c"), "rb").read()
        good = (rc == 1 and r["state"] == "MISMATCH" and r["size"] > 0
                and parse_result(format_result(r))["size"] == r["size"]
                and meta.get("source_sha1") == hashlib.sha1(best).hexdigest())
        print("\n[self-test] MISMATCH line, size delta and best.json source_sha1 "
              "(padded copy of %s): %s" % (fn, "PASS" if good else "FAIL"))
        ok &= good

        # A C error, not `#error`: the Makefile's recipe (and so compile_c)
        # pipes cpp into agbcc without pipefail, so a preprocessor error does
        # not fail the build. That is faithful to `make`, and left alone.
        awlib.write_text(os.path.join(d, fn + ".c"),
                         base + "\nint trymatch_selftest = trymatch_selftest_undeclared;\n")
        stale = os.path.join(d, "_cand.bin")
        awlib.write_text(stale, "stale")
        print()
        rc = check(fn)
        good = (rc == 1 and LAST_RESULT["state"] == "COMPILE-FAIL"
                and not os.path.exists(stale) and LAST_PCT is None)
        print("\n[self-test] COMPILE-FAIL line, and the previous _cand.bin is "
              "deleted, not left to be mistaken for this run's: %s"
              % ("PASS" if good else "FAIL"))
        ok &= good
    finally:
        WORK, WORK_REL = saved
    return ok


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("name", nargs="?", help="function name or address")
    ap.add_argument("--diff", action="store_true",
                    help="show an instruction-level diff on mismatch")
    ap.add_argument("--unit", action="store_true",
                    help="verify the whole linker unit at once, merging every "
                         "draft in it -- the only way to verify a function "
                         "that calls a static helper defined beside it")
    ap.add_argument("--profile", choices=agbenv.compiler_profiles(),
                    default="configured",
                    help="temporary compiler experiment; only 'configured' "
                         "uses data/compiler-overrides.json and is canonical")
    ap.add_argument("--self-test", action="store_true",
                    help="check the unit oracle still accepts AgbMain's unit "
                         "and still rejects sub_08071918's")
    ap.add_argument("--cflags-add", action="append", default=[],
                    metavar="=FLAG",
                    help="add one compiler flag on top of --profile, e.g. "
                         "--cflags-add=-fno-gcse (write it with '='; repeat "
                         "or comma-separate for several). The result is the "
                         "temporary profile 'custom'")
    ap.add_argument("--cflags-remove", action="append", default=[],
                    metavar="=FLAG",
                    help="remove one compiler flag, e.g. --cflags-remove=-O2")
    args = ap.parse_args()
    if args.self_test:
        return self_test()
    if not args.name:
        ap.error("give a function name or address")
    add = [x for v in args.cflags_add for x in v.split(",") if x]
    remove = [x for v in args.cflags_remove for x in v.split(",") if x]
    if add or remove:
        try:
            args.profile = agbenv.custom_profile(add, remove, base=args.profile,
                                                 fn=args.name)
        except ValueError as exc:
            ap.error(str(exc))
    if args.unit:
        if args.profile != "configured":
            ap.error("--profile is currently a per-function experiment; "
                     "unit verification must use configured integration flags")
        return check_unit(args.name, want_diff=args.diff)
    return check(args.name, want_diff=args.diff, profile=args.profile)


if __name__ == "__main__":
    sys.exit(main())
