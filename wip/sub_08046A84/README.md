# sub_08046A84

0x08046A84, 672 bytes, THUMB, parked.

Best score so far: 86.3%.

## What it does

Draws the graphics of the terrain info panel for terrain b: its picture, an icon for each unit type that can be repaired there, and, depending on the panel mode, one sprite per point of defence and an icon for each movement type that can enter it.

## How close it is

Compiles to the right size (672 bytes) with 86.3% of bytes identical. What is left: the first conditional's layout, and the original loads `defense` through [r1,#0] where the draft adds the offset.

## What is left

The original's switch on gUnknown_02028DD4 dispatches with one extra range test, a layout between what the compiler gives with and without an explicit `case 0`; and it adds 0x50 to the table value before adding `a`, where the compiler folds the constant the other way. Two literal-pool words differ only in how the disassembly names them and are not real differences.

## Already tried

- An explicit empty `case 0`: reproduces the original's dispatch tree but is 4 bytes too long (676, 49.6%).
- Binding `table[i * 2] + 0x50` to a local in both loops: right add order, but 664 bytes and worse registers.
- The three-way table choice as a `switch` with a pointer local: right comparisons but 648 bytes.
- Re-bracketing the sum: the compiler's constant folding ignores the brackets.

## Files

- `sub_08046A84.c`: the current draft
- `NOTES.md`: working notes

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 668/672 (-4), 40.2%. Binding table[i*2]+0x50 obtains the target local add order but shrinks to 664 and worsens allocation. Three-way switch forms and explicit case-0 dispatch were already measured; readable baseline is retained.

### Wave 95

Base: `best.c` (explicit `case 0: break;`, 676 bytes, +4, 49.3%). Result: **61.8%, size-exact (672)**,
draft `sub_08046A84.c` (also `v7.c`; the old draft kept as `sub_08046A84.w95-start.c`).

What moved it:
- **The `+ 0x50` bind, per loop.** The park tried it in both loops at once (664, worse). Only in the case-1
  loop, on the explicit-`case 0` base: `x = gUnknown_084C2112[i * 2] + 0x50;` then `x + a` as its own
  statement -> 672 (size-exact), 61.3%. Case-2-only: +4 (50%). Base without `case 0`: -8 / -4.
  So the two named residuals were not independent: the bind (-4) exactly cancels the explicit `case 0` (+4).
- Casting the case-2 loop bound to non-const, `((struct Unk085D583C *)&gUnknown_085D583C[b])->defense`, makes
  the bound be re-read through the row address (the ROM reloads `defense` each iteration through the held row
  pointer; the const table makes agbcc hoist it). Case-2 loop only: 61.8%, still 672. In the case-1 loop it
  costs +4 (the bind and the non-const read conflict).

Not reproduced: the first ternary's layout. The ROM has `cmp #6; beq A; cmp #8; bne C; <8 arm>; b join;
<6 arm>` (the 6 arm out of line), the draft `cmp #6; bne; <6 arm>...`. Writing it as
`b != 6 ? (b == 8 ? X8 : C) : X6` merges the arms' loads and shrinks 16-20 bytes (12.95%, 15.0%): wrong.
First difference stays +0x1e. Remaining diffs otherwise: the register of the row address (`ldr r1` vs `ldr r0`)
and add order in the second loop.
The W95-B note (forced `.rodata` word then plain literal after a join) was checked: the two literal-pool
differences here (`gUnknown_085D5ABC+0x54` vs `gUnknown_085D5B10`) are the same-address class, not that.
The one-temp-per-block lever transferred: YES in effect (per-loop temp `x`, not one shared across both loops).

### wave 95, permuter (chained 600 s runs, 2 threads, from the 61.8% size-exact draft)

