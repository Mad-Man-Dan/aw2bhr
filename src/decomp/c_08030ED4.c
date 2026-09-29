#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030ED4.
 * sub_08030ED4 @ 0x08030ED4
 */

#include "hardware.h"

void LinkShutdown(void)
{
    sub_0802F348();
    REG_RCNT = 0;
    REG_SIOCNT = 0;
    SetIRQHandler(7, 0);
    SetIRQHandler(6, 0);
    sub_0801BB10(1, -0xC1);
    gUnknown_0849B018->unk06 = 0xff;
    gPlaySt.savingEnabled = 0;
}
asm(".global sub_08030ED4\n.thumb_set sub_08030ED4, LinkShutdown\n");
