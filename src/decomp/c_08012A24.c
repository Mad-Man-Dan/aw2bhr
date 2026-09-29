#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012A24.
 * sub_08012A24 @ 0x08012A24, sub_08012A34 @ 0x08012A34
 */

#include "hardware.h"

#include "hardware.h"


void EnableHBlankInterrupt(void)
{
    gUnknown_030020B4.bits.hblank_int_enable = 1;
}
asm(".global sub_08012A24\n.thumb_set sub_08012A24, EnableHBlankInterrupt\n");

void sub_08012A34(void)
{
    gUnknown_030020B4.bits.hblank_int_enable = 0;
    UpdateInterruptEnable(1, -3);
}
