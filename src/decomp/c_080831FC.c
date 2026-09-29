#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080831FC.
 * sub_080831FC @ 0x080831FC, sub_08083484 @ 0x08083484, sub_08083738 @ 0x08083738
 */

#include "hardware.h"
/* One of three sprite builders at 0x080831FC-0x08083738 sharing one shape and
 * one callee set. gUnknown_0861696C is a six-entry phase permutation indexed by
 * DivRem(<phase>, 6); the four-way chain re-evaluates that expression each time
 * because a call is not CSE-able, which is why the ROM has four identical
 * DivRem sequences. The two .rodata words this unit emits are agbcc's own
 * -fforce-addr copies of &gUnknown_0861696C and &gUnknown_08615C04 (the ROM's
 * gUnknown_081D93BC / gUnknown_081D93C0, both dereferenced against baserom.gba),
 * not globals. The affine tail is the c_08032E88.c / c_08027B68.c idiom, with
 * `* 16` and not `<< 4` -- shorten_binary_op folds the multiply on the s16
 * gSinLut element and does not fold a shift.
 *
 * The struct is one type shared by at least four functions, and the union of
 * what they pin is worth having in one place even though the convention keeps a
 * pointer-parameter type in the .c that uses it:
 *   +0x34  int  the interpolated scale/offset; MainMenuCarouselWheel_DrawLabelPlateSlideIn and MainMenuCarouselWheel_DrawLabelPlateSlideOut
 *               write it, MainMenuCarouselWheel_DrawLabelPlate writes it only on one branch
 *   +0x38  int  the same role for MainMenuCarouselWheel_DrawTileFlipIn, which is still unmatched and is
 *               this function's only caller
 *   +0x4c  s16  MainMenuCarouselWheel_DrawLabelPlate's interpolation input (`ldrsh`)
 *   +0x4e  s16  MainMenuCarouselWheel_DrawLabelPlateSlideOut's phase offset (`ldrsh`)
 *   +0x52  u16  the phase counter every one of the four reads (`ldrh`)
 *   +0x60  int  MainMenuCarouselWheel_DrawLabelPlate's two-way branch selector
 * MainMenuCarouselWheel_DrawTileFlipIn is the obvious next target: it shares the struct, the phase
 * table and the whole affine idiom, and it also names gUnknown_08616972, the
 * six s16 y-offsets that sit immediately after gUnknown_0861696C.
 */

struct Unk80831FC
{
    /* 00 */ STRUCT_PAD(0x00, 0x34);
    /* 34 */ int unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x52);
    /* 52 */ u16 unk52;
};
/* Second of the three sprite builders at 0x080831FC-0x08083738. Identical to
 * MainMenuCarouselWheel_DrawLabelPlateSlideIn except for the two Interpolate calls (kind 1 with the endpoints
 * swapped, where MainMenuCarouselWheel_DrawLabelPlateSlideIn uses kind 4) and the phase expression, which adds
 * a signed +0x4e member to the +0x52 one. See work/MainMenuCarouselWheel_DrawLabelPlateSlideIn/ for the shape
 * notes; the two .rodata words are gUnknown_081D93C4 / gUnknown_081D93C8, this
 * unit's -fforce-addr copies of &gUnknown_0861696C and &gUnknown_08615C04.
 */

struct Unk8083484
{
    /* 00 */ STRUCT_PAD(0x00, 0x34);
    /* 34 */ int unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x4e);
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x52);
    /* 52 */ u16 unk52;
};
/* Third of the three sprite builders at 0x080831FC-0x08083738. It takes no
 * animation time: the four-way chain draws at a fixed x, and the affine tail
 * splits two ways on +0x60 -- one branch interpolates the scale from +0x4c, the
 * other uses the unit scale 0x100 for all four matrix terms. See
 * work/MainMenuCarouselWheel_DrawLabelPlateSlideIn/ for the shape notes; the two .rodata words are
 * gUnknown_081D93CC / gUnknown_081D93D0, this unit's -fforce-addr copies of
 * &gUnknown_0861696C and &gUnknown_08615C04.
 */

struct Unk8083738
{
    /* 00 */ STRUCT_PAD(0x00, 0x34);
    /* 34 */ int unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x52);
    /* 52 */ u16 unk52;
    /* 54 */ STRUCT_PAD(0x54, 0x60);
    /* 60 */ int unk60;
};

void MainMenuCarouselWheel_DrawLabelPlateSlideIn(struct Unk80831FC *p, int t)
{
    p->unk34 = Interpolate(4, -0x30, 0, t, 4);

    if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0)
    {
        if (GetCampaignSaveFlag() == 0)
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0xAA98);
        else
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0x2A98);

        PutSprite(3, p->unk34 + 0x90, 0x58, gUnknown_08615C04, 0x2AA4);
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 1)
    {
        if (GetVersusSaveFlag() == 0)
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0xBA98);
        else
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0x3A98);

        PutSprite(3, p->unk34 + 0x90, 0x58, gUnknown_08615C04, 0x3AA4);
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 5)
    {
        if (GetWarRoomSaveFlag() == 0)
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0xCA98);
        else
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0x7A98);

        PutSprite(3, p->unk34 + 0x90, 0x58, gUnknown_08615C04, 0x7AA4);
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 3)
    {
        PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0x5AB0);
        PutSprite(3, p->unk34 + 0x90, 0x58, gUnknown_08615C04, 0x5ABC);
    }

    if (gUnknown_03005968 != 0 && IsCampaignCompletionFlagSet(0x20) != 0
        && gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0)
    {
        p->unk34 = Interpolate(4, 0x10, 0x100, t, 4);

        SetObjAffine(1,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16, p->unk34 != 0 ? p->unk34 : 2),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16, p->unk34 != 0 ? p->unk34 : 2));

        PutSpriteExt(1, 0x25B, 0x153, gUnknown_0848B6CE, 0xD2C8);
    }
}
asm(".global sub_080831FC\n.thumb_set sub_080831FC, MainMenuCarouselWheel_DrawLabelPlateSlideIn\n");

