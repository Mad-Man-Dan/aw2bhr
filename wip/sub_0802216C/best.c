#include "global.h"

/* PARKED. Wave 36, W36-L. Size 560. EVERY STATEMENT IS PRESENT AND IN THE ROM'S
 * ORDER -- the remaining diff is entirely register allocation plus the one
 * extra frame slot that follows from it. Do not re-derive the logic; work the
 * allocation.
 *
 * What is settled and should not be touched:
 *   - the first parameter stays `u16 *`. The ROM materialises `dst + 0x40` and
 *     `dst + 0x42` into a register before each `strh` because 0x40 is past
 *     THUMB `strh`'s 5-bit scaled displacement, which is exactly what
 *     `dst[0x20]` / `dst[0x21]` emit; and the matched caller
 *     src/decomp/c_0802239C.c passes `dst + x + y * 32` on a `u16 *`, so a
 *     0x40-byte struct pointer would rescale that caller's arithmetic.
 *   - `a3 >> 6` BEFORE the `a3 = gUnknown_03003F2C` reassignment goes through
 *     `mov ip, r2` and comes out as `lsrs r0, r3, #0x16` -- the u16 parameter's
 *     entry shift fused with the >> 6. After the reassignment it is a plain
 *     `lsrs r4, r2, #6`. Both fall out of just writing `a3 >> 6` twice.
 *   - `gPlayers[(a3 >> 6) + 1].unk1c` folds the `+ 1` into the
 *     constant: stride 60 gives `lsls #4; subs; lsls #2` and 0x3C + 0x1C = 0x58.
 *   - `if (a4 != 0) a4 = 1;` is a real statement, not a `!!` inside a test.
 *
 * REMAINING DIFF, two coupled register facts:
 *   1. `t` and `r` are SWAPPED: the ROM puts t in r5 and r in r6, this draft
 *      the other way round. It shows as `adds r0,r5,#1; adds r0,r6,r0` in the
 *      ROM against `adds r0,r6,#1; adds r0,r5,r0` here, and likewise for the
 *      `+2` and `+3` arms. `t` is created first in both, so the first-created
 *      pseudo wins r5 in the ROM and loses it here -- the same shape as the
 *      sub_08022428 park in this batch. See docs/agbcc-codegen.md, which
 *      records that DECLARATION order is not the lever (measured, byte-identical
 *      both ways).
 *   2. ONE EXTRA FRAME SLOT: the ROM's frame is 0x14 and this draft's is 0x18,
 *      because a2 gets spilled to [sp,#0] here while the ROM keeps it in sl.
 *      That shifts a5/a6/a7/a8/a3 from [sp,#0/4/8/0xc/0x10] to
 *      [sp,#4/8/0xc/0x10/0x14] and accounts for most of the differing bytes.
 *      The ROM's assignment is dst=r7, a3>>6=r4, t=r5, r=r6, 0x400=r8, a4=sb,
 *      a2=sl; this draft spends r8 on `a3 >> 6` and never parks the 0x400
 *      constant, which is the value that has to go back into r8.
 *
 * Next things to try, cheapest first: bind 0x400 to its own local so it becomes
 * a pseudo with the three references the ROM gives it; bind
 * `n = (a3 >> 6) + 1` after the a4 normalisation so it is created at a fixed
 * point; then decomp-permuter, since the instruction order is already right and
 * that is the case it exists for.
 *
 * Wave 65 tried the first of those as the final hand probe. An `int base =
 * 0x400` local used at all three sites is folded completely back into the same
 * immediates/pool literal: candidate remains 548 bytes with the same residual.
 * A named scalar constant is therefore not enough to create the ROM's r8
 * pseudo. Wave 71 fixed the local to r8; that does create the ROM's
 * `movs #0x80; lsls #3; mov r8` and groups all three uses, but a2 remains
 * spilled at [sp,#0] and a3>>6 moves to r9. Fixing the constant is only half
 * of the coupled allocation decision. */
void sub_0802216C(u16 *dst, u8 a2, u16 a3, u8 a4, u8 a5, u16 a6, u16 a7, u8 a8)
{
    u16 t;
    u16 r;
    register int base asm("r8");

    if (a3 == 0x100)
    {
        a3 = gUnknown_03003F2C;
        t = gUnknown_0809097C[0];
    }
    else
    {
        t = gUnknown_0809097C[(a3 >> 6) + 1];
    }

    if (a4 != 0)
        a4 = 1;

    r = sub_080261A4((a3 >> 6) + 1, a2);

    if (a3 == 0 || a3 == 0x80)
    {
        base = 0x400;
        dst[0] = t + (r + 1) + base;
        dst[1] = t + r + base;

        if (sub_0802706C(a2, gUnknown_030033EC, (a3 >> 6) + 1))
            dst[0x20] = t + 0x79;
        else if (a4)
            dst[0x20] = t + 0x76;
        else if (a5)
            dst[0x20] = t + 0x77;
        else if (a7)
            dst[0x20] = t + 0x78;
        else
            dst[0x20] = t + (r + 3) + base;

        if (!(gPlayers[(a3 >> 6) + 1].turnState & 2) && a8)
            dst[0x21] = t + 0x7A;
        else if (gUnknown_08090986[a6] < 0)
            dst[0x21] = t + r + 0x402;
        else
            dst[0x21] = a6 + (t + 0x6C);
    }
    else
    {
        dst[0] = r + t;
        dst[1] = r + (t + 1);

        if (sub_0802706C(a2, gUnknown_030033EC, (a3 >> 6) + 1))
            dst[0x20] = t + 0x79;
        else if (a4)
            dst[0x20] = t + 0x76;
        else if (a5)
            dst[0x20] = t + 0x77;
        else if (a7)
            dst[0x20] = t + 0x78;
        else
            dst[0x20] = r + (t + 2);

        if (!(gPlayers[(a3 >> 6) + 1].turnState & 2) && a8)
            dst[0x21] = t + 0x7A;
        else if (gUnknown_08090986[a6] < 0)
            dst[0x21] = r + (t + 3);
        else
            dst[0x21] = a6 + (t + 0x6C);
    }
}





