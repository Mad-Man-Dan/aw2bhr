#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064FC8.
 * sub_08064FC8 @ 0x08064FC8
 */

/* MATCHED. F092 sibling of sub_080646D4 -- identical apart from the curve
 * (gUnknown_08580A88) and the emitter (sub_08064E5C). See sub_080646D4 for the
 * addend-order note. */
void sub_08064FC8(struct Unk08580934_Obj *obj)
{
    if (obj->unk24 != 0)
    {
        obj->unk24--;
    }
    else
    {
        obj->unk2a = obj->unk38 + gUnknown_08580A88[obj->unk26];
        sub_08064E5C(obj);

        obj->unk26--;
        if (obj->unk26 < 0)
        {
            gUnknown_08580934->unk2d--;
            sub_08030178();
            ClearSlotScriptCallback(gUnknown_03001FBC);
        }
    }
}