void MainMenuCarouselWheel_DrawLabelPlateSlideOut(struct Unk8083484 *p, int t)
{
    p->unk34 = Interpolate(1, 0, -0x30, t, 4);

    if (gUnknown_0861696C[DivRem(p->unk52 + 2 + p->unk4e, 6)] == 0)
    {
        if (GetCampaignSaveFlag() == 0)
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0xAA98);
        else
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0x2A98);

        PutSprite(3, p->unk34 + 0x90, 0x58, gUnknown_08615C04, 0x2AA4);
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2 + p->unk4e, 6)] == 1)
    {
        if (GetVersusSaveFlag() == 0)
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0xBA98);
        else
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0x3A98);

        PutSprite(3, p->unk34 + 0x90, 0x58, gUnknown_08615C04, 0x3AA4);
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2 + p->unk4e, 6)] == 5)
    {
        if (GetWarRoomSaveFlag() == 0)
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0xCA98);
        else
            PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0x7A98);

        PutSprite(3, p->unk34 + 0x90, 0x58, gUnknown_08615C04, 0x7AA4);
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2 + p->unk4e, 6)] == 3)
    {
        PutSprite(3, p->unk34 + 0x90, 0x48, gUnknown_08615C04, 0x5AB0);
        PutSprite(3, p->unk34 + 0x90, 0x58, gUnknown_08615C04, 0x5ABC);
    }

    if (gUnknown_03005968 != 0 && IsCampaignCompletionFlagSet(0x20) != 0
        && gUnknown_0861696C[DivRem(p->unk52 + 2 + p->unk4e, 6)] == 0)
    {
        p->unk34 = Interpolate(1, 0x100, 0x10, t, 4);

        SetObjAffine(1,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16, p->unk34 != 0 ? p->unk34 : 2),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16, p->unk34 != 0 ? p->unk34 : 2));

        PutSpriteExt(1, 0x25B, 0x153, gUnknown_0848B6CE, 0xD2C8);
    }
}
asm(".global sub_08083484\n.thumb_set sub_08083484, MainMenuCarouselWheel_DrawLabelPlateSlideOut\n");

void MainMenuCarouselWheel_DrawLabelPlate(struct Unk8083738 *p)
{
    if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0)
    {
        if (GetCampaignSaveFlag() == 0)
            PutSprite(3, 0x90, 0x48, gUnknown_08615C04, 0xAA98);
        else
            PutSprite(3, 0x90, 0x48, gUnknown_08615C04, 0x2A98);

        PutSprite(3, 0x90, 0x58, gUnknown_08615C04, 0x2AA4);
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 1)
    {
        if (GetVersusSaveFlag() == 0)
            PutSprite(3, 0x90, 0x48, gUnknown_08615C04, 0xBA98);
        else
            PutSprite(3, 0x90, 0x48, gUnknown_08615C04, 0x3A98);

        PutSprite(3, 0x90, 0x58, gUnknown_08615C04, 0x3AA4);
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 5)
    {
        if (GetWarRoomSaveFlag() == 0)
            PutSprite(3, 0x90, 0x48, gUnknown_08615C04, 0xCA98);
        else
            PutSprite(3, 0x90, 0x48, gUnknown_08615C04, 0x7A98);

        PutSprite(3, 0x90, 0x58, gUnknown_08615C04, 0x7AA4);
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 3)
    {
        PutSprite(3, 0x90, 0x48, gUnknown_08615C04, 0x5AB0);
        PutSprite(3, 0x90, 0x58, gUnknown_08615C04, 0x5ABC);
    }

    if (p->unk60 != 0)
    {
        if (gUnknown_03005968 != 0)
            p->unk34 = Interpolate(4, 0x10, 0x100, p->unk4c, 4);
        else
            p->unk34 = Interpolate(1, 0x100, 0x10, p->unk4c, 4);

        SetObjAffine(1,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16, p->unk34 != 0 ? p->unk34 : 2),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16, p->unk34 != 0 ? p->unk34 : 2));

        PutSpriteExt(1, 0x25B, 0x153, gUnknown_0848B6CE, 0xD2C8);
    }
    else if (gUnknown_03005968 != 0 && IsCampaignCompletionFlagSet(0x20) != 0
             && gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0)
    {
        SetObjAffine(1,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16, 0x100),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16, 0x100));

        PutSpriteExt(1, 0x25B, 0x153, gUnknown_0848B6CE, 0xD2C8);
    }
}
asm(".global sub_08083738\n.thumb_set sub_08083738, MainMenuCarouselWheel_DrawLabelPlate\n");
