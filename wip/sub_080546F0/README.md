# sub_080546F0

0x080546F0, 1060 bytes, THUMB, parked.

Best score so far: 26.6%, +8 bytes.

## What it does

Resets the state of a two-side, five-slot subsystem: it clears its variables and per-side records, fills a tilemap buffer, clears 32 KB of VRAM and a 32 KB work buffer, and copies two tilemaps to VRAM.

## How close it is

Compiles 8 bytes too long (1068 against 1060). 776 of 1060 bytes differ (26.8% identical), which means little because the register assignment differs from the start. Every statement, member offset, loop bound and store order matches the original; the difference is register allocation, and the draft's frame has three more spill slots.

## What is left

Find how the original shares the loop counters between the four loop nests: it keeps the shared counter in a different register, needs three fewer stack slots, and so reuses one constant and one product where the draft materialises them again. Splitting the counters by hand overshoots, so the answer is some partial split or reuse not yet found.

## Already tried

- Separate counters for all four outer loops: 16 bytes short; also for the two inner loops: 20 bytes short.
- A separate counter for one outer loop at a time: 8 or 16 bytes too long, or 16 or 36 bytes short depending on the loop; for one inner loop: 12 bytes too long.
- Writing the `|= 0xffff` rows on the declared s16 arrays: the compiler folds them to a plain store of -1; the u16 view structs are required.
- Putting the u16 cast on the whole array, or on the member's own address: 28 bytes longer, or an extra pool word.
- gUnknown_02028E5C's two stores as `[i][0]` and `[i][1]`: splits the base; the row pointer `q` is required.

## Files

- `sub_080546F0.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 1068/1060 (+8 section, +6 code), 26.8%, first difference +0xa. Settled shapes, types and stores are retained; residual is the shared-counter allocation seed.

### Wave 95

Base: the existing draft (`sub_080546F0.w95-start.c`, unchanged, 26.6%, +8). Not improved.
Only hand probes, no permuter (not size-exact; permuter reserved for the size-exact drafts).

- Counter TYPES are not the seed: `u8 i` -864 (-196), `u8 i,j` -228, `int i; u8 j` +12, `s16 i` +32, `u32 i,j` +8 (same as int), `int i; s16 j` +20.
- The frame difference is confirmed as pure allocation: draft and ROM are the same code up to the first
  loop, but the draft puts `i` in r6 and the 0x030045A0 pointer in r7, the ROM the reverse (i in r7, pointer r6).
- `gUnknown_02029664 = 0` as its own byte zero (ROM: `movs r0,#0; strb r0,[r1]`, a second zero): a
  `volatile u8` store and a `u8 tmp8 = 0` temp are both byte-neutral (+8, 26.6%). The byte zero is not
  authorable by a temp: cse still merges it with the halfword zero, or the statement is unchanged.
- One distinct temp per block (W95-A lever): does not apply. There is no reused compare/bound variable here;
  the shared object is the loop counter itself, which the wave-72 split table already measured. Transfer: NO.

Proposed summary: status +8 bytes, 26.6%; left unchanged (the ROM keeps the shared counter in r7 and
the sl/sb/r8 pointer set differs); tried gains: counter types (six spellings), byte-zero temp / volatile store.

</details>
