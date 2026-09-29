#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060930.
 * sub_08060930 @ 0x08060930
 */

void AiConsiderBuildingSupportUnits(void)
{
    if (gUnknown_030046C0.unk06 == 0)
    {
        if (gUnknown_030046B8 & 1)
            AiConsiderBuildingTCopter();
        AiConsiderBuildingApc();
        if (gUnknown_030046B8 & 2)
            AiConsiderBuildingLander();
    }
}
asm(".global sub_08060930\n.thumb_set sub_08060930, AiConsiderBuildingSupportUnits\n");
