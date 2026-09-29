#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800105C.
 * sub_0800105C @ 0x0800105C
 */

#include "hardware.h"

int DesignRoomHandleCursorInput(void)
{
    int r;
    int i;
    int v;

    r = 0;

    if ((gMap->unk10 & 0xf) != 0)
        return 0;

    i = (gpKeySt->repeated >> 4) & 0xf;

    v = gUnknown_030033E4.unk00 + gUnknown_08499C7C[i][0];

    if (v >= 0 && v < gMap->width)
    {
        gUnknown_030032C4.unk00 += gUnknown_08499C7C[i][0] * 4;
        gUnknown_030033E4.unk00 = v;

        if (gUnknown_08499C7C[i][0] != 0)
            r = 0x6a;
    }

    v = gUnknown_030033E4.unk02 + gUnknown_08499C7C[i][1];

    if (v >= 0 && v < gMap->height)
    {
        gUnknown_030032C4.unk02 += gUnknown_08499C7C[i][1] * 4;
        gUnknown_030033E4.unk02 = v;

        if (gUnknown_08499C7C[i][1] != 0)
            r = 0x6a;
    }

    return r;
}
asm(".global sub_0800105C\n.thumb_set sub_0800105C, DesignRoomHandleCursorInput\n");
