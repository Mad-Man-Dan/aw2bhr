#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048EC4.
 * sub_08048EC4 @ 0x08048EC4
 */

/* The negative-going twin of ShopScreen_StepOffsetToZero on the same s16: while unk832 is
 * above -0x38 it falls by 8, and the first frame at or past -0x38 pins it
 * there. The value returned is unk832 + 0x38, i.e. the remaining distance,
 * which is why the caller (sub_080490BC) tests the result for zero.
 *
 * The ldrsh is the compare and the ldrh beside it is the -= 8 reading the same
 * word, exactly as in ShopScreen_StepOffsetToZero. Both fetches of gUnknown_084C30F8 go
 * through a .rodata address-constant word (-fforce-addr); here that word is
 * 0x0812A150, one of four in that run (0x0812A150/154/158/15C) that all hold
 * &gUnknown_084C30F8. */
u16 ShopScreen_StepOffsetToMinus38(void)
{
    if (gUnknown_084C30F8->unk832 > -0x38)
        gUnknown_084C30F8->unk832 -= 8;
    else
        gUnknown_084C30F8->unk832 = -0x38;

    return gUnknown_084C30F8->unk832 + 0x38;
}
asm(".global sub_08048EC4\n.thumb_set sub_08048EC4, ShopScreen_StepOffsetToMinus38\n");
