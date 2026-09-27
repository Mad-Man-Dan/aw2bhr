#include "global.h"

/*
 * sub_0801C090 -- copy a sprite's OAM entries into the OAM write cursor.
 *
 * a3 points at a list that starts with an entry count and then holds three
 * halfwords per entry (attr0, attr1, attr2). Each entry is copied to the
 * cursor gUnknown_03002F2C with (a1, a2) added to its position and a4 added
 * to its tile number, and the cursor advances by four halfwords per entry
 * (the fourth is padding the hardware ignores).
 *
 * When bit 12 of a1 -- the OAM horizontal-flip bit -- is set, each entry is
 * mirrored instead: its x is negated and the sprite's width subtracted, with
 * the width read from the 4 x 4 size table gUnknown_0848B56C, indexed by the
 * shape and size fields in the top two bits of attr0 and attr1. Otherwise the
 * two scroll offsets gUnknown_03002B20 and gUnknown_030030D0 are added to x
 * and y.
 *
 * gUnknown_03002F2C is re-read and re-stored every entry because the writes
 * through the cursor may alias it.
 *
 * Why the C looks odd:
 *   - Two nested `do { } while (0)` blocks that each run once, around the
 *     setup and around the loop, together with the two source halfwords
 *     copied into locals in the mirrored branch. That combination is what
 *     makes the compiler hold the entry counter in a stack slot rather than a
 *     register, which is what the original does. None of the three does it
 *     alone, and rewriting the loop as a `for` or a guarded `do/while`
 *     changes nothing at all.
 *   - The counter is signed. The original decrements it in the high half of a
 *     word and adds 0xFFFF0000, which an unsigned counter never compiles to.
 *   - `s0` carries the first source halfword in the mirrored branch and is
 *     then reused for the tile number at the bottom of the body. Sharing one
 *     local is what frees the register the original spends elsewhere.
 *   - Each masked expression goes through the u32 `t` before reaching a u16;
 *     assigning it straight to the u16 makes the compiler shrink the mask.
 *   - This draft does not match yet; see data/parked.json.
 */
void sub_0801C090(s32 a1, s32 a2, void *a3, s32 a4)
{
    u16 *src;
    u16 *dst;
    s16 count;
    u16 x;
    u16 s0;
    u16 s1;
    s16 dx;
    s16 dw;
    u16 attr0;
    u16 attr1;
    u32 t;

    src = a3;

    do
    {
        count = *src++;
        do
        {
            dst = gUnknown_03002F2C;
        }
        while (0);
    }
    while (0);

    do
    {
        while (count != 0)
        {
            if (a1 & 0x1000)
            {
                s1 = src[1];
                s0 = src[0];
                t = (s1 >> 14) * 4 + (s0 >> 14) * 16;
                dw = -*(const u16 *)((const u8 *)gUnknown_0848B56C + t);

                x = s1 & 0x1ff;

                if (s1 & 0x100)
                {
                    t = x | 0xffffff00;
                    x = t;
                }

                dx = -x;

                t = ((a2 | s0) & 0xffffff00) | ((s0 + a2) & 0xff);
                attr0 = t;
                t = ((a1 | s1) & 0xfffffe00) | ((a1 + dx + dw) & 0x1ff);
                attr1 = t;
            }
            else
            {
                t = ((a2 | src[0]) & 0xffffff00)
                    | ((gUnknown_03002B20 + (src[0] + a2)) & 0xff);
                attr0 = t;
                t = ((a1 | src[1]) & 0xfffffe00)
                    | ((gUnknown_030030D0 + (src[1] + a1)) & 0x1ff);
                attr1 = t;
            }

            *dst++ = attr0;
            *dst++ = (attr1 & 0xcfff) | ((a1 ^ src[1]) & 0x3000);
            s0 = src[2] + a4;
            *dst++ = s0;
            dst++;

            gUnknown_03002F2C = (u8 *)gUnknown_03002F2C + 8;
            src += 3;
            count--;
        }
    }
    while (0);
}
