#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C8F8.
 * sub_0802C8F8 @ 0x0802C8F8, sub_0802C958 @ 0x0802C958
 */

bool8 sub_0802C8F8(void)
{
    struct Unit *e;
    int off;

    off = gMap->rowOffset[gUnknown_03003100.pos.unk02] + gUnknown_03003100.pos.unk00;

    if (gMap->unit[off] == 0)
        return TRUE;

    e = &gUnits[gMap->unit[off]];

    if (!CanJoinUnits((struct Unit *)gUnknown_030040D8, e))
        return TRUE;

    return FALSE;
}

bool8 sub_0802C958(void)
{
    if (!sub_0802C8F8())
        return TRUE;

    if (!sub_0802CBA0())
        return TRUE;

    FillMovementMap(0xff);
    gUnknown_03003340[gUnknown_03003100.pos.unk02][gUnknown_03003100.pos.unk00] = 0;

    if (BuildCapturableCellList())
        return FALSE;

    return TRUE;
}
