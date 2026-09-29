#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CEB8.
 * sub_0803CEB8 @ 0x0803CEB8
 */

void LoadDesignRoomSlot(u8 a1, const void *a2)
{
    ReadSaveSlot(a1 + 5, gUnknown_02000000);
    ApplyMapRecord((int)a2, gUnknown_02000000);
    RemapArmyRosters(gPlaySt.armyColor[1], gPlaySt.armyColor[2],
                 gPlaySt.armyColor[3], gPlaySt.armyColor[4]);
    RebuildMapUnitLayers2();
}
asm(".global sub_0803CEB8\n.thumb_set sub_0803CEB8, LoadDesignRoomSlot\n");
