# sub_08047190

## Wave 93 (W93-D) -- 77.32% -> 88.85%, size-exact, by four chained permuter runs

The park's own suggestion -- "the current best came from the permuter, so
another permuter run from it is a reasonable next step" -- is right, provided
the runs are CHAINED. Four runs, each started from the previous one's kept
improvement, gave 77.32 -> 77.86 -> 87.93 -> 88.85, and the fifth found
nothing. The big step was run 2 (+10 points).

All three kept changes are semantically neutral and were each read:
  * the type compare in the second scan written with the constant on the left,
    `if (t == gUnknown_08499594[...].type)`;
  * that same element bound to `e` before the compare (`e` is assigned before
    every one of its reads throughout the function, so no stale value is
    possible);
  * the loop bound's `+ 1` taken from a u8 local set to 1 immediately before
    the loop (`q3`, which is separately re-initialised to 0 before its later
    use as a counter).
`drafts.py bases` reports no read-before-set and names this file as the base.

WHAT IS LEFT, 144 of 1292 bytes, first difference +0x11: the same class as
before, register choice and instruction order in the first scan, cascading
through everything after it because the function makes no calls. The three
zero-initialisations at the top now land in r5/sl/r9 where the ROM uses
r9/sl/r3, and one `ble` moves relative to a `movs`. Nothing structural is
missing.

## wave 97
Base: sub_08047190.c (88.85%, size-exact, first diff +0x11), unchanged. The parked description holds: the ROM's three zero inits are `movs r0,#0; mov r9,r0; mov sl,r0; movs r3,#0` (rank in r3), the draft's `movs r5,#0; mov sl,r5; mov r9,r5` (rank in r5).
Init spellings, each compiled: `rank = 0; o = (n = 0);` identical to the draft (88.85%); `n = (o = 0); rank = 0;`, `n = 0; o = 0; rank = 0;`, `n = 0; rank = 0; o = 0;`, `rank = (o = (n = 0));`, `o = (n = 0); rank = 0;` all grow to 1296 bytes (+4), 18.4-18.6%. So the chained `o = (rank = 0)` is what keeps the size; any other grouping adds a copy.
Permuter, one 900 s run (2 threads) from the base: NO-IMPROVEMENT (best candidates 2540/2840 vs base 2960 were rejected by the verifier; draft restored).
Proposed summary: does = builds the sorted unit list for the current army (rank order, optional hp/fuel/ammo sort, transports followed by cargo); status = 88.9%, size-exact; left = register choice in the first scan (rank r5 vs r3, the two hi-register counters) cascading through the call-free body; tried = init groupings (six), six chained permuter runs across waves 93-97.

## wave 97 (W97-Q)
Base unchanged (88.85%, size exact, first diff +0x11). Residual sites read off the diff: (1) inits: ROM is `movs r0,#0; mov r9,r0; mov sl,r0; movs r3,#0` = n and o share one zero temp, rank has its OWN `movs` and lives in r3; the draft chains all three from r5. `n = (o = 0); rank = 0;` (w97q-f) reproduces that shape (`movs r0,#0; mov sl,r0; mov r9,r0; movs r5,#0`, only the sl/r9 order and rank's register differ) but is +4 bytes and 18% because of the two sites below. (2) loop-entry test of the ascending scan: ROM `cmp r2,r0; bge` (k held in r2, NOT folded to 1) vs ours `cmp r0,#1; ble` (k=1 folded). Spelling k's start as `q3`, `-~0`, `q3 - 0`, `(q3 = 1)` in the for-init, `k <= rank`, `rank + 1`, `(int)q3`: all byte-identical to the base (or 78% for `<= rank`), so k's start is folded in every spelling; the ROM's k start is opaque to cse for a reason not found. (3) second scan: ROM compares `cmp r0(type), r2(t)` with t used in place; ours copies t (`adds r3,r2,#0`) before the compare. Spellings `type == t` inline (88.2%), `(s8)t ==` (same as base), bound `e` in either order (1288 B, 52.8%) do not remove the copy.
Proposed summary addition: tried = k start spellings (six, no effect), split init `n = (o = 0); rank = 0;` (right shape, +4 from the k fold and the t copy), compare-side spellings.


## wave 97 (W97-AB)

Base: 88.85% draft (`sub_08047190.w97ab-start.c`, size-exact, 144 bytes differ, first diff +0x11). Now 89.47%, size-exact, **136 bytes differ**, first diff still +0x11 (`sub_08047190.w97ab-perm1-start.c` is the adopted file).
Lever (`levers.py`, 3a): step the first loop's inner counter through a copy, `for (j = 1; j <= 0x3f; j = lv0) { lv0 = j + 1; ...` with `int lv0`. 88.85 -> 89.47, wrongc-clean lever. Honest caveat: the ROM's inner counter is the plain `adds r0,r4,#1; lsls; lsrs r4` at the bottom, and this lever computes `j + 1` at the top of the body, so the gain is mostly positional (fewer bytes differ afterwards); it is not the ROM's construct.
Permuter (480 s, 2 threads) from that file: NO-IMPROVEMENT.
Residual, re-read against the ROM: the three zero inits (ROM `movs r0,#0; mov r9,r0; mov sl,r0; movs r3,#0`, i.e. n and o share a temp, rank has its own `movs` and lives in r3); the ascending scan's entry test is `cmp r2,r0; bge` in the ROM (k start not folded) vs `cmp r0,#1; ble`; the ROM hoists `arg + 0x21` into the first loop's outer body (`adds r0,r7,#0; adds r0,#0x21; str r0,[sp,#0x60]`), which the draft also does. No new lever on the k-start fold or the init grouping (the previous notes' six init spellings and six k-start spellings stand).
Unexplored: the ROM's outer/inner counter roles per loop were not tabulated for this function the way they were for sub_0802F03C (the role swap that matched it); with i, k, t, g, j, s, q, q2, q3 all separate s8/u8 locals here the analogue would be which of them share a register across the sibling scans.
