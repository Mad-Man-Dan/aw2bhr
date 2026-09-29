#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025AEC.
 * sub_08025AEC @ 0x08025AEC
 */

struct Unit *FindFreeUnitSlot(void)
{
    int i;

    for (i = gUnknown_03003F2C + 1; i < gUnknown_03003F2C + 0x33; i++)
    {
        if (gUnits[i].type == 0)
            return &gUnits[i];
    }

    return 0;
}
asm(".global sub_08025AEC\n.thumb_set sub_08025AEC, FindFreeUnitSlot\n");
