#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064660.
 * sub_08064660 @ 0x08064660
 */

void sub_08064660(struct Unk08580934_Obj *obj)
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

    sub_080645AC(obj);

    if ((u16)(obj->unk2a + 0x20) > 0xC0)
    {
        gUnknown_08580934->unk2d--;
        LinkRestartKeySync();
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
