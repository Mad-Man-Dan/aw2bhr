#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065198.
 * sub_08065198 @ 0x08065198
 */

void sub_08065198(struct Unk08580934_Obj *obj)
{
    if (obj->unk24 != 0)
    {
        obj->unk24--;
    }
    else
    {
        obj->unk2a = Interpolate(1, 0x34, -0x30, obj->unk26, 0xc);
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
