#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D064.
 * sub_0802D064 @ 0x0802D064
 */

/* A screen/mode teardown-then-notify: six unconditional calls, then a guarded
 * SendActionCommand command. `gPlaySt.unk32` is reached as
 * `ldr rB,=g; adds rB,#0x32; ldrb` because 0x32 is past `ldrb`'s 5-bit
 * displacement -- addressing, not a member array; the field is already
 * declared in include/unknown-globals.h and is NOT re-typed here.
 *
 * The argument setup at the `bl` is the operand-class grouping from
 * docs/agbcc-codegen.md: the pool `ldr` for gUnknown_03003F38 first, then the
 * three `movs #imm8`, regardless of argument order.
 *
 * UnitMenu_Load is the same function with LoadUnitIntoTransport in place of
 * ApplyCaptureProgress and id 7; UnitMenu_Dive / UnitMenu_Rise are the two-call variants
 * with ids 0xb / 0xc. */

void UnitMenu_Capture(void)
{
    LockUnitSelection();
    CloseTopMenu();
    BackupUnitStartPosition();
    ApplyCaptureProgress();
    CommitUnitMove();
    RestoreUnitStartPosition();

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(3, gUnknown_03003F38, 0, 0);
}
asm(".global sub_0802D064\n.thumb_set sub_0802D064, UnitMenu_Capture\n");
