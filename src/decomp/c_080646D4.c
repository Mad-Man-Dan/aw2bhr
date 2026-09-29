#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080646D4.
 * sub_080646D4 @ 0x080646D4
 */

/* MATCHED. Representative of family F092 (RuleOptionEnter_Loop, ArmyColumnEnter_Loop,
 * ArmyColumnEnterCurve_Loop), the per-frame tick of a falling/arcing effect object: count
 * down a delay, then read the next vertical offset out of a ROM curve, hand
 * the object to its emitter, and when the curve runs out decrement the owner's
 * live count and end the whole effect.
 *
 * The one spelling that is not free is the ADDEND ORDER in the unk2a
 * assignment: `obj->unk38 + table[obj->unk26]` and not the other way round.
 * agbcc issues the two loads in the reverse of the order the addends are
 * written, so the natural table-first phrasing puts the unk38 load first and
 * swaps the operands of the final `adds`. Same effect, same length, different
 * bytes.
 *
 * The three `ldrsh` reads are what typed unk24/unk26 signed in
 * unknown-globals.h; see the note there. `obj->unk24--` still emitting `ldrh`
 * on a signed member is the documented "the sign bits cannot survive the
 * strh" narrowing and is not a contradiction. */
void RuleOptionEnter_Loop(struct Unk08580934_Obj *obj)
{
    if (obj->unk24 != 0)
    {
        obj->unk24--;
    }
    else
    {
        obj->unk2a = obj->unk38 + gUnknown_08580948[obj->unk26];
        RuleOption_Draw(obj);

        obj->unk26--;
        if (obj->unk26 < 0)
        {
            gUnknown_08580934->unk2d--;
            LinkRestartKeySync();
            ClearSlotScriptCallback(gUnknown_03001FBC);
        }
    }
}
asm(".global sub_080646D4\n.thumb_set sub_080646D4, RuleOptionEnter_Loop\n");
