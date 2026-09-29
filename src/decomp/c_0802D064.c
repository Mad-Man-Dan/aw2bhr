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
 * sub_0802D0B4 is the same function with LoadUnitIntoTransport in place of
 * ApplyCaptureProgress and id 7; sub_0802D1C0 / sub_0802D1F8 are the two-call variants
 * with ids 0xb / 0xc. */

void sub_0802D064(void)
{
    LockUnitSelection();
    CloseTopMenu();
    sub_0802C57C();
    ApplyCaptureProgress();
    CommitUnitMove();
    sub_0802C594();

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(3, gUnknown_03003F38, 0, 0);
}
