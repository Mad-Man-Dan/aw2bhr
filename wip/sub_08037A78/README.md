# sub_08037A78

0x08037A78, 268 bytes, THUMB, parked.

Best score so far: 98.1%.

## What it does

For every map cell that holds a unit, draws a small tile chosen by the unit's army colour into the buffer a1, masking out what was there first. What screen this draws is not confirmed.

## How close it is

Compiles to the right size (268 bytes) with 98.1% of bytes identical. Binding the mask table before the outer loop and the row table (`rowTable = gUnknown_030032E0;`) before the inner loop puts both addresses where the original has them. What is left: two stack slots are swapped (the original keeps the row pointer at [sp,#4] and the outer map at [sp,#12]); declaration order does not move them.

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

### Wave 96

Base: wave-95 draft (85.8%, size-exact). Pre-registration CONFIRMED in part: binding the mask table before the outer
loop (`maskTable = gUnknown_0849D534;` right after `outerMap = map;`, deleted from the inner body) puts the
`ldr r4,=0849D534; mov r9,r4` in the outer preheader like the ROM and fixes the r8/sb swap. Alone it is 29.4% +4: it also
makes loop.c hoist `gUnknown_030032E0 + y*2` into a register (the ROM leaves it in the loop body) and the size grows
by 4. Every respelling of the `dst` expression (order, split statements, row-base temp) is byte-identical 29.4%.
A chained permuter run (900 s, 2 threads) from that start: 29.4% -> 97.4%, one kept change: a local
`rowTable = gUnknown_030032E0;` bound in the inner preheader (after `p = ...`) and `rowTable[y]` in the dst
expression. It is plain C: an invariant table-address bind, which is what keeps the sum out of the hoist.
Then declaring `outerMap` second (right after `maskTable`): 98.1%. Residual (5 bytes, all stack slot numbers): the ROM's
spill slots are row pointer [sp,#4], `map+0x12` [sp,#8], outer map [sp,#12], y+1 [sp,#16]; ours puts the outer map at
[sp,#4] and the row pointer at [sp,#12]. All single moves in the 11-local declaration order and eight multi-move orders
score 97.4-98.1%, so it is not declaration order; slots follow allocno priority (refs / live length), and the outer map
needs a LOWER priority than the row pointer. The permuter cannot see this (its text score ignores the slot numbers:
"starting point already scores 0" at 97.4%). Next: raise the row pointer's ref count or shorten the outer map's live
range (e.g. reload it from `gUnknown_08499590` instead of a held copy in one of its three uses).
Proposed summary: status 98.1% size-exact, only spill-slot numbering differs; left: swap of two spill slots between the outer map pointer and the row pointer.

### Wave 97

wave 97
Base: wave-96 draft (98.13%, size-exact), unchanged (`sub_08037A78.w97-start.c`). No improvement.

Mechanism SETTLED with `-da` (tools/rtldump.py --flags=-da): reload gives spill slots in ASCENDING PSEUDO NUMBER,
lowest pseudo at the lowest offset. Draft: [sp,#4] = pseudo 24 (`outerMap`, 2nd declared local), [sp,#8] = 29
(`cellRow`, 7th local), [sp,#12] = 51 (the row pointer `p + 0x417a + y*2`, an EXPAND-time temp created inside the
`v = ...` statement), [sp,#16] = 153 (y+1, a later temp). The ROM has row pointer < cellRow < outerMap, i.e. the
row-pointer temp is numbered BELOW the `map+0x12` value and the outer map. Pre-registration held (slots follow
pseudo number, not declaration position or priority).
Consequence: every user local has a pseudo number below every expand-time temp, so no declaration order can put
the row temp (51) below `cellRow` (29) or `outerMap` (24). Measured (build/probe/w97e.py, one unit, 20 variants):
- moving `outerMap` after any local: outer and cellRow swap slots (outer #8, cellRow #4), row stays #12. Never #12/#8/#4.
- making the row pointer a named local declared first (`rowIdx = &p->unk417a[y]`): frame grows to 0x18, +8 bytes.
- inlining `cellRow` (`p->unk12[...]`): loses the separate `map+0x12` slot, frame 0x10, -4 bytes.
- declaring `cellRow` in the inner block after a `col` temp (`u16/int col = ...`): frame 0x10 (same loss).
- dropping `outerMap` (using `map`, or re-reading gUnknown_08499590 at the bottom test or the inner guard): frame 0x08-0x10.
So the ROM's `map+0x12` and outer-map values are NOT plain user locals; they look like compiler temps made after
the row temp (cse/gcse copies). What lever creates a hoisted `map + 0x12` temp without a user variable is unfound.
Proposed summary: status 98.13% size-exact, only the three spill-slot numbers differ; left: row pointer must own the lowest slot, `map+0x12` next, outer map third; tried: all declaration orders (slot order follows pseudo number, user locals always below temps), row pointer as a local (+8), cellRow inlined (-4), outerMap dropped (frame shrinks).

### wave 97 (W97-M, second pass)
Chained permuter run 1 (`perm-w97-1.log`, 900 s, 2 threads, from the 98.13% draft): PERMUTE NO-IMPROVEMENT. The one "new best" it logged
(score 40 -> 36) is a header-expanded `output-36-1/source.c` (7,441 lines), not usable; draft restored unchanged. Slot-only residual is
now searchable by the permuter but nothing reordered the pseudo numbers. Hand levers unchanged (see above).

wave 97 (W97-PG)
Permuter chain: 1 link, 98.13% -> 98.13%, NO-IMPROVEMENT.

</details>
