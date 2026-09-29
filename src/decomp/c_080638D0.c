#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080638D0.
 * sub_080638D0 @ 0x080638D0, sub_08063928 @ 0x08063928
 */

#include "hardware.h"

void sub_080638D0(int a1)
{
    if (a1 != 0)
    {
        REG_IE |= 4;
        gUnknown_030020B4.bits.vcount_int_enable = 1;
        SetIRQHandler(2, (void *)a1);
    }
    else
    {
        REG_IE &= ~4;
        gUnknown_030020B4.bits.vcount_int_enable = 0;
    }
}

void sub_08063928(int a1)
{
    if (a1 != 0)
    {
        REG_IE |= 2;
        gUnknown_030020B4.bits.hblank_int_enable = 1;
        SetIRQHandler(1, (void *)a1);
    }
    else
    {
        REG_IE &= ~2;
        gUnknown_030020B4.bits.hblank_int_enable = 0;
    }
}
