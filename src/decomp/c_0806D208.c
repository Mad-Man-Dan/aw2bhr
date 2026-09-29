#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D208.
 * sub_0806D208 @ 0x0806D208
 */

void RulesScreenRuleOptionEnterSlide_Loop(struct Unk08580934_Obj *obj)
{
    if (obj->unk24 != 0)
    {
        obj->unk24--;
    }
    else
    {
        obj->unk28 = obj->unk2c + gUnknown_08581E70[obj->unk26];

        RulesScreenRuleOption_Draw(obj);

        if (--obj->unk26 < 0)
        {
            gUnknown_08580934->unk2d--;
            ClearSlotScriptCallback(gUnknown_03001FBC);
        }
    }
}
asm(".global sub_0806D208\n.thumb_set sub_0806D208, RulesScreenRuleOptionEnterSlide_Loop\n");
