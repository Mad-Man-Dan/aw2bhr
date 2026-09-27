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
