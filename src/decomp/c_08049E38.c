#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049E38.
 * sub_08049E38 @ 0x08049E38
 */

void DefeatFlow_ResetCounters(void)
{
    gUnknown_02028E3C = 0;
    gUnknown_084C3240->unk2c = 0;
}
asm(".global sub_08049E38\n.thumb_set sub_08049E38, DefeatFlow_ResetCounters\n");
