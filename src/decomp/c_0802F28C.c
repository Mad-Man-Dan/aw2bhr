#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F28C.
 * sub_0802F28C @ 0x0802F28C
 */

/* A bulk reset of the link block. Three short byte clears over the head of
 * gUnknown_02025564, then the twelve 0x1c-byte records at +0x20, then four
 * 13-byte rows of gUnknown_020257E4.
 *
 * TWO COUNTERS, and that is the whole register allocation. The three short
 * loops and both inner loops use `j`; only the two outer loops use `i`. One
 * shared counter puts the outer index in r3 where the ROM has r4 and swaps
 * r2/r3 in all three short loops -- 36 bytes of difference from one variable.
 *
 * The +0x22..+0x32 and +0x34..+0x38 runs really are DESCENDING (`strb` then
 * `subs r0,#1` on the pointer). The last loop is the opposite case: its pointer
 * ASCENDS while the counter counts down, which is check_dbra_loop rewriting an
 * ordinary `j <= 0xc` and not a descending source loop.
 *
 * The dead `ldrb` in front of the short loops' and the last loop's stores is
 * the volatile tell; the record array at +0x20 carries none and is not
 * volatile. See the type comments in include/unknown-globals.h. */
void LinkClearMapListAndNames(void)
{
    int i;
    int j;

    for (j = 0; j <= 1; j++)
        gUnknown_02025564.unk00[j] = 0;

    for (j = 0; j <= 2; j++)
        gUnknown_02025564.unk02[j] = 0;

    for (j = 0; j <= 0x17; j++)
        gUnknown_02025564.unk05[j] = 0;

    for (i = 0; i <= 0xb; i++)
    {
        gUnknown_02025564.unk20[i].unk01 = 0;
        gUnknown_02025564.unk20[i].unk00 = 0;
        gUnknown_02025564.unk20[i].unk13 = 0;
        gUnknown_02025564.unk20[i].unk1a = 0;
        gUnknown_02025564.unk20[i].unk19 = 0;

        for (j = 0x10; j >= 0; j--)
            gUnknown_02025564.unk20[i].unk02[j] = 0;

        for (j = 4; j >= 0; j--)
            gUnknown_02025564.unk20[i].unk14[j] = 0;
    }

    for (i = 0; i <= 3; i++)
    {
        for (j = 0; j <= 0xc; j++)
            gUnknown_020257E4[i][j] = 0;
    }
}
asm(".global sub_0802F28C\n.thumb_set sub_0802F28C, LinkClearMapListAndNames\n");
