#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D46C.
 * sub_0806D46C @ 0x0806D46C
 */

void RulesScreenArmyColumnExitLeft_Loop(struct Unk08580934_Obj *obj)
{
    if (obj->unk24 == 0)
    {
        obj->unk30 += obj->unk34;
        obj->unk28 += obj->unk30;
    }
    else
    {
        obj->unk24--;
    }

    RulesScreenArmyColumn_Draw(obj);

    if (obj->unk28 < 0)
    {
        gUnknown_08580934->unk2d--;
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
asm(".global sub_0806D46C\n.thumb_set sub_0806D46C, RulesScreenArmyColumnExitLeft_Loop\n");
