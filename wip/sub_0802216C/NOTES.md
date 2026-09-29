## wave 95

Base: sub_0802216C.w95-start.c (39.5%, size -8, `register int base asm("r8")` pin). Draft now has the pin removed (plain `int base;`, permuter-compatible); NOT scored or permuted this wave (permuter slot was used up by sub_0801C01C and sub_08022BB8's hand work). ROM detail read: a5..a8 and a3 are spilled at [sp,#0..#0x10] (five slots), a2 in sl, a4 in sb, 0x400 set in r8 at the start of the a3==0/0x80 arm. Next step: chained permuter run from the unpinned draft (never run on this function).
