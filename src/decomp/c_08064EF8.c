#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064EF8.
 * sub_08064EF8 @ 0x08064EF8, sub_08064F54 @ 0x08064F54
 */

void sub_08064EF8(struct Unk08580934_Obj *obj)
{
    obj->unk26--;
    obj->unk2a = Interpolate(1, 0x20, 0x34, obj->unk26, 0xB);
    sub_08064E5C(obj);

    if (obj->unk26 == 0)
    {
        gUnknown_08580934->unk2d--;
        LinkRestartKeySync();
        ClearSlotScriptCallback(gUnknown_03001FBC);
        obj->unk2a = 0x20;
    }
}

/* sub_08064EF8 with the two Interpolate endpoints swapped and the settled
 * value to match. */
void sub_08064F54(struct Unk08580934_Obj *obj)
{
    obj->unk26--;
    obj->unk2a = Interpolate(1, 0x34, 0x20, obj->unk26, 0xB);
    sub_08064E5C(obj);

    if (obj->unk26 == 0)
    {
        gUnknown_08580934->unk2d--;
        LinkRestartKeySync();
        ClearSlotScriptCallback(gUnknown_03001FBC);
        obj->unk2a = 0x34;
    }
}
