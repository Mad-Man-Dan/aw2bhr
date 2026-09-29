#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025744.
 * sub_08025744 @ 0x08025744
 */

u8 IsVisibleAllyUnitAtCell(int a1, int a2)
{
    int off;
    int id;

    off = gMap->rowOffset[(s16)a2] + (s16)a1;
    id = gMap->unitUnk[off];

    if (id == 0)
        return 0;

    if (gMap->visible[off] == 0)
        return 0;

    if (gMap->unit[off] == 0 && (gUnits[id].flags & 4) == 0)
        return 0;

    return IsUnitOnCurrentTeam(id);
}
asm(".global sub_08025744\n.thumb_set sub_08025744, IsVisibleAllyUnitAtCell\n");
