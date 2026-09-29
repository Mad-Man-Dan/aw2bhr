#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066B8C.
 * sub_08066B8C @ 0x08066B8C
 */

void RuleOption_DrawArrows(int index)
{
    struct Unk08580934_Obj *obj;

    obj = gUnknown_08580934->unk54[index];

    if (obj->unk47 == 1 || obj->unk48 != 0)
        DrawUpArrow(obj->unk28 + 0xa, obj->unk2a - 0x10);

    if (obj->unk47 == 1 || obj->unk48 < obj->unk4b - 1)
        DrawDownArrow(obj->unk28 + 0xa, obj->unk2a + 0x1f);
}
asm(".global sub_08066B8C\n.thumb_set sub_08066B8C, RuleOption_DrawArrows\n");
