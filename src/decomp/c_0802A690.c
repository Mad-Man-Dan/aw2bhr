#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A690.
 * sub_0802A690 @ 0x0802A690
 */

void FuelUpkeep_Init(void)
{
    gUnknown_03001470[gUnknown_03001FBC].unk38 = 0;
}
asm(".global sub_0802A690\n.thumb_set sub_0802A690, FuelUpkeep_Init\n");
