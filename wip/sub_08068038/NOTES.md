# sub_08068038 — 176 bytes against 172, +4, 36.6%. Wave 58, W58-C.

Was +28 / 15.7% (wave 44, W44-B). The whole of that 24-byte improvement was one
edit: **the inner scan loop is a `goto` loop, not a `for (;;)`.**

## Everything wave 44 settled is still settled

Read `docs/agbcc-codegen.md`, "A `goto` scan-loop is what stops the inner-loop
rotation", for the chapter version. Wave 44's type model and shape were correct
and are unchanged:

- `gUnknown_08581160` is ONE table of 8-byte records. The splitter's
  `gUnknown_08581164` is `-fforce-addr` folding the `+4` member offset into the
  relocation; the sentinel arm's `ldr r0, [r0, #0x64]` is the same table at
  `12 * 8 + 4`, which fixes the record size at 8.
- `gUnknown_0202F208` is a genuine global word.
- The double compare re-reads `.chr` rather than reusing the register, so it is
  spelled out twice.
- `a2 * 32` is LICM-hoisted; `0x06010000 + i * 512` is per outer iteration; the
  sum groups left.

## What changed

`for (;;)` with two `break`s **rotates**: agbcc duplicates the 0xFF test at the
loop bottom (a second `ldrb; cmp #0xff; bne`). The ROM does not — its continue
path is a bare `b` back to the single copy of the test. Writing the scan as an
if / else-if / else chain with `goto scan` reproduces the ROM's block sequence
exactly, block for block:

    top-test -> cmp-block -> src=tbl[cur].data -> Decompress -> advance -> outer test

Two further things are authored rather than left to the compiler, both read off
the ROM's inner preheader by W58-B's order rule (they precede the scan loop, so
they are source):

    dst  = 0x06010000 + i * 512;    /* lsls r0, ip, #9 ; ldr =0x06010000 ; adds */
    next = i + 1;                   /* mov r7, ip ; adds r7, #1            */

`i = next` (rather than `i++`) is required: it keeps `i` from being a biv, which
is why the ROM has no givs on the outer loop and recomputes `str + i` and
`i * 512` every iteration.

## The remaining 4 bytes — one cause, three symptoms

The goto loop is invisible to `loop.c`. `scan_loop` abandons hoisting after a
label that is jumped to from below, so the two loop-invariant addresses the ROM
holds in the OUTER preheader stay inside the scan loop:

| | ROM | candidate |
|---|---|---|
| `gUnknown_08581160` | hoisted to `sl`, `mov r0, sl` per use | `ldr r6, [pc]` per iteration |
| `&gUnknown_0202F208` | hoisted to `r8`, CSE'd from the pre-loop store | `ldr r0, [pc]` per iteration, plus a `.rodata` force-addr word for the pre-loop store the ROM does not have |
| `.data` member | `ldr r1, =gUnknown_08581164; adds r0, r4, r1` | `adds r0, r6, #4; adds r0, r5, r0` (base is in a register, so no folded pool word) |
| spill choice | `str` on the stack, `a2 * 32` in `sb` | `a2 * 32` on the stack, `str` in `sl` |

**So the ROM wants a real loop's LICM and a goto loop's block layout at the same
time, and nothing probed this wave gives both.** That is the park, and it is a
sharper statement than wave 44's.

## Ruled out this wave, each by controlled `compile_probe`

- `for (;;)` + authored `dst`/`next` + `i = next` (i.e. everything except the
  goto): **still rotates.** The rotation does not depend on `i` being a biv, on
  the increment being authored, or on the destination being hoisted.
- `continue;` vs `goto bottom;` with `bottom: ;` as the last statement of the
  outer body: **byte-identical output.** The candidate's trailing `b` to the
  exit block is not reachable from the source side this way.
- The plain goto loop with `i++` (no authored `next`/`dst`): gets the LICM
  hoists back, but `i` becomes a biv and `strength_reduce` then creates address
  givs for `str[i]` and `i * 512` that the ROM does not have. Strictly worse.

## Not tried

Authoring the two LICM hoists as source locals bound to the symbol before the
loop. This is forbidden by the preheader rule (they sit AFTER the duplicated
outer exit test, which is LICM territory), but the goto loop means LICM will
never place them, so it may be the only route. If a later wave tries it, note
that the ROM's `mov r8, r2` re-uses the address pseudo from the **pre-loop**
`gUnknown_0202F208 = 0` store, so a single bare reference plus a bound local is
the shape to aim for — and that is also what removes the `.rodata` word.

## wave 95

Base: existing draft (176/172, +4, 35.8%), kept as `sub_08068038.w95-start.c`. Result: SIZE-EXACT (172), 41.3%, first difference still +0xc (frame/spill choice). Draft at `sub_08068038.c` is the permuter's run-1 result, checked by reading: same C as the start except the table is read through a local `new_var = gUnknown_08581160;` bound AFTER the first `gUnknown_0202F208 = 0;` (the sentinel `[12].data` and the matching-entry `.data` reads stay on the bare global; the chr compares use the local).
- Pre-registered hypothesis (bind table and cursor addresses before the loop, first store bare): as written it OVERSHOOTS. Both bound: 160 (-12), 14.5%; cursor only: 164 (-8), 21.5%; table only: 160 (-12), 16.3%. Binding every table read removes the ROM's folded `gUnknown_08581164` pool word (member offset becomes `ldr [r3,#4]`). The permuter's mixed form (local for the chr reads, bare global for the two `.data` reads) is what restores the size: the bare reads keep the folded base+4 pool word.
- Chained permuter: run 1 (900 s) 35.8 -> 41.3, run 2 NO-IMPROVEMENT.
- Residual: str spill vs a2*32 spill and where &cursor lives (frame/reg allocation only; instruction multiset now close).
Proposed summary: does=renders a string glyph by glyph; status=size-exact, allocation differs; left=ROM keeps str on the stack, a2*32 in sb, table in sl and &cursor in r8 for the whole function; tried=binding table/cursor addresses (all reads: too small; mixed: size-exact).

## wave 97

Base: wave-95 draft (41.28%, 172 B), kept as `sub_08068038.w97-start.c`. No movement.
- Also binding the sentinel `[12].data` read to the table local: 180 B (+8), 19.4%. Also binding the matching `.data` read: 164 B (-8), 27.9%. Cursor pointer local bound after the first store: byte-identical. `off = a2 * 32` as a user local (before the loop or inside): 38.9% / 41.3%.
- Classified the residual from the disassembly: the candidate reaches the cursor through a `.rodata` force-addr word for the pre-loop store (`ldr r1,=word; ldr r0,[r1]; str r6,[r0]`), the ROM uses a plain pool word and then holds the address in r8 (`mov r3,r8; ldr r5,[r3]`). Same split as the wave-96 note; unsolved. The ROM also spills `str` to the stack and holds `a2*32` in r9, opposite to the candidate.
