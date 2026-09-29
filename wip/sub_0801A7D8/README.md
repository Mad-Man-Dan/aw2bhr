# sub_0801A7D8

0x0801A7D8, 1056 bytes, THUMB, parked.

Best score so far: 84.4%.

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
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 1068/1056 (+12), 20.0%. A narrow block-local u8 tag improved +16 to +12; the remaining 0x02002000 address-hub choice and spill cascade survive. Switch/retry CFG, folded-address forms, direct narrow OR, and wider tag spellings are settled in the draft comment.

### Wave 94

W94-A adopted Vesly's local draft (work/sub_0801A7D8/vesly-best.c, 83.24% size-exact) over ours (19.76% at +12), then two chained permuter runs: 83.71% size-exact. Kept forms audited by the orchestrator: a u8 `owner` local for unk00[i] (unk00 is u8) and `total` reused as scratch in the slot-owner loop (the outer for(;;) resets it before its only other use). Next: chain further.

</details>
