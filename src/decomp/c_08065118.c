#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065118.
 * sub_08065118 @ 0x08065118
 */

/* MATCHED. F092 sibling of sub_080646D4 -- identical apart from the curve
 * (gUnknown_08580ABE). It shares sub_08064E5C with sub_08064FC8, so the two
 * differ in one pool word and nothing else. See sub_080646D4 for the
 * addend-order note, and unknown-globals.h for why the three curve symbols are
 * three separate declarations. */
void sub_08065118(struct Unk08580934_Obj *obj)
{
    if (obj->unk24 != 0)
    {
        obj->unk24--;
    }
    else
    {
        obj->unk2a = obj->unk38 + gUnknown_08580ABE[obj->unk26];
        sub_08064E5C(obj);

        obj->unk26--;
        if (obj->unk26 < 0)
        {
            gUnknown_08580934->unk2d--;
            LinkRestartKeySync();
            ClearSlotScriptCallback(gUnknown_03001FBC);
        }
    }
}
