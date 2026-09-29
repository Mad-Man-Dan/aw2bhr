# sub_0806AB9C

0x0806AB9C, 360 bytes, THUMB, parked.

Best score so far: 43.1%, -12 bytes (best.c).

## What it does

Per-frame draw of a scrolling group of sprites for a proc. The group's y is half the proc's halfword counter unk44: it draws unk48 sprites from the codes in unk2f[], one sprite at x 0x3a, up to three icons 16 pixels apart from x 0x6a for the unk2c[] entries that are not -1, and one sprite from the 16-byte table gUnknown_085816F0[unk4c], each only when its y is on screen. Then it decrements the counter and calls Proc_Break once y is below -0x7c.

## How close it is

Compiles 8 bytes short of 360, 41.9% of bytes in place. The difference is in the third loop (the three icons): the ROM holds x shifted left 16 bits, computed once per pass and shared by the 0x1ff mask and the increment, which frees a register for the 0x1ff constant and gives it an 8-byte stack frame instead of the draft's 4.

## What is left

Find a spelling where the value masked with 0x1ff is a 16-bit truncation the compiler cannot prove unnecessary, so that it computes the shifted x once and shares it between the mask and the increment.

## Already tried

- u16 x with `x & 0x1ff`, int x with u16 casts, and an extra `x = (u16)x` in the loop: the compiler drops the truncation; no change (352 bytes).
- s16 x: the increment then uses a signed shift, which is wrong.
- No mask at all: 344 bytes (16 short), so the mask is real.
- The truncation written as an explicit shift pair: 348 bytes; computing x from i with no running value: 348 bytes.
- `x += 0x10` moved into the for-increment: byte-identical.
- A wide running value plus a separate u16 copy: gets the 8-byte frame but not the shared shifted value; the other way round keeps the 4-byte frame.

## Files

- `sub_0806AB9C.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 352/360 (-8), 41.9%. Separate narrow snapshots and live wide accumulators recover either the eight-byte frame pressure or the shifted truncation sequence, never both. Residual is the third-loop PRE and mask/preheader allocation.

### Wave 95

Base: existing draft (352/360, -8, 41.9%). Pre-registered hypothesis (wave-94 chapter: the running x must be something the compiler cannot bound) NOT settled in its favour.
- Fold-proof mask on the mask operand, `(((u32)x << 16) & 0xffff0000) >> 16) & 0x1ff` with `u16 x`: 348 (-12), 41.4%. Folds like the earlier spellings (combine discharges it, then the shift pair is gone, nothing is shared with the increment).
- `u32 x` with the increment written as the shift pair `x = ((x << 16) + 0x100000) >> 16` and the mask operand `((x << 16) >> 16) & 0x1ff`: 348 (-12), 43.1% (higher score only because the size moved; first difference still +0xa).
- Mechanism: x's two defs (0x6a and the truncated increment) let nonzero_bits prove x <= 0xffff, so any truncation of x in the mask is deleted by combine; the ROM's shared `lsls r5,r1,#16` therefore needs a definition of x that is NOT single-range, and none is available from the constants in this loop.
Proposed summary left: loop 3 keeps x<<16 once per pass in the ROM; every spelling that keeps the truncation lets combine delete it. Not permuter-run (size -8).

</details>
