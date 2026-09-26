# sub_0804BB74

0x0804BB74, 324 bytes, THUMB, parked.

Best score so far: 29.5%, +12 bytes.

## What it does

Decompresses picture `a` and turns it into BG tilemap entries, then copies them to `dst`. With `d` set the picture is drawn mirrored (flip bit and palette 4), otherwise straight with palette 1. What the pictures are is unknown.

## How close it is

Compiles 12 bytes too long (336 against 324). 225 of 324 bytes differ (30.6% identical), but that score means little because the size difference starts near the top and shifts everything after it. The extra bytes come from gUnknown_0200FC50's address being held as two separate values (one for the decompress call, one for the loops) and from one extra pool word for gUnknown_085519FC in the 17-column loop.

## What is left

Find a spelling where one value holds gUnknown_0200FC50's address for the decompress call, both mirrored loops and the final copy, while each loop still reloads the gUnknown_085519FC pointer from memory on every step, as the original does. Every spelling tried so far gets one of these two but not both.

## Already tried

- A `src` local for gUnknown_0200FC50 assigned before the decompress call and passed to it: one shared address, but the per-step pointer reload is lost in the 32-column loop.
- The same local assigned after the call: the reload survives, but `dst` is spilled to the stack (4 more bytes of frame).
- Binding `u16 **p = &gUnknown_085519FC` in each arm: 4 bytes shorter, but the pointer is loaded once before the loop instead of every step and `dst` is spilled (13.9% identical).
- Naming gUnknown_08136044 (the read-only word holding gUnknown_085519FC's address) directly, in place or through a local: an extra indirection, or the pointer is loaded once before the loop.
- The 17-column loop as a pointer walk (`row += 32`): removes the row counter as the original does, but adds a loop-entry check the original does not have.
- The range test as a switch or a signed compare: wrong branch instruction; `(u32)(u8)(unk02 - 1) <= 1` is required.

## Files

- `sub_0804BB74.c`: the current draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 336/324 (+12), 30.6%, first difference +0x10. The established semantic draft was reverified; prior pointer-global spelling axes remain exhausted. Residual is address allocation and excess code.

</details>
