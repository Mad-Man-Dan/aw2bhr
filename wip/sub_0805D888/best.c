#include "global.h"

/* Wave 76 advance: separate int copies plus empty-asm barriers at the two
 * s16 call sites prevent CSE from sharing the conversions across the loop.
 * This removes both unwanted strength-reduced givs and improves the configured
 * result from 544 bytes / 33.1% to 512 bytes / 45.3%. The remaining +4-byte
 * residual is allocation/pressure: the candidate has a 16-byte frame and
 * keeps bestX in sl, while the ROM has a 20-byte frame, spills bestX, and uses
 * sl for the hoisted 0x1f mask. Explicit fixed mask/row-offset registers were
 * tested and regress to 544 bytes, so they do not reproduce that pressure.
 *
 * PARKED at 508 bytes expected / 544 emitted (+36) on Wave 62's current
 * configured remeasurement. The old 536-byte Wave 49 measurement below is
 * stale. `old-agbcc` is also 544 and does not change the strength-reduction
 * mechanism; `old-agbcc-no-force` worsens to 556. `o1` suppresses the excess
 * givs and reaches 504 (-4), but diverges from +0xa and is not a viable
 * override. Giving the body call separate s16 copies increases configured to
 * 552 (+44), so it does not split the shared conversions and is ruled out.
 *
 * Historical Wave 49 note (extension work
 * after the assigned batch closed 4/4).
 *
 * THE SHAPE IS RIGHT AND THE WHOLE RESIDUAL IS ONE THING: agbcc strength-
 * reduces `x << 16` and `y << 16` into two induction variables that the ROM
 * does not have. Everything else is byte-exact -- the prologue, the
 * sub_0805C290 / sub_0805E5AC / sub_0804236C head, both sub_0805D648 call
 * shapes, all three scoring arms cross-jumped into one update block, the
 * `flag`/0x11 early exit, and the tail.
 *
 * The two givs cost 7 extra instructions (their increments at both loop
 * bottoms, plus `mov ip,sl` / `mov r9,r3` to carry them to the exit block) and
 * they also take the two callee-saved registers the ROM spends on the hoisted
 * 0x1f mask (`mov r0,sl; ands r0,r1` in the ROM against `movs r0,#31` here) and
 * on the y*2 giv (sb in the ROM, sp+0x14 here). So the diff reads as four
 * differences and is ONE.
 *
 * Where they come from: `(s16)x` expands to `(x << 16) >> 16`, the `lsl` is
 * `mult (biv, 65536)` and loop.c records it as a giv. It survives loop.c's
 * worth-while test only because the value is live a long time -- CSE has
 * already merged the `(s16)x` at the sub_0804236C call with the `(s16)x` at the
 * `found:` block, so one pseudo spans the whole body. The ROM RECOMPUTES
 * `lsls r0,r4,#0x10; asrs r0,r0,#0x10` at the exit block from the raw counter,
 * which is what keeps each giv's lifetime at two insns and gets it rejected.
 *
 * RULED OUT by probe, so do not re-spend these:
 *   - `s16 x, y` loop counters. Then the s16 formals of sub_0804236C need no
 *     conversion at all and both shift pairs disappear, while the ROM has them;
 *     the counters' own `adds r4,#1` with no re-extension says int as well.
 *   - Writing the exit block inline (`sub_0805D648(...); return;` at the point
 *     of the test) rather than behind a `goto`. The `goto` version above is
 *     strictly better and IS worth keeping: it is what moves the exit block
 *     physically past the loop, which fixed the y/best register assignment
 *     (y -> r6, best -> r8, map pool word -> r7) and merged the three update
 *     blocks. The inline version additionally spilled a third giv and needed a
 *     0x20 frame against the ROM's 0x14; this one needs 0x1c.
 *
 * Wave 66 tested five shape/type spellings at the body call -- `(s16)(u16)x`,
 * `(s16)(u32)x`, `(s16)(x & 0xffff)`, `(s16)(x + 0u)` and `(s16)(x * 1)` (and
 * the same for y). All fold to the identical shared conversions, preserve both
 * unwanted givs and the +36-byte residual. The permuter is the wrong tool here
 * -- this is an extra-instruction residual, not an allocation one.
 *
 * Prototype work done for this function is already committed and is NOT part of
 * the park: sub_0805D648's signature was corrected to (s16, s16, u8, u8, u8) and
 * sub_0802042C's to (int, int, u8 *), both in include/unknown-functions.h with
 * the evidence. sub_0805D648 itself MATCHED. */


void sub_0805D888(void)
{
    int bestX;
    int bestY;
    int flag;
    struct Unit *e;
    int best;
    int x;
    int y;
    int sx;
    int sy;
    int fx;
    int fy;
    u8 t;

    bestY = 0;
    flag = 0;
    e = NULL;
    bestX = -1;
    best = 0;

    t = sub_0805C290(gUnknown_030033EC, 1);
    if (t != 0)
    {
        flag = 1;
        e = &gUnknown_08499594[t];
    }

    sub_0805E5AC();

    if (sub_0804236C(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03) == 1)
        sub_0805D648(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03, 3, 0, 0);

    sub_080202A4(gUnknown_030040D8);

    for (y = 0; y < ((struct Map *)gUnknown_08499590)->height; y++)
    {
        for (x = 0; x < ((struct Map *)gUnknown_08499590)->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;

            sx = x;
            sy = y;
            asm("" : "+r" (sx), "+r" (sy));
            if (sub_0804236C(sx, sy) == 1
                && ((struct Map *)gUnknown_08499590)->unit[((struct Map *)gUnknown_08499590)->rowOffset[y] + x] == 0)
            {
                if ((((struct Map *)gUnknown_08499590)->terrain[((struct Map *)gUnknown_08499590)->rowOffset[y] + x] & 0x1f) == 8
                    && (s8)gUnknown_03003340[y][x] + 8 > best)
                {
                    bestX = x;
                    bestY = y;
                    best = (s8)gUnknown_03003340[y][x] + 8;
                }
                else if (gUnknown_085767F2[((struct Map *)gUnknown_08499590)->terrain[((struct Map *)gUnknown_08499590)->rowOffset[y] + x] & 0x1f] != 0
                         && (s8)gUnknown_03003340[y][x] + 4 > best)
                {
                    bestX = x;
                    bestY = y;
                    best = (s8)gUnknown_03003340[y][x] + 4;
                }
                else if ((s8)gUnknown_03003340[y][x] > best)
                {
                    bestX = x;
                    bestY = y;
                    best = (s8)gUnknown_03003340[y][x];
                }
            }
            else if (flag != 0
                     && (((struct Map *)gUnknown_08499590)->terrain[((struct Map *)gUnknown_08499590)->rowOffset[y] + x] & 0x1f) == 0x11
                     && ((struct Map *)gUnknown_08499590)->unit[((struct Map *)gUnknown_08499590)->rowOffset[y] + x] == 0)
            {
                goto found;
            }
        }
    }

    if (bestX != -1)
        sub_0805D648(bestX, bestY, 3, 0, 0);

    return;

found:
    fx = x;
    fy = y;
    sub_0805D648(fx, fy, 0x14, e->x, e->y);
}











