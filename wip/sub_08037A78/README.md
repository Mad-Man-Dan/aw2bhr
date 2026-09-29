# sub_08037A78

0x08037A78, 268 bytes, THUMB, parked.

Best score so far: 85.8%.

## What it does

For every map cell that holds a unit, draws a small tile chosen by the unit's army colour into the buffer a1, masking out what was there first. What screen this draws is not confirmed.

## How close it is

Compiles to the right size (268 bytes) with 85.8% of bytes identical, and the stack frame now matches. Both loops are written out by hand with the map pointer loaded in the loop tests. What is left: the mask table's address is loaded inside the loop where the original loads it before the outer loop, and two saved registers are swapped.

## What is left

The original loads the map pointer as part of the outer loop's first test and saves it to the stack only after that test, and its inner loop's first test runs before the second pointer load; every spelling tried puts the load before the test. Not yet on record: making the load part of the loop condition itself (an assignment inside the condition).

## Already tried

- Using the map global directly with no local: the compiler cannot move the load out of the loop and reloads it in every inner iteration.
- One map-pointer local instead of two: 20.9%, worse than the two-local draft.
- Rotating the loop into a do/while: gets the original's 20-byte frame but reloads the pointer in the loop body and breaks the exact tail.
- Putting `map + 0x12` in its own local before the inner loop: 24-byte frame (original 20), more spills.
- Writing the stores through a two-halfword struct so they cannot alias the global: byte-identical.
- The automatic permuter for 45 minutes from the size-exact variant: no structural progress.

## Files

- `sub_08037A78.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 264/268 (-4), 27.6%. Body, increments and tails are byte-exact; residual remains in two loop preheaders. A rotated do/while obtains the ROM's 20-byte frame but reloads P2 in the body and breaks the exact tail, so the original fixpoint is retained. WAVE 61: permuter 2700s / 4 threads from the size-exact draft (32.5%). Best returned was 31.0% size-exact, i.e. slightly WORSE, with an excursion to 248 bytes (-20) at 13.4%. Restored unchanged. No structural movement.

### Wave 95

Base: the old draft (`sub_08037A78.w95-start.c`, 27.6%, -4). Result: **39.2%, size-exact (268)** in
`sub_08037A78.c` (variant kept as `v3.c`). The first difference is still at +0xa because the frame is
0x10 where the ROM's is 0x14.

The named untried step (assignment inside the loop condition) was tried and is the seed of the
improvement, but in a specific form:
- `for (y = 0; y < (map = X)->unk02; y++)` with the same in the inner loop: +4, 23.2% (a `for` re-tests
  the assignment at the bottom, so the load is repeated where the ROM's bottom test reads the spill).
- Hand-inverted outer loop with the assignment in the guard only:
  `y = 0; if (y < (map = X)->unk02) do { ... } while (++y < map->unk02);` -> 264 bytes, 18.3%.
- **Also hand-invert the inner loop, guard on the OUTER `map`, load `p` after the guard:**
  `x = 0; if (x < map->unk00) { p = X; do { ... } while (++x < p->unk00); }` -> size-exact, 39.2%. This
  is what the ROM does: the inner guard reads the outer pointer from its spill slot and only then loads
  the second pointer into ip, and the inner bottom test uses that second pointer.
- Sharing `y * 2` between `p->unk417a[y]` and `gUnknown_030032E0[y]` through an `int y2 = y * 2` temp
  and byte-pointer arithmetic: 29.1% (worse). The ROM does share `sl = y*2`, but this spelling does not get there.

Residual: (1) the ROM stores the map pointer to its slot AFTER the outer guard's `bge` (the draft stores
it right after the load, before the `ldrh`); (2) frame 0x14 vs 0x10: the ROM hoists `p + 0x12` and
`p + 0x417a + y*2` as two separate slots ([sp,#8] and [sp,#4]) and keeps `gUnknown_030032E0` in the loop
body, the draft folds `030032E0 + y*2` into one hoist; (3) literal pool order of the tail globals.
The one-temp-per-block lever (W95-A): no separate compare/bound pair here; not applicable. Transfer: NO.

Proposed summary: status 39.2%, size-exact; left: the ROM stores the map pointer after the outer guard and
spills two separate hoists (`map+0x12`, row pointer) while leaving `gUnknown_030032E0 + y*2` in the loop;
tried: assignment in the `for` condition (+4), inverted outer loop only (-4), inverted both loops with the
outer-pointer guard (size-exact), shared `y*2` temp (worse).

### wave 95, permuter (four chained 600 s runs, 2 threads, from the 39.2% size-exact draft)

39.2% -> 80.6% -> 83.6% -> 85.8% -> NO-IMPROVEMENT. All size-exact (268), frame now the ROM's `sub sp,#0x14`
(the `sp` slot numbers differ). Kept in `sub_08037A78.c` (`.w95-perm3-out.c`). Every kept mutation read, all
the same C: `new_var = &p->unk12;` (binds `map + 0x12`, the ROM's separate hoisted slot), `new_var2 =
gUnknown_0849D534;` (the AND-mask table pointer bound in the inner body), `new_var3 = map;` (a second
copy of the outer pointer used for the inner guard and the outer bottom test), `(v >> 3) >> 3` for `v >> 6`
(v is u8), and the bind moved later in the body. The extra pointer copies are the W95-A lever (one distinct
temp per block for a value that is both a compare operand and a base): **transferred, yes** -- the ROM's
`str r1,[sp,#12]` slot + separate compare use needed a copy of `map`, and the permuter found it.
Residual (38 of 268 bytes): the mask-table pool word is loaded in the outer preheader in the ROM
(`ldr r4,=0849D534; mov sb,r4` before the outer loop) and inside the body in the draft; the r8/sb pair is
swapped; slot numbers [sp,#4/8/12] are permuted. Names `new_var`, `new_var2`, `new_var3` are still permuter names
(`tileBase`, `maskTable`, `outerMap` would be plain); rename when done, re-measure.

</details>
