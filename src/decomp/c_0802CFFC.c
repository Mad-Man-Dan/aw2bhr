#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CFFC.
 * sub_0802CFFC @ 0x0802CFFC
 */

/* Five bracketing calls, then one of two gUnknown_030044B0 commands depending
 * on whether the two bytes of gUnknown_030033E8 are both zero.
 *
 * `gUnknown_08090C04` is NOT a global. The word at 0x08090C04 in the ROM is
 * 0x030033E8, i.e. &gUnknown_030033E8, so it is agbcc's own `-fforce-addr`
 * address-constant pool word -- the same thing already recorded for 0x08090D84
 * / 0x08090D88 / 0x08090D8C in unknown-globals.h, and its two neighbours here
 * (0x08090C00 = &gPlaySt, 0x08090C0C = &gUnknown_030033EC) are more
 * of the same run. Naming gUnknown_030033E8 directly reproduces it: agbcc
 * emits the word into this unit's .rodata and holds its ADDRESS in r2 across
 * the branch, which is the `adds r2, r0, #0` / `ldr r2, [r2]` pair. */

void UnitMenu_Wait(void)
{
    LockUnitSelection();
    CloseTopMenu();
    BackupUnitStartPosition();
    CommitUnitMove();
    RestoreUnitStartPosition();

    if (gPlaySt.savingEnabled != 0)
    {
        if (gUnknown_030033E8[0] == 0 && gUnknown_030033E8[1] == 0)
            SendActionCommand(2, gUnknown_03003F38, 0, 0);
        else
            SendMoveCommand(gUnknown_03003F38, gUnknown_030033E8[0], gUnknown_030033E8[1]);
    }
}
asm(".global sub_0802CFFC\n.thumb_set sub_0802CFFC, UnitMenu_Wait\n");
