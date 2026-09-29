# sub_08037A78 — parked at 27.6% (-4 bytes), wave 50, W50-L

## What the function does

For every cell of the map, take the terrain byte at
`map->unk12[map->unk417a[y] + x]`; if it is non-zero, look up a tile quad and
merge it into a destination buffer: AND four halfwords of the destination with
a four-entry mask, then OR in four halfwords of the source tile.

```
dst = (u16 *)a1 + gUnknown_03004010[x] + gUnknown_030032E0[y];
src = (u16 *)gUnknown_080A0F38
    + gUnknown_08582E74[gUnknown_030040F8[(terrain >> 6) + 1] + 0x12];
dst[0] &= mask[0];  dst[2] &= mask[1];  dst[4] &= mask[2];  dst[6] &= mask[3];
dst[0] |= src[0];   dst[2] |= src[2];   dst[4] |= src[4];   dst[6] |= src[6];
```

Everything from the first `ldrh r0,[r1]` of the AND quad to the end of the
function is **byte-exact** in the parked draft, including the inner increment,
both loop tests and the epilogue. The AND-then-OR split into two groups of four
(rather than four `dst = (dst & m) | s` statements) is confirmed: agbcc keeps
each AND result live in r2/r3/r4/r5 and the OR group reuses them.

## The residual, and why it is one fact and not three

The ROM keeps **two** copies of the map pointer:

| pseudo | loaded | used by |
|---|---|---|
| P1 | once, before the outer loop | outer condition (`unk02`), inner-loop **guard** (`unk00`), outer bottom test (`unk02`) |
| P2 | once per outer iteration, in the inner preheader | `map + 0x12`, `map + 0x417a + y*2`, inner **bottom** test (`unk00`) |

P1 and P2 are separate loads of `*gUnknown_08499590` — the pool word
`_08037B64` is referenced twice. Two locals (the parked draft) reproduce that
split; a single local, or no local at all, does not.

What no spelling has reproduced is the **placement**:

* The ROM's P1 is produced *inside* the outer condition (`ldr r1,[r2];
  ldrh r3,[r1,#2]; cmp; bge`) and spilled to `[sp,#12]` only **after** the
  guard branch. `map = gUnknown_08499590;` as a statement makes agbcc emit
  `str` immediately at the def and then `ldr` it straight back for the
  `ldrh` — one insn more, and the reason the candidate is 4 bytes short
  overall despite carrying an extra reload.
* The ROM's inner guard runs **before** P2 is loaded. Every source ordering
  puts the assignment first, because the assignment is a statement in the
  outer body and the guard comes from the `for` header after it.

The two preheader differences fall out of that and are not independent: the ROM
hoists `map + 0x12` into `[sp,#8]` and rematerialises
`gUnknown_030032E0 + y*2` from the pool inside the loop, while every candidate
does the reverse (hoists `gUnknown_030032E0 + y*2` into `sl`, recomputes
`map + 0x12`). The ROM's frame is 20 bytes, the candidate's 16 — one slot for
`map + 0x12` and one for P1. The pool order is the same fact read a third way:
the ROM's `gUnknown_030032E0` word is **last**, i.e. its pseudo is created
inside the loop; the candidate's is fourth.

## Ruled out by controlled probe

1. **`gUnknown_08499590`'s load is not LICM-hoistable.** Writing the global
   directly everywhere (no locals) leaves the `ldr`/`ldr` pair *inside* the
   inner loop, reloaded every iteration. That is `unknown_address_altered`:
   the `strh` stores through `dst` have a varying address, so agbcc's
   `invariant_p` rejects every MEM in the loop. Neither P1 nor P2 can be
   compiler output — both must be written in the source.
2. **`MEM_IN_STRUCT_P` on the stores changes nothing here.** Rewriting the
   four AND/OR pairs as `dst[n].unk00` through a two-halfword `struct Cell`
   (so the stores become COMPONENT_REFs, which in gcc 2.x
   `fixed_scalar_and_varying_struct_p` should stop them aliasing a fixed
   scalar global) produced **byte-identical** assembly to the plain
   `u16 *` spelling. So that disambiguation is either absent from agbcc or
   does not reach `invariant_p`. Worth knowing before anyone else spends a
   probe on it.
3. **Authoring the hoist backfires.** Adding `tiles = p->unk12;` before the
   inner loop to force `map + 0x12` into a slot gave agbcc a 24-byte frame
   (ROM: 20), spilled the map pointer as well and moved the loop counter into
   `ip`. This is the brief's "anything after a hoisted invariant was written by
   the loop optimiser and must not be authored", confirmed once more.
4. Single-local variants: `map` at the outer top used for everything (guard and
   bottom both read it, no P1) scored 20.9%; two locals scored 27.6%.

## Symbols — all settled, nothing left to declare

Added to `include/unknown-globals.h` this wave with evidence:
`gUnknown_030032E0`, `gUnknown_03004010` (lds-bound IWRAM u16 tables),
`gUnknown_0849D534`, `gUnknown_08582E74` (ROM const u16 tables).
Already present: `gUnknown_08499590` (`u8 *`), `gUnknown_030040F8` (`u8 []`),
`gUnknown_080A0F38` (`const u8 []`, so the tile reads cast to `const u16 *`).

`struct Map37A78` is kept local to the `.c`, exactly as the matched exemplar
`src/decomp/c_080377C4.c` keeps `struct Map377C4`. The shared
`struct Unk08499590` in the header has no member at 0x12 and **must not be
reshaped** to add one — W50-G verified two functions byte-for-byte against its
current layout this wave.

## Wave 65 closure

The configured Wave 65 recheck is unchanged: target 268 bytes, candidate 264
(-4), 27.6% positional identity.  The exact tail remains byte-identical.  The
two-source-pointer requirement and the impossible placement of P1's spill after
the outer guard still exhaust the source-level axis; authoring the missing
hoist was already measured to worsen the frame.  Closed without another
fixpoint extension.

## wave 95

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

## wave 96

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

## wave 97

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

## wave 97 (W97-PG)
Permuter chain: 1 link, 98.13% -> 98.13%, NO-IMPROVEMENT.
