#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031A24.
 * sub_08031A24 @ 0x08031A24
 */

int LinkWaitMapListAcks(void)
{
    int r;

    if ((gGameClock & 0xf) == 0)
    {
        gUnknown_02025760.unk00 = 0xab;
        gUnknown_02025760.unk01 = gUnknown_0849B018->unk06;
        gUnknown_02025760.unk02 = 0;
        sub_0802F588(&gUnknown_02025760, 0x84);

        gUnknown_0202575C.unk00 = 0xaa;
        gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
        gUnknown_0202575C.unk02 = 0;
        sub_0802F588(&gUnknown_0202575C, 4);
    }

    if (gUnknown_0849B018->unk09 != gUnknown_030044C4)
    {
        r = 1;
    }
    else
    {
        gUnknown_0300410C = gUnknown_030040CC;
        r = 0;
    }

    return r;
}
asm(".global sub_08031A24\n.thumb_set sub_08031A24, LinkWaitMapListAcks\n");
