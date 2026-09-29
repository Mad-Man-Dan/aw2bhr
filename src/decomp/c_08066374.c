#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066374.
 * sub_08066374 @ 0x08066374
 */

#include "hardware.h"

/* Sibling of the matched ReadyMarkerDisappear_Loop in src/decomp/c_08066470.c, and the case
 * W20-C's overlap screen flagged as its strongest joint signal: identical callee
 * set AND identical data_refs. It fell on the first draft with no probe round,
 * so the two axes did not separate here -- see docs/agbcc-codegen.md.
 *
 * Everything is the exemplar's, with three differences and no new vocabulary:
 * ONE scale drives all four Div calls instead of the exemplar's separate sx/sy;
 * the countdown wraps (`if (obj->unk26 < 0) obj->unk26 = 3;`) rather than
 * stopping at zero; and the unk70 slot mark is SET to 1 inside the guard rather
 * than held at 0xff every frame and cleared. The `--` still reads `ldrh` while
 * the standalone reads use `ldrsh`, and the guard tests `lsls #0x10; cmp #0` on
 * the value still in the register -- the exemplar's comment explains both and
 * neither needed re-deriving. */

void ReadyMarkerAppear_Loop(struct Unk08580934_Obj *obj)
{
    int s = obj->unk26 * 64 + 0x100;

    SetObjAffine(obj->unk1c,
                 Div(COS_Q12(0) * 16, s != 0 ? s : 2),
                 Div(-SIN_Q12(0) * 16, s != 0 ? s : 2),
                 Div(SIN_Q12(0) * 16, s != 0 ? s : 2),
                 Div(COS_Q12(0) * 16, s != 0 ? s : 2));

    PutOamHi((obj->unk28 + 0x200) & 0x1ff, (obj->unk2a + 0x100) & 0xff,
                 gUnknown_08580CFC[obj->unk1c], 0);

    obj->unk26--;

    if (obj->unk26 < 0)
    {
        obj->unk26 = 3;
        ClearSlotScriptCallback(gUnknown_03001FBC);
        gUnknown_08580934->unk70[obj->unk1c] = 1;
    }
}
asm(".global sub_08066374\n.thumb_set sub_08066374, ReadyMarkerAppear_Loop\n");
