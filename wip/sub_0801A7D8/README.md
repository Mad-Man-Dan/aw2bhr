# sub_0801A7D8

0x0801A7D8, 1056 bytes, THUMB, parked.

Best score so far: 84.4% (best.c).

## What it does

Saves a block of game data to flash memory in 4 KiB sectors. It splits the data into pieces, writes each piece with a header and checksum to a free slot chosen by its generation counter, and checks it, moving to another slot if the check fails. Returns 1 if it runs out of slots, otherwise 0.

## How close it is

Compiles to the right size (1,056 bytes); about 84% of bytes line up.

## What is left

Resolve the remaining instruction and register differences in the size-exact drafts. The earlier 12-byte excess belongs to the superseded draft.

## Already tried

- Writing the success flags as `| (idx << 4)` directly: 8 bytes worse, because the OR widens to a full word. A `u8 tag` set inside the byte-0xC store and reused later (the current draft) saves 4 bytes; `u16` or `int` for that local are 4 and 12 bytes worse.
- `if (retry == 0 || retry == 4)` instead of a switch with a fall-through: 8 bytes worse.
- Indexing `gUnknown_0200CC88[16 + x]` instead of taking the address of the second array: about 12 bytes worse.
- Copying a3 into its own local before the loop: 8 bytes worse, and a1 loses its register.
- `list[n++] = i` in one statement: increments before storing, where the original stores first.
- The plain member spelling `gUnknown_0200CC88.slotGeneration[x]` (kept in best.c): same size, a few more bytes line up; not adopted until the 12 bytes are solved.
- The merged size-exact drafts improve on the 1068-byte versions; their earlier experiment notes are retained in NOTES.md.

## Files

- `sub_0801A7D8.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 1068/1056 (+12), 20.0%. A narrow block-local u8 tag improved +16 to +12; the remaining 0x02002000 address-hub choice and spill cascade survive. Switch/retry CFG, folded-address forms, direct narrow OR, and wider tag spellings are settled in the draft comment.

### Wave 94

W94-A adopted Vesly's local draft (work/sub_0801A7D8/vesly-best.c, 83.24% size-exact) over ours (19.76% at +12), then two chained permuter runs: 83.71% size-exact. Kept forms audited by the orchestrator: a u8 `owner` local for unk00[i] (unk00 is u8) and `total` reused as scratch in the slot-owner loop (the outer for(;;) resets it before its only other use). Next: chain further.

### Wave 97

wave 97
Base: `sub_0801A7D8.c` (83.71%, size-exact, first difference +0x82), saved as `sub_0801A7D8.w97-start.c`. Final: **84.38%**, size-exact, first difference +0x82 (the residual is the sort-loop register swap and everything downstream of it).

Pre-registration ("struct MEMBER hoists its base register; an array subscript does not"): NOT the lever for the first difference. The save sector is already a struct view throughout, and the first difference is in the sort loop over `freeSlots`, which is a plain array.

Mechanism of the first difference, from `-da` (dump.greg): the sort loop holds two invariants, the `freeSlots` stack base (pseudo 95: 7 refs over 20 insns) and the generation table address (pseudo 100: 8 refs over 34 insns). The allocator priority is floor_log2(refs) * refs / live_length: 2*7/20 = 0.70 against 3*8/34 = 0.70, and the table is sorted first, so it takes the first hi register (ip) and the base gets r8. The ROM has them the other way round (base ip, table r8), so its base outranks its table. Only a reference-count change moves that: one more use of the base pseudo (8 refs gives 3*8/20 = 1.2) or one fewer of the table (7 refs gives 0.41).

Probed and negative, all byte-identical or worse (one-unit harness `build/probe/w97k.py`):
- the table bound to a local before the loop or inside the outer body (first difference moves to +0xc): binding makes the table a user variable hoisted and re-derived everywhere;
- an `fs = freeSlots` pointer (size +8); `&freeSlots[i]` held in a local (first difference +0x80, worse); `&freeSlots[j]` per iteration;
- comparison operands swapped (`>`): puts base in ip and table in r8 like the ROM, but turns `bcs` into `bls` and makes `&freeSlots[j]` the hoisted address; the ROM hoists `&freeSlots[i]`;
- generation values read into locals in either order; swap spelled from `[j]` first; a second temp for freeSlots[j]; `(int) freeCount` in the inner bound.

Permuter (3 chained 900 s x 2 runs, 83.71 -> 84.00 -> 84.19 -> 84.38): all value-preserving, none touch the first difference: a `do { } while (0)` around the swap store, `+ -1` for `- 1` in the segment tag, a `new_var` pointer binding of the sector view at the `checksum` store, `do{}while(0)` around one slotOwners store, and `lastByte = 0xfff` moved down to just before the `sub_0801B648` call (equivalent: it is only read in the failure arm). A `(segment ^ 0) >= 0` in the loop test was removed as not load-bearing.

Proposed summary: does = writes a save to flash sector by sector with a header, checksum and generation counter, retrying on spare slots; status = "1056 bytes, size exact, 84.4%; register allocation in the sort loop and after"; left = "the original holds the freeSlots stack base in ip and the generation table address in r8; ours holds them the other way round, and the rest follows"; tried = the list above and the priority arithmetic.

</details>
