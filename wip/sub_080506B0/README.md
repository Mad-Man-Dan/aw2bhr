# sub_080506B0

0x080506B0, 680 bytes, THUMB, parked.

Best score so far: 38.5%, +4 bytes (best.c).

## What it does

Sets up the current sprite object (gUnknown_03001FBC) for one side and slot: flip, palette 8, tile number and priority 3. For two unit types it also turns on alpha blending so the sprite is semi-transparent, then computes the sprite's position and places it.

## How close it is

Compiles 4 bytes too long (684 against 680). 417 of 680 bytes differ (38.7% identical), which means little because the size difference shifts everything after it. The instructions are in the original's order; the size difference is the one missing force-addr word for gUnknown_03004580 (the draft gets a plain pool word), and the register choices follow from it.

## What is left

Make the compiler load gUnknown_03004580's address through a force-addr word, as it already does here for gUnknown_02029710 and gUnknown_085D6A48. The untried idea: the original adds the column offset to the base before adding the row, so read gUnknown_03004580[side][1] through `(u16 *)((u8 *)gUnknown_03004580 + 2)` indexed by the side.

## Already tried

- Reading gUnknown_03004580[side][1] a second time in the 0x17 test: identical output; the compiler merges the two reads.
- `v` as int, u32 or s16, nested-if or switch for the 0x17/0x11 test, and a separate copy for the compare: pool words unchanged.
- `dx`/`dy` as u16 or int instead of s16: a different negate sequence; s16 is required.
- Plain 2-D subscripts on gUnknown_085D6A48 instead of the row-struct cast: an extra add per read.
- Writing the gUnknown_08553C18 term first in the two sums: moves its whole address chain ahead of the entry pointer.
- Binding the tile-number source to a u16 local: no change.

## Files

- `sub_080506B0.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 improved from 692 to 684 bytes against 680 (+12 to +4 section), 38.7%, first difference +0xa. Binding sidep/slotp and reassigning after sub_08015608 makes four documented pool choices coexist; only the forced gUnknown_03004580 word is still missing. Preserve this improved draft.

</details>
