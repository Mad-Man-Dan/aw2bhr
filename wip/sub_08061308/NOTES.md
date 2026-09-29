# sub_08061308 — Wave 63 follow-up

The configured draft remains 15.4% (836/864, -28; 703/836 bytes differ,
first difference +0x1c). A 300-second four-thread decomp-permuter run in Wave
63 did not match. It did leave a 44.9% `best.c`, but byte-level validation of
the reported outputs ranged from -28 to +24 bytes and none matched; the clean
draft was restored unchanged. Treat the permuter output only as hypotheses.

## Wave 72

The case-2 accept path is now written inside case 2 before `continue`. This
reproduces the ROM's duplicated 8-byte accept prefix exactly and advances the
configured draft to 844/864 (-20), 727/844 bytes different (13.5%), first
difference +0x1c. The low score is still positional.

Three distinct first-read spellings for `gUnknown_030046C0.unk06` (volatile
struct, byte-pointer cast, and a bound byte local) all retain its address in r8
across the call split. A fixed-r8 map-pool local increases the frame from 16 to
20 bytes. Compiler profiles are negative: old-agbcc is 828/864 (-36), while
old-agbcc-no-force is 804/864 (-60). Retain the duplicated case-2 draft.

## wave 95

Base: existing draft (844/864, -20, 13.5%), kept as `sub_08061308.w95-start.c`. Now SIZE-EXACT (864), 25.2%, first difference +0xa (frame is `sub sp,#20`, ROM #16: one extra slot, the spilled `i * 4`).
- `gMap` / `struct Map` members (width, height, unit, terrain, rowOffset, move) in place of the file-local cast: BYTE-IDENTICAL to the cast draft (844, 13.5%). The park's named probe changes nothing; it is kept because it is readable.
- The lever is the bind of the map pointer's ADDRESS: `struct Map **mp = &gMap;` bound inside `if (gUnknown_030045C8 != a1)` before the k loop, with every switch-case read written `(*mp)->...` and everything else (k-loop head, loop bounds, `gMap->move`) left on bare `gMap`. That is the chapter "bind the address and leave the first reference bare"; it is what buys the ROM's r8 copy of the address word and the extra `mov r0,r8` before each case, and turns -20 into exact size.
- Variants: mp bound at function top: exact size, 24.2%; bound in the loop body before the first map read: -8, 16.0%; bound after the first compare: -4, 14.4%; `(*mp)` also in the i/j loop bounds: -8, 16.7%.
- Permuter (900 s, 2 threads, chained run 1): NO-IMPROVEMENT (raw form 29.9% only).
- Residual: the ROM keeps `i * 4` in r7 (no slot); the draft spills it to [sp,#16]; the ROM's 030046C0 read uses r3 and the ROM copies the bound word after the first bare read (`mov r8,r2` mid-loop), the draft copies at the loop entry.
- The .rodata pool word for the map pointer: candidate emits `.rodata` for `gMap`; the ROM's word is gUnknown_0816DAF4 (same address); pool words to record if this ever matches: 0816DAEC, 0816DAF0, 0816DAF4.
- Transfer test of the sub_08022BB8 lever (two variables, compare temp assigned before the bound value, one temp per block): NOT applicable, no probe run. That lever needs a narrowed or compare form of the same value beside an `adds rN,rM,#0` copy; here the copy is of the map pointer's ADDRESS word and there is no narrowed twin. The address bind already reproduces the copy (see above).
