#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CC40.
 * sub_0802CC40 @ 0x0802CC40
 */

int UnitMenu_SupplyUsability(void)
{
    if (!UnitMenu_JoinUsability())
        return 1;

    if (!UnitMenu_LoadUsability())
        return 1;

    if (!HasSupplyAbility((u8 *)gUnknown_030040D8))
        return 1;

    FillMovementMap(0xff);
    MapZeroNeighbors(gUnknown_03003100.pos.unk00, gUnknown_03003100.pos.unk02);

    if (BuildResupplyTargetList())
        return 0;

    return 1;
}
asm(".global sub_0802CC40\n.thumb_set sub_0802CC40, UnitMenu_SupplyUsability\n");
