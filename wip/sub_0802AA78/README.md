# sub_0802AA78

0x0802AA78, 2356 bytes, THUMB, parked.

Best score so far: 98.2%.

## What it does

Draws the map information panel for the cell under the cursor: the terrain box, and the unit standing there with its HP, fuel, ammo and cargo. It then shows one of three small icons with a number, and sets display window 0 around the panel.

## How close it is

Compiles to the right size (2356 bytes); 46 bytes differ (98.0% identical). Two things remain: five compiler-made address words that the build still has to place where the original keeps them, and one pair of stack copies the compiler emits in the opposite order.

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

### Wave 93

WAVE 93 (W93-D): 89.26% -> 98.05%, still size-exact at 2356 bytes, 46 differing bytes, first difference +0x90. The park said two clean chained permuter runs over 11,000 iterations found nothing; that was true of the run, not of the method. FIVE runs, each started from the previous one's kept improvement, gave 89.26 -> 93.80 -> 95.84 -> 97.28 -> 98.05, and a sixth found nothing. Every gain is a BINDING LOCAL or a REASSOCIATION; no statement was added, removed or reordered. The kept changes, each read and checked: the first sub_0802BAFC argument in the hp branch bound to `q` (a dead-range reuse -- q is assigned 0 later, before its own first read); gUnknown_0849A284[1] indexed through a local holding 1 (renamed yIndex); the table's unk04 column base bound to a local (renamed tblUnk04); `accv != zero` written as `accv != 0`, the same test because zero = 0 is assigned earlier; and three address sums reassociated. Both permuter temporaries were renamed out of new_var form and re-measured byte-identical. drafts.py bases reports no read-before-set and names the draft as the base. WHAT IS LEFT, two things. (a) FIVE .rodata POOL WORDS: the candidate emits its own address-constant pool relocating against .rodata with addends 0, 4, 8, 0xc, 0x10, where the ROM names gUnknown_08090B98, _B9C, _BA0, _BA4 and _BA8 -- five consecutive 4-byte incbins in data/rodata-0808F098.s, i.e. the ORIGINAL unit's own -fforce-addr pool that the splitter gave invented names. This is the ordinary rodata-carve case; the promotion needs a "rodata" entry naming those five words. (b) ONE SWAPPED PAIR OF SPILL COPIES: the ROM copies [sp,#36]->[sp,#56] then [sp,#32]->[sp,#52]; the candidate does the two the other way round, while the following pairs [sp,#40]->[sp,#60], [sp,#44]->[sp,#64] and the [sp,#72] store already agree. There is no struct copy in the source there -- these are reload's own spill copies around the terrain switch, so the order is reload's, not the source's. NOTE for the next wave: the 92.5%/93.9% figures this entry warned about as header-expanded contamination are now beaten by an honest include-based draft.

</details>
