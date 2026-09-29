#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027984.
 * sub_08027984 @ 0x08027984
 */

void sub_08027984(void)
{
    int v;
    s16 w;

    v = GetSlotSpriteScaleX(gUnknown_03001FBC);
    SetSlotSpriteScaleX(gUnknown_03001FBC, v - 0x10);

    v = GetSlotSpriteScaleY(gUnknown_03001FBC);
    SetSlotSpriteScaleY(gUnknown_03001FBC, v - 0x10);

    v = GetSlotSpriteRotation(gUnknown_03001FBC);
    w = v - 2;
    SetSlotSpriteRotation(gUnknown_03001FBC, w);

    if (w == 0)
    {
        ClearSlotSpriteDoubleSize(gUnknown_03001FBC);
        SetSlotSpriteScriptIndex(gUnknown_03001FBC, 1);
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
