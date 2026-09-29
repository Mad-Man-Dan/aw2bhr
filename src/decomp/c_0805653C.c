#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805653C.
 * sub_0805653C @ 0x0805653C
 */

/* BuildDeathFigureList @ 0x0805653C
 *
 * Fills gUnknown_0202980A[side][0..] with the slots that are "live" -- entry
 * .unk01 == 0 and .unk00 != 0 -- walking them in the gUnknown_0855218C
 * permutation order, and returns how many it wrote.
 *
 * The return type is u16 and that is read off the CALLER (sub_0805634C): it
 * stores the result back into its own u16 third parameter and passes it on to
 * SetDeathBatchSizes with no narrowing at either point, which a u32 return cannot
 * do. The value returned is this function's own first parameter, which both
 * arms reuse as the running result -- 0/1 in the first, the requested count in
 * the second.
 *
 * `i == 5` sets n = a rather than breaking: that is what the ROM's
 * `cmp r7,#5; bne; adds r6,r5,#0` in front of the bottom test says, and it is
 * why the loop is a `while (n != a)` and not a counted one.
 */

struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u8 filler_08[0x12 - 0x08];
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 filler_14[0x18 - 0x14];
};

u16 BuildDeathFigureList(u16 a, u16 b)
{
    struct Unk85D6A48Row *rows;
    u16 n;
    u16 i;
    u16 t;

    n = 0;
    i = 0;
    rows = (struct Unk85D6A48Row *)gUnknown_085D6A48;

    if (rows[gUnknown_03004580[b][1]].unk04 == 1)
    {
        t = gUnknown_08552148[b];
        a = 0;

        if (gUnknown_02029A10[b].entries[t].unk01 == 0
         && gUnknown_02029A10[b].entries[t].unk00 != 0)
        {
            gUnknown_02029808[b].unk02[n] = t;
            a = 1;
        }
    }
    else
    {
        while (n != a)
        {
            t = gUnknown_0855218C[b][i][0];

            if (gUnknown_02029A10[b].entries[t].unk01 == 0
             && gUnknown_02029A10[b].entries[t].unk00 != 0)
            {
                gUnknown_0202980A[b][n] = t;
                n++;
            }

            i++;

            if (i == 5)
                n = a;
        }
    }

    return a;
}
asm(".global sub_0805653C\n.thumb_set sub_0805653C, BuildDeathFigureList\n");
