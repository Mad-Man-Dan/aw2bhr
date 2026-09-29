#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026798.
 * sub_08026798 @ 0x08026798
 */

/* Two statements; RecomputeArmyVisionMasks takes nothing, so the store is not feeding it. */

void ResetUnitCycleAndVisionMasks(void)
{
    gUnknown_030032C0 = 0;
    RecomputeArmyVisionMasks();
}
asm(".global sub_08026798\n.thumb_set sub_08026798, ResetUnitCycleAndVisionMasks\n");
