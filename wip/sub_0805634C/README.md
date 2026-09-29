# sub_0805634C

0x0805634C, 364 bytes, THUMB, parked.

Best score so far: 38.2%, -4 bytes (best.c).

## What it does

Builds side `b`'s sort keys from ROM tables, adjusting the keys of up to five particular slots, then sorts them with sub_08056638(b). Does nothing if `c` is 0.

## How close it is

Compiles 4 bytes too short (360 against 364). 221 of 364 bytes differ (38.2% identical), which means little because the difference starts at the top; the first loop and everything before it are right apart from one register pair, and the gap is in the second loop.

## What is left

In the second loop the original loads the bare addresses of gUnknown_020298E0 and gUnknown_08551D22 and adds the member offsets (unk1a, and column 3) at run time before adding the varying index; every spelling so far folds the offset into the pool word or into the load. Separate `p = base; p += K;` statements produce the right instructions, but they must live exactly as long as the original's values: before the loop they get spilled, and inside it the compiler folds them back.

## Already tried

- Member, flat and pointer spellings (`gUnknown_020298E0[b].unk1a[j]`, `rec->unk1a[b * 72 + j]`, `gUnknown_08551D22[x][3]`, `c3 = gUnknown_08551D22[0] + 3`): the offset is folded into the pool word.
- A local holding the table's address, before or inside either loop: the whole address is moved out of the loop and a shared index computation is lost, or the offset goes into the load.
- Wrapper structs around the tables (a leading filler, or an array of row structs): the offset still folds, or goes into the load.
- Separate `p = base; p += K;` statements before the loops: the adjusted pointers are spilled; inside the loop body: folded back to base + K.
- Writing the `+` operands of the first loop the other way round: an extra force-addr word for gUnknown_0300450C appears; the draft's order is required.

## Files

- `sub_0805634C.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 70 at 360/364 (-4). First loop and all semantics are settled; the second loop needs two addresses emitted as bare symbol plus runtime constant before the varying offset. Separate p=base; p+=K statements prove that spelling for +0x1a and +6, but before-loop placement spills the adjusted pointers while body placement const-propagates back to base+K. Existing direct/member/pointer forms are documented in the draft; the remaining axis is lifetime/CSE, not transcription.

### Wave 95

No probe run. Reading the pre-registered chapters (two-statement split, member-array giv order) against the record: the wave-70 park already ran the two-statement `p = base; p += K;` split, before the loop (spills, kills the inner CSE) and in the body (const-propagates back). The chapters' successful splits use a base that is a LOAD (`e4 = *pE4`) or a two-element row read; here the base is a bare SYMBOL_REF, so cse folds base+K back. Nothing in the chapters gives a non-constant base for gUnknown_020298E0 / gUnknown_08551D22. Left at the wave-70 draft (360/364).
(draft left at the wave-70 draft.)

</details>
