#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060684.
 * sub_08060684 @ 0x08060684, sub_080606A0 @ 0x080606A0
 */

void sub_08060684(void)
{
    StartSubmarineDiveEffectDive();
    gUnknown_030040D8->unk01 |= 0x20;
}

void sub_080606A0(void)
{
    StartSubmarineDiveEffectRise();
    gUnknown_030040D8->unk01 &= ~0x20;
}
