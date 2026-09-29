
## wave 95

Base: existing draft (468/472, -4, 31.8%).
- Pre-registered hypothesis (local `s5 = side * 5` used in loop 2, side*40 recomputed from it as `s5 * 8` inside the loop via `gUnknown_020296BC[0][s5 * 8 + out]`, so the tails differ): 25.6%, still 468 (-4). Refuted as written: the size is unchanged and the pool/register order moved further from the ROM (mov r9/r8/sl rotation in loop 3). The recomputed side*40 does not change whether jump2 cross-jumps the two tails; the tails end at the shared `strh` regardless of how the index was prepared.
- Not permuter-run (budget went to the three permuter-friendly functions).
