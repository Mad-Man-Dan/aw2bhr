#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D0B4.
 * sub_0802D0B4 @ 0x0802D0B4
 */

/* See src/decomp/c_0802D064.c: same shape, LoadUnitIntoTransport instead of
 * ApplyCaptureProgress and command id 7. */

void UnitMenu_Load(void)
{
    LockUnitSelection();
    CloseTopMenu();
    BackupUnitStartPosition();
    LoadUnitIntoTransport();
    CommitUnitMove();
    RestoreUnitStartPosition();

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(7, gUnknown_03003F38, 0, 0);
}
asm(".global sub_0802D0B4\n.thumb_set sub_0802D0B4, UnitMenu_Load\n");
