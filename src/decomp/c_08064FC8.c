#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064FC8.
 * sub_08064FC8 @ 0x08064FC8
 */

/* MATCHED. F092 sibling of RuleOptionEnter_Loop -- identical apart from the curve
 * (gUnknown_08580A88) and the emitter (ArmyColumn_Draw). See RuleOptionEnter_Loop for the
 * addend-order note. */
void ArmyColumnEnter_Loop(struct Unk08580934_Obj *obj)
{
    if (obj->unk24 != 0)
    {
        obj->unk24--;
    }
    else
    {
        obj->unk2a = obj->unk38 + gUnknown_08580A88[obj->unk26];
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
asm(".global sub_08064FC8\n.thumb_set sub_08064FC8, ArmyColumnEnter_Loop\n");
