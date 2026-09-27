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
