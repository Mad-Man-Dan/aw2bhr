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
