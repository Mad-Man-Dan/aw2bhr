#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029880.
 * sub_08029880 @ 0x08029880
 */

void DropCellPicker_Finish(void)
{
    if (gUnknown_030040E4 != 0)
        return;

    EndSlotScriptAt(gUnknown_03001FBC);

    if (gUnknown_03001470[gUnknown_03001FBC].unk24 == 0)
        DropCargoUnit(gUnknown_03001470[gUnknown_03001FBC].unk22);

    LockUnitSelection();
    DecrementMapLock();

    if (gUnknown_03001470[gUnknown_03001FBC].unk24 == 0)
    {
        RebuildMapUnitLayers();

        if (!UnitMenu_DropFirstUsability())
        {
            gUnknown_030033E4.unk00 = gUnknown_03003100.pos.unk00;
            gUnknown_030033E4.unk02 = gUnknown_03003100.pos.unk02;
            sub_0802D558();
            gUnknown_03003334 = 5;
            return;
        }
    }

    gUnknown_03003334 = 0;
    CommitUnitMove();

    if (gPlaySt.savingEnabled != 0)
        SendMoveCommand(gUnknown_03003F38, gUnknown_030033E8[0], gUnknown_030033E8[1]);
}
asm(".global sub_08029880\n.thumb_set sub_08029880, DropCellPicker_Finish\n");