61.8% -> 63.0% -> 66.8% (size-exact 672, kept in `sub_08046A84.c` = `.w95-perm2-out.c`). Kept mutations, read:
a `const struct Unk085D583C *new_var` row pointer, `new_var2 = ...movementChart[gPlaySt.weather];` (row of the
movement chart bound, then indexed), `(gUnknown_085D583C + b)->defense`. All the same C.
**Run 3 reported 83.3% and is WRONG C -- rejected.** It added `volatile int new_var3; new_var3 = 0x50;` and used
`new_var3` for the `+ 0x50`. The volatile local makes a stack slot the ROM lacks: `sub sp,#20` against the ROM's
`#16`, first difference moves EARLIER (+0x1e -> +0xa). The banner was a frame artefact, exactly the wave-94
pattern. Kept file `.w95-perm3-out.c` only as evidence; the permuter was killed during run 4. The `+ 0x50`
order residual is therefore still open; the permuter's own answer for it (a stored constant) is not the ROM's.
Residual as before: the first ternary's layout (+0x1e), the row-address registers.

### Wave 97

wave 97
Base: sub_08046A84.c (66.82%, size-exact 672, first diff +0x1e). `best.c` (83.33%) is WRONG C: it adds `volatile int new_var3` for the `+ 0x50` (frame `sub sp,#20` vs ROM `#16`), rejected again; the "valid twin" of that form is the `x = ... + 0x50` bind already in the draft (no other twin found).
Tested the first conditional's layout (ROM: `cmp #6; beq X6(out of line); cmp #8; bne C; X8 inline; b join; X6; C`), each compiled in place:
- `b != 6 ? (b==8 ? X8 : C) : X6`, `b==6 ? X6 : (b!=8 ? C : X8)`, `b != 8 ? (b==6 ? X6 : C) : X8`: all -16 bytes (~14%): merges the per-arm address adds.
- `b==8 ? X8 : (b==6 ? X6 : C)`: 672, 66.7%, same layout with the tests swapped (8 first) -- not the ROM either.
- pointer-of-record ternary `(...? &A[i] : ...)->unk08`, and if/else statements assigning a `const struct Unk085D583C *rec` (three arm orders): all -24 bytes (~12-15%): once the arms yield a record pointer, the `+8` and the index scaling are hoisted to the join.
- struct-valued ternary `(b==6 ? A[i] : ...).unk08`: identical to the draft (66.82%).
Mechanism: the ROM keeps `adds r2,#8; adds r0,r0,r2` INSIDE each arm and loads at the join, so the arms are `.unk08` addresses of different tables, not records; no spelling that merges the record keeps that. The 6-first, 8-inline order is not reachable by reordering the ternary (tests follow source order and the first-tested arm is emitted inline). Open.
Proposed summary: does = draws the unit-detail window; status = size-exact, 66.8%; left = first ternary layout (6 arm out of line) and row-address registers in the two loops; tried = ternary/if arm orders and polarities, record-pointer merge (shrinks 24 B), volatile constant (wrong).

wave 97 (W97-V)
Base: levers 5b-68_1d-273i (bind `a + 0x37` to the already-declared `x`; respell `(k + 0x50) + a` as `a + (k + 0x50)` at case 2's loop); wrongc OK. 66.82% -> 84.67% size-exact. Permuter chain (3 runs, 540-600 s): run 1 -> 85.86% (case 1 loop bound bound through a temp `row = gUnknown_085D583C + b`, mirroring case 2's `terrain`), run 2 -> 86.31% (case 1 `x = table[i*2] + 0x50` split out, mirroring case 1's other spelling), run 3 no improvement. All wrongc OK. Final 86.31%, size exact, first difference +0x1e. Residual: the ROM's pool word is the separate symbol gUnknown_085D5B10 (= &gUnknown_085D5ABC + 0x54, the repairTable member) where the draft emits a base+offset load, plus `defense` loaded through `[r1,#0]` after `adds r0,#16` in the ROM vs `[r1,#16]` here. Negative: `const int *stars = &row->defense; i < *stars` -> +4/+8 bytes, 41%/39%.

wave 97 (W97-Z)
No change (86.31%, size-exact, first diff +0x1E). Checked by the pool-word mechanism: there is no `.rodata` word here; `gUnknown_085D5B10` is an lds name for `&gUnknown_085D5ABC[0] + 0x54` (include/unit.h ~L270), so the pool difference is the same-address class and the residual is code (the first ternary's layout and the `defense` load through `[r1,#0]`). The split construct does not apply.

</details>
