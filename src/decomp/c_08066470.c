#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066470.
 * sub_08066470 @ 0x08066470
 */

#include "hardware.h"

/* The third member of the affine-sprite tick cluster (sub_08027B68 /
 * sub_08027CC8 / src/decomp/c_0806E240.c): four Div calls building the 2x2
 * matrix from a fixed angle-0 sin/cos pair scaled by 16, each divided by a
 * scale guarded against zero, then the sprite goes out at a fixed OAM slot.
 * Its parameter is `struct Unk08580934_Obj *` and not a proc -- +0x1c, +0x26,
 * +0x28 and +0x2a are already that struct's members, and +0x26 as a proc would
 * sit inside proc_mark.
 *
 * The scale is an INTERPOLATION, which is what makes this member different: it
 * ramps sx up 0 -> 0xc0 and sy down 0x1c0 -> 0x100 over four frames as unk26
 * counts down, so the sprite stretches horizontally while it settles.
 *
 * `obj->unk26` is s16, and the two accesses disagree for the reason recorded
 * on sub_08027B68's unk48: the standalone read is `ldrsh`, while the `--` uses
 * `ldrh` because force_to_mode drops the sign extension when the only
 * consumers are a halfword store and a truncated `== 0` test. The test here is
 * `lsls #0x10; cmp #0` on the value still in the register rather than a
 * re-load, which is the same source with no call in between -- sub_08027B68
 * re-loads with `ldrsh` only because PutSpriteExt clobbers memory first.
 *
 * gUnknown_03001FBC is declared s16 and read here with `ldrb`; that is
 * ClearSlotScriptCallback's u8 parameter folding the truncation into the load, not
 * evidence about the global. */
void ReadyMarkerDisappear_Loop(struct Unk08580934_Obj *obj)
{
    int sx = obj->unk26 * 64;
    int sy = (3 - obj->unk26) * 64 + 0x100;

    SetObjAffine(obj->unk1c,
                 Div(COS_Q12(0) * 16, sx != 0 ? sx : 2),
                 Div(-SIN_Q12(0) * 16, sy != 0 ? sy : 2),
                 Div(SIN_Q12(0) * 16, sx != 0 ? sx : 2),
                 Div(COS_Q12(0) * 16, sy != 0 ? sy : 2));

    PutOamHi((obj->unk28 + 0x200) & 0x1ff, (obj->unk2a + 0x100) & 0xff,
                 gUnknown_08580CFC[obj->unk1c], 0);

    gUnknown_08580934->unk70[obj->unk1c] = 0xff;

    obj->unk26--;

    if (obj->unk26 == 0)
    {
        gUnknown_08580934->unk70[obj->unk1c] = 0;
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
asm(".global sub_08066470\n.thumb_set sub_08066470, ReadyMarkerDisappear_Loop\n");
