#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802EA5C.
 * sub_0802EA5C @ 0x0802EA5C
 */

#include "hardware.h"

void SioRegisterIrq(struct Unk030040C0 *a1)
{
    gUnknown_02023894 = 0;
    gUnknown_03003F6C = (struct Unk03003F6C *)a1;
    gUnknown_03000578 = 0;
    gUnknown_03000570 = 0;
    gUnknown_03000560 = a1->unk08 & 3;
    gUnknown_03000564 = a1->unk00;

    if (a1->unk06 == 0)
        a1->unk06 = 10;

    REG_RCNT = 0;
    REG_SIOCNT = gUnknown_03000560 | 0x2000;
    *(vu16 *)(REG_BASE + 0x10E) = 0;

    SetIRQHandler(7, (void *)SioHandleIrq_Serial);
    SetIRQHandler(6, (void *)SioHandleIrq_Timer3);
    UpdateInterruptEnable(2, 0x000100C0);

    gUnknown_0300055C = 0xf0;
}
asm(".global sub_0802EA5C\n.thumb_set sub_0802EA5C, SioRegisterIrq\n");
