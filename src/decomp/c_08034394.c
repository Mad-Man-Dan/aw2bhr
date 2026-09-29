#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034394.
 * sub_08034394 @ 0x08034394, sub_080343D8 @ 0x080343D8
 */

/* sub_080309AC returns s16 -- the `lsls #0x10; asrs #0x10` on the result is a
 * re-narrowing at the call site, and -1 needs `movs #1; rsbs #0` because THUMB
 * `cmp rN, #imm8` cannot hold it. The `beq` skipping the three stores is the
 * negation of the source `if`, so the source condition is `!= -1`. */
void sub_08034394(void)
{
    sub_08034598();

    if (sub_080309AC(&gUnknown_030046C0, sub_08034380) != -1)
    {
        gUnknown_03004780 = 3;
        gUnknown_030045D4 = 0;
        gUnknown_03003F60 = 4;
    }
}

/* `movs r1, #0; ldrsh r0, [r0, r1]` is the register-offset ldrsh agbcc must use
 * because THUMB has no immediate-offset form -- it is what settles
 * gUnknown_03004780 as s16 rather than u16. */
void sub_080343D8(void)
{
    sub_08034598();
    AiExecuteActionStep();

    if (gUnknown_03004780 == 2)
        gUnknown_03003F60 = 0;
}
