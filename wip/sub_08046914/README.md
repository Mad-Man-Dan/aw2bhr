# sub_08046914

0x08046914, 368 bytes, THUMB, parked.

Best score so far: 17.9%, -8 bytes (best.c).

## What it does

Draws the text of an info panel at x position a for entry b of gUnknown_085D583C (20-byte records with a name id and a defence value, likely terrain types): the name centred on row 1, text 0x960 on row 3 (plus 0x969 when gUnknown_02028DD4 is 0), on row 5 either text 0x969 or the number sub_08026C6C(b) returns, and text 0x961 on row 7 if any of the three unit types listed in gUnknown_084C20C0 has a non-zero repairTable entry for b.

## How close it is

Compiles 12 bytes short (356 of 368 bytes); 14.9% of bytes line up, which means little because the size difference shifts everything after it. The call sequence, constants and every argument are settled.

## What is left

The draft keeps one value too many in registers, so it never spills: the original saves `a + 0x38` to the stack and re-reads the text-buffer pointer through its address before every call, where the draft keeps both the address and the loaded pointer. The draft's own reading is that the original had more locals here; splitting `t` or the 0x8000 and 0 constants into their own locals is the untried next step.

## Already tried

- Writing the centring as one inline expression: the compiler reorders it and loads 0x50 before the call; the separate `x = ...` statement is required.
- `(u32)` casts on the two `/ 8` divides: 8 bytes worse.
- Loading the text-buffer pointer in its own statement or inside the first call's argument: moves the load but rotates the registers of a, b and the name pointer and shrinks the frame to 8 bytes.
- Volatile or cast spellings of that load: byte-identical.

## Files

- `sub_08046914.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 356/368 (-12), 14.9%. The early gUnknown_08499578 value/address lifetime split remains. Binding, comma placement, volatile and cast probes either rotate a/b/gfx and shrink the frame or compile identically; call sequence and body shape are settled.

</details>
