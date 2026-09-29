#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060A7C.
 * sub_08060A7C @ 0x08060A7C
 */

void AiConsiderBuildingSubmarine(void)
{
    int r;

    if (gUnknown_030046C0.unk06 == 0)
    {
        r = CountUnitsOfType(0x18);
        if (gUnknown_030045D8 > r)
        {
            if (CountBuildablePropertiesOfKind(6) != 0)
                gUnknown_030046C0.unk06 = 0x18;
        }
    }
}
asm(".global sub_08060A7C\n.thumb_set sub_08060A7C, AiConsiderBuildingSubmarine\n");
