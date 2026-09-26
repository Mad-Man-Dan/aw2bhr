# sub_08046A84

0x08046A84, 672 bytes, THUMB, parked.

Best score so far: 49.3%, +4 bytes (best.c).

## What it does

Draws the graphics of the terrain info panel for terrain b: its picture, an icon for each unit type that can be repaired there, and, depending on the panel mode, one sprite per point of defence and an icon for each movement type that can enter it.

## How close it is

Compiles 4 bytes short (668 of 672 bytes); 40.2% of bytes line up. Everything structural is in place.

## What is left

The original's switch on gUnknown_02028DD4 dispatches with one extra range test, a layout between what the compiler gives with and without an explicit `case 0`; and it adds 0x50 to the table value before adding `a`, where the compiler folds the constant the other way. Two literal-pool words differ only in how the disassembly names them and are not real differences.

## Already tried

- An explicit empty `case 0`: reproduces the original's dispatch tree but is 4 bytes too long (676, 49.6%).
- Binding `table[i * 2] + 0x50` to a local in both loops: right add order, but 664 bytes and worse registers.
- The three-way table choice as a `switch` with a pointer local: right comparisons but 648 bytes.
- Re-bracketing the sum: the compiler's constant folding ignores the brackets.

## Files

- `sub_08046A84.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 668/672 (-4), 40.2%. Binding table[i*2]+0x50 obtains the target local add order but shrinks to 664 and worsens allocation. Three-way switch forms and explicit case-0 dispatch were already measured; readable baseline is retained.

</details>
