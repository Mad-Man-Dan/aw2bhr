#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034394.
 * sub_08034394 @ 0x08034394, sub_080343D8 @ 0x080343D8
 */

/* LinkReceiveCommand returns s16 -- the `lsls #0x10; asrs #0x10` on the result is a
 * re-narrowing at the call site, and -1 needs `movs #1; rsbs #0` because THUMB
 * `cmp rN, #imm8` cannot hold it. The `beq` skipping the three stores is the
 * negation of the source `if`, so the source condition is `!= -1`. */
void RemoteTurn_WaitForCommand(void)
{
    RemoteTurn_UpdateCursorAndCamera();

    if (LinkReceiveCommand(&gUnknown_030046C0, IsLinkCommandIdValid) != -1)
    {
        gUnknown_03004780 = 3;
        gUnknown_030045D4 = 0;
        gUnknown_03003F60 = 4;
    }
}
asm(".global sub_08034394\n.thumb_set sub_08034394, RemoteTurn_WaitForCommand\n");

/* `movs r1, #0; ldrsh r0, [r0, r1]` is the register-offset ldrsh agbcc must use
 * because THUMB has no immediate-offset form -- it is what settles
 * gUnknown_03004780 as s16 rather than u16. */
void RemoteTurn_ExecuteCommand(void)
{
    RemoteTurn_UpdateCursorAndCamera();
    AiExecuteActionStep();

    if (gUnknown_03004780 == 2)
        gUnknown_03003F60 = 0;
}
asm(".global sub_080343D8\n.thumb_set sub_080343D8, RemoteTurn_ExecuteCommand\n");
