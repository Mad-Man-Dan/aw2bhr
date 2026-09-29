# sub_0806412C — wave 93 (W93-B)

## New base: 85.34% size-exact, up from 56.90%

`best.c` / `recovered.c` (identical files) scored 85.34% at the exact 232 bytes
where the draft scored 56.90%. Diffed statement by statement against the draft
and it IS equivalent C:

- the outer loop's `i = 0; ... for (; i != 8; i++)` became
  `for (i = 0; i <= 7; i++)` — same eight iterations;
- the second loop's `for (i = 0; i <= 5; i++)` became
  `for (i = 0; (i + 1) <= (5 + 1); i++)` — same six iterations;
- a temporary (`new_var`, renamed here to `entries`) holds
  `gUnknown_0202F110`, the second table's base, assigned at the top of the loop
  body and read on the next line. It is the array's ADDRESS, which is a
  constant, so binding it changes no value.

No read-before-set, no call moved or duplicated (this function calls nothing),
the same arrays indexed. Adopted; the draft is backed up as
`sub_0806412C.w93-start.c` and the adopted base as `w93-base-8534.c`.
Re-measured after the `entries` rename: **85.34%, size-exact, first difference
+0x44**, identical to the pre-rename score.

## This overturns two recorded negatives — read them carefully before re-using them

The parked entry says, twice, that `i <= 7` reverses the first loop's counter:
"Every relational form of the first loop (`i <= 7`, `i < 8`, do/while ...): the
compiler rewrites the counter to count down. Only `i != 8` keeps it counting
up (kept)", and "The permuter's saved best (best.c): shares the table constant
as the ROM does, but compiled, its first loop counts down again."

Both were measured with the SECOND loop written `i <= 5`. With the second loop
written `(i + 1) <= (5 + 1)`, `i <= 7` in the first loop is worth 28 points. So
the two loops are not independent: they share the counter pseudo `i`, and what
`check_dbra_loop` does to the first loop depends on how the second loop's exit
test is spelled. The ruled-out-axis entries above are evidence about the exact
pair of spellings measured, not about `i <= 7`.

The `(i + 1) <= (5 + 1)` form is deliberately left as the permuter wrote it. It
is not cosmetic: folding it to `i <= 5` is the spelling the old draft had, and
that is the 56.90% one.

## Residual

232/232, 34 of 232 bytes differ, first difference at +0x44.

## Permuter, 900 s x 4 threads from the 85.34% base: 85.34% -> 88.79%, still size-exact

Kept in `sub_0806412C.c`; the base is `w93-base-8534.c` and the run's output is
also saved as `w93-perm1-8879.c`. Three mutations, all audited as equivalent C
before adopting:

- **`v8 = a8 * 0x1000;` sank from before the outer loop into the INNER loop
  body.** `a8` is a parameter and is never modified, and both loops have constant
  bounds (8 and 3 iterations), so the body always runs and `v8` always holds the
  same value by the time `gUnknown_030005F8 = v8;` reads it. Equivalent. LICM
  hoists it straight back out — the point is that it now creates its pseudo at a
  different position, which is the wave-48 "where a hoisted invariant lands in the
  preheader is set by pseudo-creation order" lever.
- **`j = i; dst = gUnknown_0202F140[j].unk00;`** where the base indexed with `i`.
  `j` is set before it is read and the inner `for` reassigns it immediately
  afterwards, so the same row is taken. Equivalent.
- **`i = 4;` inserted between the `[0]` and `[1]` stores, and `[4]` rewritten as
  `[i]`.** `i` is dead after the second loop, the six stores keep their order, and
  the index value is the same. Equivalent.

None of the three is cosmetic — do not tidy them. Per wave 59, folding a
permuter's temporary back into one statement has cost a matched function before.

## Residual after the run

232/232, 26 of 232 bytes differ, 88.79%, first difference at +0x44. The counter
reversal that dominated this function's history is gone; what is left is 26 bytes
from +0x44 on.

## Reading the residual moved it again by hand: 88.79% -> 90.95%

The diff at +0x44 was a two-register swap with an ORDER behind it. The ROM puts
the outer counter in r1 and the row pointer in r3; the candidate had them the
other way round, and the ROM emitted the counter's init one statement earlier:

    ROM        lsls r0,#16 / ldr r4 / ldr r7 / movs r1,#0 / lsrs r6,#4 / mov ip,r6 / lsrs r0,#4
    candidate  lsls r0,#16 / ldr r4 / ldr r7 / lsrs r6,#4 / mov ip,r6 / movs r3,#0 / lsrs r0,#4

So `i`'s pseudo is created too late. Five positions for `i = 0;` as its own
statement, with the loop written `for (; i <= 7; i++)`:

    at the very top, before `src` ............ 88.79%  first +0x40
    between `src` and `tbl` .................. 89.66%  first +0x42
    after `tbl`, before `v7` ................. 90.95%  first +0x45   <- adopted
    after `v7` ............................... 88.79%  first +0x44
    left in the `for` init (the base) ........ 88.79%  first +0x44

The position is a single optimum, not a direction: one statement either side of
it is worth two points less. Adopted as `w93-base-9095.c`.

