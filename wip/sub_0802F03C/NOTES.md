
## wave 96

Base: existing draft (93.16%, size-exact), unchanged; copy `sub_0802F03C.w96-start.c`. The whole residual is one register swap in the
first `for (i < 4)` loop: the ROM has `i` in r4 and the inner `j` in r5, the draft the reverse (35 differing bytes, all register names;
the later loops then choose r4/r5/r3/r6 differently for the same reason).
Negatives, with mechanism:
- Declaring `int j, i;` instead of `int i, j;`: byte-identical (allocation does not follow declaration order here).
- Wrapping the whole outer loop in `do { } while (0)` (raise priority of the outer counter): 27.9% and +8 bytes; it changes which loops
  keep their counters in registers, not the i/j swap.
- Giving the first loop its own counters (`x`, `y`) and leaving the later loops on `i`, `j`: 21.3% and +4; the wave-56 rule (a counter
  shared with a later loop outranks a fresh one) is visible here, since separating them makes both worse.
- Permuter, one 600 s run from the draft: NO-IMPROVEMENT.
Proposed summary: does = resets the link record, receive rings and send ring; status = 93.2%, size-exact; left = i/j register swap in
the first loop (ROM i=r4, j=r5); tried = declaration order, do-while wrap, separate counters, one permuter run.
