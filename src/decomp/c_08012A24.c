#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012A24.
 * sub_08012A24 @ 0x08012A24, sub_08012A34 @ 0x08012A34
 */

#include "hardware.h"

#include "hardware.h"


void sub_08012A24(void)
{
    gUnknown_030020B4.bits.hblank_int_enable = 1;
}

void sub_08012A34(void)
{
    gUnknown_030020B4.bits.hblank_int_enable = 0;
    sub_0801BB10(1, -3);
}
