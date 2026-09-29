# sub_0807B7BC — 144 bytes against 156, −12, 9.0%. Waves 48 (W48-E) and 58 (W58-C).

Ignore the 9.0%: it is positional. Being 12 bytes short shifts every byte after
the prologue. The instruction stream is semantically correct end to end —
statement order, the glyph-table walk, the VRAM destination arithmetic, the u16
accumulator, the out-parameter store and the `(u16)` return are all right.

## Wave 58 (W58-C): it is NOT a missing induction variable, and that was measured

W58-B's reverse preheader rule predicts that a loop draft 12 bytes SHORT is
missing a second source induction variable, and this function's ROM has exactly
the shape the rule names — `adds r6, r4, #1` at the top of the outer body,
`adds r4, r6, #0` at the bottom. **It is the compiler's own W47-D increment
hoist, and the unmodified draft already emits it.** From a `compile_probe` of
the draft below, verbatim:

    ldr  r4, .L16          ; g = gUnknown_08616194
    ldrb r1, [r4]
    add  r0, r5, #0x1      ; <-- the hoist of `str++`, unauthored
    mov  r8, r0
    ...
    mov  r5, r8            ; <-- and the copy back

Authoring it anyway (`u8 *next; next = str + 1; ... str = next;`) made it
**worse**: 144 B / 9.0% → 140 B / 3.8%. The authored pseudo takes r8 and evicts
`outWidths` and `total` into two caller-save slots where the ROM has one.

The general form of this is now a chapter in `docs/agbcc-codegen.md`
("A SHORT loop draft is almost never a missing induction variable"): the
compiler's hoist **coalesces** the incremented value back into the counter's
register, a source variable cannot. Position at the top of the body does not
discriminate; the copy-back at the bottom does — and here the draft already has
both.

## The actual residual: a callee-saved / caller-saved split

Same eight live values in both. The ROM spends THREE high registers and
caller-saves one value; the draft spends TWO and caller-saves two.

    ROM   push {r4-r7,lr}; mov r7,sl; mov r6,sb; mov r5,r8; push {r5,r6,r7}
          sub sp,#4
          r4=str  r5=g  r6=str+1  r7=tile  r8=outWidths  sb=total  sl=outTotal
          r2=count, caller-saved around Decompress (one slot)
    here  push {r4-r7,lr}; mov r7,sb; mov r6,r8; push {r6,r7}
          sub sp,#8
          r5=str  r4=g  r8=str+1  r6=tile  r7=count  r9=outTotal
          outWidths and total both caller-saved (two slots)

The 12 bytes are the extra push/pop pair, the extra `mov` at each high-register
use, and the second caller-save pair. The inverted loop-test placement noted in
wave 48 (the ROM duplicates the OUTER test and shares the INNER one; the draft
does the opposite) falls out of the same allocation.

The competition is `str+1` versus `count` for the fourth low callee-saved
register. The ROM gives it to `str+1` (2 references) over `count` (5), which is
backwards from allocno priority — that is the fact a next attempt has to
explain.

## Spellings already measured — do not re-test

All of these compile to byte-identical output:

1. inner loop as `for (g = tbl; g->unk00 != 0; g++)` (this draft)
2. inner loop as `while (g->unk00 != 0) { ...; g++; }`
3. outer loop as `if (*str != 0) { do { ... } while (*str != 0); }` — an explicit
   attempt to author the ROM's duplicated outer test; gcc normalises it straight
   back
4. local declaration order permuted (`u16 total;` first vs `int count;` first) —
   so the allocno tie-break is not the lever
5. (wave 58) assignment order `count = 0; total = 0;` vs the reverse

And measured as strictly worse:

6. (wave 58) `u8 *next;` authored, with `str = next;` at the bottom — 140 B, 3.8%

## The prototype is correct and was verified, not assumed