## A third combination-specific negative in this function's history

The parked entry lists "the `i = 0` before or after the v7/v8 lines" among the
loop forms that were tried and ruled out. It was — with `i != 8` as the exit test
and `i <= 5` in the second loop. Under this wave's base (`i <= 7`,
`(i + 1) <= (5 + 1)`, `v8` sunk into the inner loop) the same edit is worth 2.2
points. Together with the `i <= 7` reversal that the base overturned, this
function has now had two recorded negatives fail to survive a change of
surrounding spelling. Read every "ruled out" line here as scoped to the exact
combination it was measured in.

## Residual

232/232, 21 of 232 bytes differ, 90.95%, first difference at +0x45.

## Wave 94 (W94-A) - chain closed, 90.95% stands

Permuter run 1 (900 s, 4 threads, `--current` from the 90.95% base) found no
candidate better than the starting point, so the chain is closed here rather
than truncated by budget. The draft is byte-identical to the pre-run copy
(`sub_0806412C.pre-run1.c`). The residual is the two facts already recorded in
`data/parked.json`.

## wave 97 (W97-G)

Base: drafts.py named best.c (90.95%). Its second loop was written `(i + 1) <= (5 + 1)`. **Moved to 97.0% size-exact
(7 of 232 bytes differ, first difference +0x71)** by respelling that loop as the ROM's own shape: the next index
is computed FIRST and assigned back at the bottom, so the counter and the row pointer can share a register:

    for (i = 0; i <= 5; ) { int k = i + 1; entries = gUnknown_0202F110; base = (u8 *)entries;
                            q = base + 2 + i * 8; ...copy loop...; i = k; }

That removed the `adds r3,r1,#1` reorder and the `cmp r0,#6` shape (90.95% -> 96.1%); binding the base as a `u8 *`
and adding the 2 as its own term (`base + 2 + i * 8`) gave the last point (96.1 -> 97.0).
Reading the file: the `v8 = a8 * 0x1000` sits inside the inner loop (same value each pass, kept from the permuter
base; hoisting it was not re-tested); `i = 4; ...[i].unk00 = a5` is the same 96% file's costume. wrongc.py: OK (400 seeds).
Permuter (900 s, 22,738 it, from the 96.1% file): no improvement.

Residual (7 bytes): the ROM makes `entries + 2` a loop-invariant of its own (`ldr r0,=g; adds r4,r0,#2`, then
`lsls r0,r1,#3; adds r1,r0,r4`); ours adds the 2 after the shift (`adds r0,#2`). Every spelling that makes the +2 a
separate statement (`base += 2`, `base = base + 2`, `(u8 *)entries + 2` bound) is folded by cse into a
`gUnknown_0202F110+0x2` pool word (60.8%, seven pool words) -- two states only: folded pool word, or +2 after the
shift. `2 + base + i * 8` is byte-identical to `base + 2 + i * 8`.

Proposed summary: does = fills the eight 3-word vectors from the ROM table scaled to 20.12, the six 4-byte rows,
then stores the eight arguments. status = 97.0% size-exact. left = `+2` of the row base is added after the index
shift instead of hoisted with the base. tried = loop-shape respelling (moved), base binds (above), permuter.

## wave 97 (second pass)

Base: unchanged 97.0% draft. Goal: make `entries + 2` a loop-invariant of its own (`ldr r0,=g; adds r4,r0,#2`).
Measured (spellings.py, 15 variants): `entries[i].unk02` / `gUnknown_0202F110[i].unk02` (array member): 60.8% (folded into a `g+0x2` pool word); `base = (u8 *)&g[0] + 2` and `base = g[0].unk02`: 59.9%; `base = (u8 *)g; base += 2`, `+ (u16)two` block local, entries bound before the first loop: size +8, frame 0x14 (the address goes through a `.LC` rodata word and a7 spills).
ONE spelling produces the ROM's separate `adds r5,r4,#2`: bind `entries = gUnknown_0202F110; base = (u8 *)entries + 2;` between the loops AND write the six trailing stores through `entries[..]`. That gives `add r5,r4,#2` and `q = base + i*8` but the bound `entries` stays live to the stores (r4 held, size -4, 67.7%), where the ROM reloads `ldr r0,=g` after the loop (the pool word is shared). Any spelling that leaves the trailing stores bare (`gUnknown_0202F110[k]`) after the bind switches to the `.LC` indirect word (size +8). Re-binding `entries = gUnknown_0202F110` again before the stores (or a second pointer `e2`, or `entries = 0;` first) also switches to `.LC` (+8).
Conclusion: the ROM needs the bound copy dead after the loop AND bare-global stores that share the same pool word; every spelling gets one of those two, not both. Not matched.
Proposed summary addition (tried): `+2` as a member/array-member address, `(u8 *)` walker with `+= 2`, bind before either loop, bind between loops with the stores through the bind (-4), re-bind before the stores (+8).

## wave 97 (W97-PG)
Permuter chain: 2 links, 96.98% -> 99.14%. Kept link1: constant new_var = 2 used in q = (base + new_var) + i*8 (benign constant). Link2 NO-IMPROVEMENT. Start files .w97pg-perm1/2-start.c.
