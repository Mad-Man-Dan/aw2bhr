#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060684.
 * sub_08060684 @ 0x08060684, sub_080606A0 @ 0x080606A0
 */

void DiveSelectedUnit(void)
{
    StartSubmarineDiveEffectDive();
    gUnknown_030040D8->unk01 |= 0x20;
}
asm(".global sub_08060684\n.thumb_set sub_08060684, DiveSelectedUnit\n");

void SurfaceSelectedUnit(void)
{
    StartSubmarineDiveEffectRise();
    gUnknown_030040D8->unk01 &= ~0x20;
}
asm(".global sub_080606A0\n.thumb_set sub_080606A0, SurfaceSelectedUnit\n");
