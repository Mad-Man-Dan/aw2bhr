#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806DC50.
 * sub_0806DC50 @ 0x0806DC50
 */

void RulesScreenRuleOption_DrawArrows(int i)
{
    struct Unk08580934_Obj *obj = gUnknown_08580934->unk54[i];

    if (obj->unk47 == 1 || obj->unk48 != 0)
        RulesScreenDrawUpArrow(obj->unk28 + 9, obj->unk2a - 0x10);

    if (obj->unk47 == 1 || obj->unk48 < obj->unk4b - 1)
        RulesScreenDrawDownArrow(obj->unk28 + 9, obj->unk2a + 0x1f);
}
asm(".global sub_0806DC50\n.thumb_set sub_0806DC50, RulesScreenRuleOption_DrawArrows\n");
