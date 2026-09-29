#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064660.
 * sub_08064660 @ 0x08064660
 */

void RuleOptionLeave_Loop(struct Unk08580934_Obj *obj)
{
    if (obj->unk24 == 0)
    {
        obj->unk3a += obj->unk3c;
        obj->unk2a += obj->unk3a;
    }
    else
    {
        obj->unk24--;
    }

    RuleOption_Draw(obj);

    if ((u16)(obj->unk2a + 0x20) > 0xC0)
    {
        gUnknown_08580934->unk2d--;
        LinkRestartKeySync();
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
asm(".global sub_08064660\n.thumb_set sub_08064660, RuleOptionLeave_Loop\n");
