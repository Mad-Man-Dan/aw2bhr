# sub_08084C14 notes

## wave 97 (W97-M)
Base: current draft (75.86%, size-exact), unchanged; chained permuter run 1 (900 s, 2 threads): PERMUTE NO-IMPROVEMENT.
Classification of the extra copy (read off the diff): the ROM's shared constant 1 is a pseudo in r8 set once in the Left and once in the Right arm
(`movs r1,#1; mov r8,r1`) and the store `p[0x27] = 1` reads it back (`mov r3,r8; strh r3,[r5]`) as does the merged tail (`p[0x32] = 1`, `mov r5,r8`).
The draft already has the r8 constant and the tail use, but its `p[0x27] = 1` store uses the still-live `movs` result directly (`strh r0,[r5]`), so the ROM has
ONE extra `mov rN,r8` per arm. Everything else in the first diff is which low register carries a `movs rN,#0; ldrsh` index.
Tried: a user variable `one` (int / s16 / u16 / u8, declared first / middle / last) assigned `p[0x27] = (one = 1)` and read at `p[0x32] = one` in both arms:
872 bytes (+56), 45.5%; `p` moves to r6 and every register shifts. A user local for the constant re-ranks the allocation function-wide, so the ROM's r8 value is a
compiler temp, not a source variable. The draft's `t = gPlayers` trick and the empty `do { } while (0)` were left alone.
Proposed summary: unchanged (status 75.86% size-exact; left: the store of 1 into p[0x27] must read the r8 copy; a source variable for it costs +56).
