# sub_08057164

0x08057164, 268 bytes, THUMB, parked.

Best score so far: 22.4%, -4 bytes (best.c).

## What it does

Fills the five entries of gUnknown_02029A10[y]. Each entry's unk1a comes from gUnknown_0855203C, gUnknown_08551F60 or gUnknown_08551E84, chosen by two flags in the gUnknown_085D6A48 row for gUnknown_03004582[y][0], at index a * 10 + y * 5 + i (b instead of a for gUnknown_0855203C when y is not gUnknown_0300450C). unk00 and unk01 are bytes from gUnknown_085521DC at y * 55 + a * 5 + i and y * 55 + b * 5 + i, and unk02, unk04 and unk06 are cleared.

## How close it is

Compiles to the right size (268 bytes), but 212 of 268 bytes differ (about 21% identical): the loop's index and pointers are set up differently, which shifts registers through the whole body.

## What is left

Make the compiler step one element index by 1 and scale it at each table read, as the original does, instead of also turning the gUnknown_0855203C read into a separate stepping pointer. The original then keeps gUnknown_0855203C's address in a register and reloads gUnknown_085521DC's inside the loop.

## Already tried

- Binding `tbl = gUnknown_0855203C` before the loop: the compiler still builds a stepping pointer from it.
- A real `idx` counter incremented at the bottom of the loop, with an explicit record-pointer walk: gives the original's step-by-1 index, but the stepping pointer for gUnknown_0855203C is still created.
- Removing `tbl` in that form: no change.

## Files

- `sub_08057164.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 70 at exact size 268/268. All statements and branches are present; the residual is strength_reduce retaining an address GIV for gUnknown_0855203C where ROM keeps an element-index IV. A bound table pointer and Wave 70's explicit idx counter plus record-pointer walk still form the address GIV. The new source counter does recover the desired +1 element IV, proving the remaining problem is the second GIV decision rather than loop semantics.

</details>
