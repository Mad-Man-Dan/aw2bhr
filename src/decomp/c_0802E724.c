#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E724.
 * sub_0802E724 @ 0x0802E724
 */

bool8 IsValidMoveDestination(s16 a1, s16 a2)
{
    struct Unit **ptbl;
    struct Unit *e;
    int off;
    int army;

    ptbl = &gUnits;

    off = gMap->rowOffset[a2] + a1;
    e = &(*ptbl)[gMap->unit[off]];

    army = gUnknown_03003F38 & 0xc0;

    if (army != gUnknown_03003F2C)
        return FALSE;

    if (gUnknown_03003340[a2][a1] > 0x78)
        return FALSE;

    if (gMap->unit[off] == 0)
        return TRUE;

    if ((gMap->unit[off] & 0xc0) != army)
        return FALSE;

    if ((u8)CanJoinUnits((struct Unit *)gUnknown_030040D8, e) == 1)
        return TRUE;

    if ((u8)IsBoardableTransportAt(a1, a2) == 1)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802E724\n.thumb_set sub_0802E724, IsValidMoveDestination\n");
