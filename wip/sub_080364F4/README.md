# sub_080364F4

0x080364F4, 296 bytes, THUMB, parked.

Best score so far: 9.5%, -16 bytes.

## What it does

Sets up the players for the current map from its chapter record gUnknown_085C77A0[mapID]: each army's colour, CO and one more per-army byte are copied into gPlaySt. It then starts the map's pre-placed-unit script, or calls sub_080364E0 if there is none.

## How close it is

Compiles 16 bytes short (280 of 296 bytes); only 9.5% of bytes line up, which means little because the size difference shifts everything after it. All three branches, both four-army loops, the 0xff fallback and the final script choice are present.

## What is left

The original loads the record table's base address through a pointer word, keeps it in a register, and adds the field offsets (0x3c, 0x40, 0x44) at run time; the draft's plain array indexing lets the compiler fold those offsets into its address constants and order the bases differently. Find a spelling that keeps the base separate without the extra 4-byte stack slot the probes so far caused.

## Already tried

- Naming the original's two private pointer words (at 0x08090EBC and 0x08090EC0) and adding table/base locals: reproduces the original's extra load step but forces a 4-byte stack frame the original does not have.
- Switching directly on the u8 gameMode member: gives an unsigned comparison where the original's is signed, so the switch uses an int copy.
- Reading the offset-0x44 bytes as `unk44[i].unk00`, the shared struct's declared shape: scales the index by 4, which is wrong; they are read through a byte cast instead.

## Files

- `sub_080364F4.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 280/296 (-16), 9.5%. Private-slot/table-base probes restore the initial indirection but force a non-ROM four-byte frame. This confirms base association/allocation rather than missing statements; shared structs and the readable semantic draft are retained.

</details>
