#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065118.
 * sub_08065118 @ 0x08065118
 */

/* MATCHED. F092 sibling of RuleOptionEnter_Loop -- identical apart from the curve
 * (gUnknown_08580ABE). It shares ArmyColumn_Draw with ArmyColumnEnter_Loop, so the two
 * differ in one pool word and nothing else. See RuleOptionEnter_Loop for the
 * addend-order note, and unknown-globals.h for why the three curve symbols are
 * three separate declarations. */
void ArmyColumnEnterCurve_Loop(struct Unk08580934_Obj *obj)
{
    if (obj->unk24 != 0)
    {
        obj->unk24--;
    }
    else
    {
        obj->unk2a = obj->unk38 + gUnknown_08580ABE[obj->unk26];
        ArmyColumn_Draw(obj);

        obj->unk26--;
        if (obj->unk26 < 0)
        {
            gUnknown_08580934->unk2d--;
            LinkRestartKeySync();
            ClearSlotScriptCallback(gUnknown_03001FBC);
        }
    }
}
asm(".global sub_08065118\n.thumb_set sub_08065118, ArmyColumnEnterCurve_Loop\n");
