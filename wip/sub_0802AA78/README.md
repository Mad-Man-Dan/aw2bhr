# sub_0802AA78

0x0802AA78, 2356 bytes, THUMB, parked.

Best score so far: 89.3%.

## What it does

Draws the map information panel for the cell under the cursor: the terrain box, and the unit standing there with its HP, fuel, ammo and cargo. It then shows one of three small icons with a number, and sets display window 0 around the panel.

## How close it is

Compiles to the right size (2356 bytes); 253 bytes differ (89.3% identical). The first differences are compiler-made address words; the first differing instruction is about 920 bytes in.

## What is left

Two spots remain. In one if/else pair the original loads two tables in the opposite order in both arms, but writing both arms that way lets the compiler merge them and lose 4 bytes, so only one arm is written that way; and at the end the original sign-extends the icon value q and reuses it, which our build optimises away. Both need a new idea.

## Already tried

- Reordering the table sum in both arms of that if/else: 4 bytes short (2352). Only one arm is reordered in the draft.
- Holding the shared sum in a local in one or both arms: much worse (as low as 43% identical).
- Declaring q as int, as u16, or as s16 with casts, in different scopes: no change, or 4 bytes too long for u16.
- Two chained permuter runs, over 11,000 attempts: no match.
- Older higher scores (92.5%, 93.9%) came from a best.c with the headers pasted in, not from this draft; they cannot be reproduced and must not be used as a starting point.

## Files

- `sub_0802AA78.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

Wave 74 strongest semantic draft: exact 2356/2356, 89.3%, 253 differing bytes, first difference +0x90.

### What still differs

A coupled GCSE/pool-order and q-allocation residual remains around the sub_0802BB74/sub_0802BAFC branch and later merge. The historical 92.53%/176 and apparent 93.9%/144 records came from expanded-header contamination, not a reproducible ordinary draft.

### Why it is close

Control flow, canonical Map view, call sequence and total size are settled; Wave 74 improved the honest 87.9%/286 baseline to 89.3%/253.

### Already ruled out

- Both-arm reassociation shrinks to 2352; then-arm-only association is retained.
- Two clean chained --current permuter runs, over 11000 iterations total, found no match.
- Do not seed from the contaminated ~198KB best.c or reshape Map/Tbl49A2A6.

### Settled

- Retain the then-arm `(cx + tbl) + gUnknown_0849A284[6]`, unit->unk07 binding, and zero-lifetime accv compare temporary.

### Why it is parked

Wave 74 W74-B. Resume with a new GCSE/merge-allocation mechanism from the include-based active draft.

</details>
