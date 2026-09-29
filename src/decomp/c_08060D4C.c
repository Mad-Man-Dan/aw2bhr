#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060D4C.
 * AiPickUnitToBuild @ 0x08060D4C, AiCalcMechChance @ 0x08060D78
 */

void AiPickUnitToBuild(void)
{
    u8 v;

    if (gUnknown_030046C0.unk06 == 0)
    {
        AiCalcBuildPriorities();
        AiMarkAffordableUnits();
        v = AiSelectBuildUnit();
        if (v != 0xff)
            gUnknown_030046C0.unk06 = v;
    }
}

asm(".global sub_08060D4C\n.thumb_set sub_08060D4C, AiPickUnitToBuild\n");

void AiCalcMechChance(s16 *p)
{
    if (gUnknown_03004080 <= 3)
    {
        *p = 0;
    }
    else
    {
        *p += gUnknown_03004080 * gUnknown_085766E0->unk03;
        if (*p > 0x50)
            *p = 0x50;
    }
}
asm(".global sub_08060D78\n.thumb_set sub_08060D78, AiCalcMechChance\n");
