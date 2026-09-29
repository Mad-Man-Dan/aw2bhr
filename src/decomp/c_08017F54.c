#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017F54.
 * sub_08017F54 @ 0x08017F54
 */

bool8 EventOp_SetPortraitCo(s16 a)
{
    bool8 r;

    if (gUnknown_03002514 != 1)
    {
        if (DivRem(gUnknown_03002F08.unk02, 0x18)
            == DivRem((s16)gUnknown_0200C528[a].unk04->unk08, 0x18))
        {
            gUnknown_03002F08.unk02 = gUnknown_0200C528[a].unk04->unk08;
            LoadPortraitFace(gUnknown_03002F08.unk02);
        }
        else
        {
            gUnknown_03002F08.unk02 = gUnknown_0200C528[a].unk04->unk08;
            gUnknown_0200C528[a].unk0e = 0;
            gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)PortraitWipe_OutStep;
        }

        r = FALSE;
    }
    else
    {
        r = TRUE;
    }

    gUnknown_0200C528[a].unk04++;

    return r;
}
asm(".global sub_08017F54\n.thumb_set sub_08017F54, EventOp_SetPortraitCo\n");
