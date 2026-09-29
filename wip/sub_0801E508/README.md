# sub_0801E508

0x0801E508, 976 bytes, THUMB, parked.

Best score so far: 56.9%.

## What it does

Copies a sprite's list of OAM entries into the OAM buffer, rotating and scaling each entry's position about (a2, a3) using the scale and angle in affine record gUnknown_0200F720[a5]. Returns 1 without writing if the entries would not fit, otherwise 0.

## How close it is

Compiles to the right size (976 bytes) with 56.9% of bytes identical. Binding `angle = e[2]` for three of the four rotation-call arguments in the full-size arm fixed the size. What is left: the original reloads e[2] at each use and copies values from high registers before each call.

## What is left

Find a way of writing it that keeps as many values live as the original does (the object pointer `e` in a high register and a 0x24-byte frame instead of 0x20), so that the extra register copies appear.

## Already tried

- Writing each coordinate as one inline expression: the compiler moved both trig calls ahead of the size lookup and shared sin and cos between the x and y parts; 44 bytes short. The current explicit temporaries fixed the call order and gained 16 bytes.
- Putting every float term in its own statement, with the size lookups computed first, `a4` kept alive and a separate loop counter: 76 bytes short, worse, because values then live for even less time.
- Reading the affine record's halfwords unsigned instead of through `s16 *e`: adds an unsigned-correction float add that the original does not have.

## Files

- `sub_0801E508.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 948/976 (-28), 22.6%. Explicit arithmetic temporaries now preserve every observable soft-float, trig and libgcc call in ROM order. Remaining deficit is frame/register allocation, not arithmetic semantics or call association.

### Wave 96

Base: sub_0801E508.c (22.64%, size-28; the wave 71 draft). Result: 56.9%, size-EXACT (976), first difference +0x12. NOT matched; the instruction-level diff is still large (about 80 differing lines vs 104 before).

What moved it (permuter, 900 s, then cleaned by hand): a "mixed bind". `s16 angle = e[2];` assigned once per record before the `(w & 0x300) == 0x300` test, and used for exactly THREE of the four `(float)e[2]` arguments in the full-size arm (fx with cosine, fy with sine, fx of the y part with sine); the y part's cosine and all four reads in the half-size arm keep `e[2]`. Binding all of them, or all in the full-size arm, or all in the half-size arm, is far worse (14-30%, size -8 to -40): the size gain is the subset. This is the wave 95 "mixed bind" lever (item 2 in the brief) applied to a struct member read that is re-read across calls; it works on a subset here, and the failures match the brief's warning for re-read pointers.
The size is exact but the ROM still re-loads e[2] from memory with `movs r1,#4; ldrsh r0,[r3,r1]` at every use (r3 = r9 copy of e), where this draft sign-extends `angle` in registers (`lsls; asrs`). So the bind is an approximation: it repairs the size, not the mechanism. The ROM has no local for e[2]; something else makes the first arm's reads differ from the second's.

Not resolved: e is in r6 here, r9 in the ROM (the ROM keeps w in r8, v in sl, u in r7, e in r9, n in the stack); the high-to-low `mov`/`ldrsh` copy pairs before each sinf/cosf call are the +11 copy delta. The pre-registered "several source temps" hypothesis is therefore PARTLY right (the bind is a source temp) but the ROM's extra copies are the `mov rLo,rHi` before each call, which a temp for the ANGLE does not create; the hypothesis' mechanism did not hold.
A second permuter chain (900 s) from the 56.9% file found nothing.

Proposed summary: does = rotates and scales a list of OAM pieces by an affine entry, using soft-float; status = size exact, 56.9% identical; left = e/w/v/u register assignment and the per-call reload of e[2]; tried = temps for the accumulators (wave 65), distinct loop counter, angle temp at several subsets, permuter.

### Wave 97

wave 97 (W97-R)
Tried "respell one of identical expressions" per call site (full-size arm): each of the four `(float)e[2]`/`angle` sites as `e[2]`, `angle` or `*(e + 2)`, all 81 combinations. Best remains the wave 96 file (56.86%); `*(e + 2)` is the same tree as `e[2]` (byte-identical to it), the other combinations are 26-54%. No new lever. The ROM's `movs r1,#4; ldrsh r0,[r3,r1]` is just how Thumb loads a signed halfword; the real difference is that e lives in a high register (r9) in the ROM, so each call site copies it down. Not pursued further.

</details>
