#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08059AEC.
 * sub_08059AEC @ 0x08059AEC
 */

void AiMarkAttackRings(void)
{
    u8 n;
    int i;

    if (GetUnitFiringRangeWithCoBonus(gUnknown_030033EC, gUnknown_030040D8->unk00) > 1)
        n = GetUnitFiringRangeWithCoBonus(gUnknown_030033EC, gUnknown_030040D8->unk00);
    else
        n = gUnknown_085D5ABC[gUnknown_030040D8->unk00].minRange;

    for (i = 0; i < n; i++)
        MapMarkHalo((u8)(0x79 + i));
}
asm(".global sub_08059AEC\n.thumb_set sub_08059AEC, AiMarkAttackRings\n");
