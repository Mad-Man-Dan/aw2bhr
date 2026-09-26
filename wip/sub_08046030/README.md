# sub_08046030

0x08046030, 1556 bytes, THUMB, parked.

Best score so far: 42.6%, +4 bytes (best.c).

## What it does

Sets up a per-army statistics screen (the draft calls it the results screen). It draws one row per army with its unit count and units lost, then captures, income and funds or placeholder text depending on defeat, AI control and fog, and stores each army's share of the counted terrain as a percentage.

## How close it is

Compiles 8 bytes short (1548 of 1556 bytes); 41.6% of bytes line up, which means little past the first difference because the size difference shifts everything after it. The first quarter of the function is byte-identical; the real difference is four instructions in one place plus two swapped stack slots.

## What is left

After working out the text column `a`, the original re-creates the three values shared by the next five text calls (the text-buffer pointer, 0x8000 and 0) where the draft keeps the earlier copies alive, and it stores two loop temporaries in the opposite stack slots. Find a spelling that gives each group of calls its own copies of those values without pinning registers.

## Already tried

- Setting `a` with an if statement instead of `?:`: loads in the wrong order; the `?:` form fixed 4 bytes and is kept.
- Reading the compared global into a local first: right load order but an extra copy the original lacks, and still one copy of each shared value.
- Fixing block-local variables to the original's registers: right registers, but their setup moves outside the first call.
- Assembler-symbol aliases for the shared values: forces the missing reload but splits the literal-pool entries wrongly.
- Register pins on the first group's shared values (the saved best): 4 bytes too long, 42.7%, not usable as source.

## Files

- `sub_08046030.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 1548/1556 (-8), 41.6%, pool 57/57, first real difference +0x188. Fixed block-local allocnos reproduce desired r6/r5/r4 but schedule initialization outside the first call; assembler-symbol aliases force the missing reload while incorrectly splitting literal-pool identity. Baseline restored.

</details>
