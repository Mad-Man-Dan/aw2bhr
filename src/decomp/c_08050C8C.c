#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050C8C.
 * sub_08050C8C @ 0x08050C8C
 */

void ThirdEffect_Loop(void)
{
    u16 side;
    u16 slot;
    u16 t;

    side = gUnknown_03001470[gUnknown_03001FBC].unk30;
    slot = gUnknown_03001470[gUnknown_03001FBC].unk34;

    t = sub_080156C4(gUnknown_03001FBC);

    if (t == 2)
        gUnknown_02028E5C[side][0] = 1;

    if (t == 3)
        gUnknown_020296B0[side].unk1b++;

    if (gUnknown_02029710[side].unk00 == 0 && t == 1
        && gUnknown_02029A10[side].entries[slot].unk1a == 0)
    {
        if (side == gUnknown_0300450C)
            gUnknown_02029664 |= 1;
        else
            gUnknown_02029664 |= 8;
    }
}
asm(".global sub_08050C8C\n.thumb_set sub_08050C8C, ThirdEffect_Loop\n");