W48-C declared this from one call site as
`u16 (u8 *, u16 *, u8 *, int, void *)`. The body reads only FOUR argument
registers and never touches `[sp,#0x24]`, so the fifth looks invented from
inside the function — but BOTH ROM call sites (0x0806AB24 and the one at
0x0807BC00 in `code-0806CFC8.s`) push a fifth stack argument before the `bl`.
The parameter is real and simply dead in the body. Leave the declaration alone.

## Next lever

A register-allocation residual, but NOT the permuter's stated case — the sizes
differ, and a 6-instruction prologue delta is the shape the permuter is
documented to be useless on. Attack the pressure: find the spelling that makes
agbcc want `sl`.

## wave 95

Base: the `do { } while (0)` around the whole outer body (148, -8, 5.8%), kept as `sub_0807B7BC.w95-perm1-start.c`; original draft kept as `sub_0807B7BC.w95-start.c` (144, 9.0%). `str++` at the top with the compare re-read (136), `nx = str + 1` temp (144, 10.3%), bare copies of outWidths / outTotal (148): no gain; the copies propagate away.
RESULT: SIZE-EXACT (156), 51.3%, first difference +0xa. Draft = `sub_0807B7BC.w95-perm3-start.c` = current `sub_0807B7BC.c`. Permuter chain (600 s each): 5.8 -> 46.8 -> 51.3 (both semantically identical to the start, checked by reading). What it changed: `total` is `int` (the u16 store at the end truncates identically), a variable `new_var = 0` stands for the zero in the two `!= 0` loop tests, `new_var2 = 0` stands for the NULL test on outTotal, and the return sits inside the `do { } while (0)`. Runs 3 and 4 (58.3, 59.0) were WRONG C: `new_var = tile;` is written inside the glyph loop over the very variable used as the zero constant. Kept as `.w95-WRONG-58.c` / `.w95-WRONG-59.c`. Rewriting that step with a distinct temp (`tc = tile`) is 51.3%, the same as before it, so the improvement was the clobber.
The park's "one register short" is gone: the wrapper plus zero-variable form saves the third high register; what is left is register choice (str in r5 not r4, etc.) inside a size-exact body.
Lever 1: does not transfer as a source form (copies fold away); the permuter's zero-variable is the working equivalent. Comments in the draft are the permuter's, not to be promoted as is.

## wave 97

best.c is WRONG C (wrongc: `new_var = tile` inside the glyph loop, the same clobber as the wave-95 WRONG files); not used.
Base: the size-exact 51.28% draft (w97-start). Rewriting from scratch with the wave-96/97 levers:
* `u16 total` as the ROM has it (per-iteration `lsls/lsrs` into a hi register) and a copy-back step
  (`nx = str + 1` at the top of the outer body, `str = nx` at the bottom) put in front of the wrapper draft: 25%, frame 8.
* Removing the permuter's zero variable only in the inner `for` test (`g->unk00 != 0`) while keeping it in the outer `while`
  and for the outTotal test, plus u16 total and the nx copy-back: **size-exact 156, 57.05%** (was 51.28); kept as e3 below if the
  draft file is that version. Removing the zero variable from all three tests: 152/144 and worse.
* `total = count = 0;` / `count = total = 0;`: byte-identical.
Residual on the 57% form: the ROM keeps `str+1` in r6 and spills only `count` ([sp] = count); we keep `count` in r7, `tile`
in r6 and spill `nx` in r3 across the Decompress call (frame 8 vs 4), and the zero variable still occupies `sl`.

Update (end of wave 97): permuter from the 57.05% form: 57.05 -> 66.03 -> 66.67, size-exact 156, first diff +0xc. Both runs are
valid C (read): `tile & 0x3ff` / `<< 1` / `<< 4` split into temporaries, `0x06010000` and `4` and `8` held in ints, `*str` read
into a u8 before the compare, `nx = str + 1` inside the do-block. Names are still `new_var*`; the residual is unchanged in kind
(callee-saved assignment: the ROM keeps `str+1` in a low register and spills only `count`). Draft file is the 66.67% form.
