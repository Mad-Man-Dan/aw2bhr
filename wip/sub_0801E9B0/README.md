# sub_0801E9B0

0x0801E9B0, 824 bytes, THUMB, parked.

Best score so far: 44.0% (best.c).

## What it does

Copies a sprite's list of OAM entries into the OAM buffer, combining each with the template a5, moving it to (a2, a3) and applying flips and the owning object's scale. Each entry is passed to its object's callback; returns 1 without writing if the entries would not fit.

## How close it is

Compiles 12 bytes too short (812 against 824). The stack layout and nearly every statement are right; the difference is that the compiler merges the ends of the two scaling branches (full offset and half offset) into shared code, where the original keeps them separate.

## What is left

Make the two scaling branches come out with different registers, as in the original (its half-offset branch needs an extra register for the rounding), so the compiler stops merging their final x and y updates. The permuter has not been tried on this function.

## Already tried

- Shared `dx`/`dy` locals for both scaling branches: the branches merge almost completely, 72 bytes short.
- Separate locals per branch (`dx`/`dy` and `ex`/`ey`): 20 bytes short; the branches split, but their last updates still merge.
- Binding the width table to a local pointer in only one branch (the current draft): 8 bytes better, still 12 short.
- Writing each branch's final updates as different inline table expressions: identical code to the current draft.
- Other declaration orders for the locals: the stack slots follow declaration order, and only the current order gives the original's layout.

## Files

- `sub_0801E9B0.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 812/824 (-12), 27.8%. Inlining distinct expressions does not recover the missing code. Call/arithmetic semantics are retained; residual is expression lifetime and allocation.

### Wave 95

Base: sub_0801E9B0.w95-start not needed; draft unchanged (27.8%, size -12). Confirmed by diff that the allocation differs from the prologue (ROM: a4 in r9, a1 in ip, a2 spilled at [sp,#0]; draft: a2 in ip, a1 spilled). Not hand-probed and permuter not run this wave (ran out of time). Next step: chained permuter from this draft; the branch-merge hypothesis (different final update registers in the two scaling arms) is untested.

### Wave 96

Base: the wave 95 draft (sub_0801E9B0.w96-start.c, 27.79%, size-12); final source is v13.c (copied to sub_0801E9B0.c): 10.7% size-12 by the score (the score falls because the shifted bytes moved, NOT progress or regression), but the register-blind instruction diff against the ROM fell from 52 differing lines to 20 and the whole flag/non-flag branch layout now agrees with the ROM. Judge it by `python tools/drafts.py score` plus a normalised diff, not by the percentage.

Changes that moved it, each with its mechanism:
1. `t0`, `t1`, `t2` declared `u16` (they are read from u16 halfwords). This is the ROM's copy-of-the-mask-constant pattern (`movs rA,#0x80; lsls; adds rB,rA,#0; ldr rC,[sp]; ands rC,rB`): a masked test of a NARROW value copies the constant, an `int` does not. Four of the five missing `adds rX,rY,#0` came from this, not from separate source variables. (Pre-registered "several source temps" hypothesis: REFUTED for this function; the ROM's extra copies are constant copies from narrow operands.) The fifth site (`h & 0x100`, `h` = the u16 member just read) also wants `h` u16 and does then copy the constant, but a u16 `h` also copies `h` itself (`adds r0,r3,#0` before the `ands`) which the ROM does not; the two ROM registers for h/x are (h r4, x r3), the draft has them the other way round.
2. `x = x + a2; y = y + a3;` (x first). With y first, the two zero-extended parameter copies land in the wrong slots (the ROM keeps a2 in ip and a3 in the stack slot). The order in the ROM's tail is y then x, so the emitted order and the source order are not the same thing here.
3. Branch layout: `if (flag == 0 && (t1 & 0x1000)) x = -(...);` as its own statement, then `if (flag != 0) { if (t0 & 0x200) {...} else {...} } else { y += a3; x += a2; }`. This puts the non-flag tail after the two scaling arms as in the ROM (the earlier draft had it inline, which needed a `b` around it). Splitting only the first `if` into two, without turning the second into `if (flag != 0)`, was byte-identical to the old layout (jump threading undoes it).
4. `x = *(u8 *)&gUnknown_03000548.unk02;` (byte read) instead of `(u8)gUnknown_03000548.unk02`; equal bytes to the cast form with `h` int, kept because the two-halfword read is then not merged with `h`.

Permuter (900 s from v13): 'improved' 10.68% -> 44.05% by rewriting `i < n` as `i <= n - 1` and `a1 + n > 0x80` as `a1 + n + 1 > 0x81`. Size became exact only because those two rewrites ADD instructions (a `subs` and two `asrs`) that make up for the real missing bytes. Discarded as a size-only gain; the permuter file is kept as sub_0801E9B0.w96-perm1-out.c.

What is still missing (12 bytes): the ROM's prologue moves a4 to r9 before loading a6 (`mov r9,r3; ldr r3,[sp,#84]`), so a6 keeps r3 while the draft puts it in r4 and n in r5; the 0xFF00 / 0xC000 mask constants are materialised at different points (one `movs #255` and one `movs #192; lsls #6` are early in the ROM); the `(h & 0xC000) >> 12` half of the two-dimensional table index is computed BEFORE the `unk00 & 0xC000` half in the ROM and after it here (subscript order; a shared `jj` temp for the second subscript is much worse, 368 vs 394 instructions).

Proposed summary: does = builds OAM entries for a list of sprite pieces, moving each by (a2, a3) and, when its flag bit is set, scaling it by the affine entry of a6; status = branch layout and copies match, register assignment of a6/n/h/x and mask-constant placement differ, 12 bytes short; left = the prologue register order and index-expression order; tried = u16 temps, tail layout, x/y order, permuter.

### Wave 97

wave 97 (W97-V)
Base: levers 5a-416+5b-100f (`q2 = a1 + n; if (q2 > 0x80)` using the already-declared q2, and `h0 = gUnknown_03000548.unk00` bound before the second table lookup); wrongc OK. 10.68% -12 -> 31.55% size-exact. Permuter run 1 -> 32.04% (`(h0 & 0xC000) >> 14` -> `h0 >> 14`, equal for u16), and it reformatted the file (comments/blank lines lost, formatting only). First difference still +0xc; the size-exactness is probably the temp adding the 12 missing bytes rather than reproducing the ROM's copies. Not matched.

</details>
