#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08083A44.
 * sub_08083A44 @ 0x08083A44, sub_08083EE0 @ 0x08083EE0
 */

#include "hardware.h"
/* Fourth of the block, and the one that shows what the three matched exemplars
 * in src/decomp/c_080831FC.c look like once a loop is wrapped round them: the
 * four-way phase chain and the whole affine tail are transcribed from
 * MainMenuCarouselWheel_DrawLabelPlate unchanged, and everything that cost anything is loop shape.
 * Structure is MainMenuCarouselWheel_DrawTileFlipIn's five-step sprite loop, then MainMenuCarouselWheel_DrawLabelPlate's
 * four-way chain with each arm turned into a two-iteration loop selected by
 * `i == DivRem(p->unk66, 2)`, then MainMenuCarouselWheel_DrawLabelPlate's affine tail verbatim.
 *
 * Two wave-20 rules live here (docs/agbcc-codegen.md, W20-A):
 *   - ONE `int i` is reused by the outer loop and all four inner loops. With a
 *     separate `int j` for the inner ones, all four come out with the counter
 *     and the strength-reduced `i * 16 + 0x48` in swapped registers, and no
 *     spelling of either expression moves them -- sixteen were probed and all
 *     sixteen were byte-identical. A spare local sits between the biv and
 *     loop.c's givs in allocno order and takes the register the giv wants.
 *   - The `i == 0` arm's OAM word is `(i * 12 + 0x298) | 0xA800`, NOT the
 *     literal 0xAA98 the exemplar uses, even though cse folds it to exactly
 *     that constant on a path where it knows i == 0. The fold leaves the value
 *     in a different register than a source literal would, and that is 12 bytes
 *     across the three arms that have it.
 * Its single .rodata word is &gUnknown_08615C04; the exemplars each have two
 * because they also reroute &gUnknown_0861696C, which the loop here hoists into
 * a register instead. Promotion must carry rodata: ["0x081D93D4"]. */

struct Unk8083A44
{
    /* 00 */ STRUCT_PAD(0x00, 0x34);
    /* 34 */ int unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x52);
    /* 52 */ u16 unk52;
    /* 54 */ STRUCT_PAD(0x54, 0x60);
    /* 60 */ int unk60;
    /* 64 */ STRUCT_PAD(0x64, 0x66);
    /* 66 */ s16 unk66;
};
/* A four-leaf sprite placer for one actor, driven entirely by struct fields --
 * zero globals and zero pool words other than agbcc's own 0x1FF constants.
 * The parameter is parked in r7 for the whole body and the four member
 * ADDRESSES agbcc CSEs out of it (+0x64 in sl, +0x68 at [sp,#4], +0x4c in r8,
 * +0x66 in sb) are what buys the high-register prologue; there is no loop.
 *
 * Leaf structure, from the top-level tests in order:
 *   unk4e == 0                       -> the "settled" leaf, unk2c/unk30 only
 *   unk4e != 0, unk64 <= 0           -> the "entering" leaf
 *   unk4e != 0, unk64 > 0, unk68 > 0 -> the "leaving" leaf
 *   unk4e != 0, unk64 > 0, unk68 <=0 -> the "held" leaf, which is the only one
 *                                       that clears unk64/unk4e/unk68
 * Every leaf ends in the same pair of DrawOamObject calls: argument 2 is an
 * x | flags word masked to 9 bits, argument 3 a y masked to 8, and the second
 * call always ORs 0x3000 into the x word. agbcc builds that constant as
 * `movs r0,#0xc0; lsls r0,r0,#6`, which is just how it makes an even constant
 * above 255 -- nothing in the source distinguishes it from the literal.
 *
 * The `if (DivRem(unk58, 0x20) <= 0xf)` split appears FOUR times and each arm
 * spells out its statements in full. That really is duplication in the source,
 * not a lifted `kind` variable: the two arms differ in the inner Interpolate's
 * (kind, from, to) triple and the ROM materialises all three as immediates in
 * each arm, which a variable could not do. What looks like sharing --
 * _08084090, _080843C2, _0808447E, _080842CE -- is agbcc cross-jumping
 * identical tails, and the merge points land mid-statement (right on a `bl`),
 * where no source boundary could put them.
 *
 * The struct overlaps src/decomp/c_080831FC.c's on +0x4c and +0x4e, both s16
 * and both read with `ldrsh` in both places, and nothing here contradicts that
 * file's +0x34/+0x52/+0x60. That is consistent with one shared type but is NOT
 * evidence for it -- the two field sets are disjoint apart from those two, so
 * there is no discriminating use either way, and per the convention the
 * pointer-parameter type stays local to this file until one turns up.
 */

struct Unk8083EE0
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x3c);
    /* 3c */ int unk3c;
    /* 40 */ int unk40;
    /* 44 */ STRUCT_PAD(0x44, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x58);
    /* 58 */ int unk58;
    /* 5c */ STRUCT_PAD(0x5c, 0x64);
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
    /* 68 */ s16 unk68;
};

void MainMenuCarouselWheel_DrawSubmenuOptions(struct Unk8083A44 *p)
{
    int i;

    for (i = 0; i <= 4; i++)
    {
        if (i == 2)
            PutSprite(2, gUnknown_08616972[i + 1] & 0x1FF, gUnknown_08616980[i + 1] - 8,
                      gUnknown_08615C76,
                      ((gUnknown_0861696C[DivRem(p->unk52 + i, 6)] + 2) << 12) | 0x998);
        else
            PutSprite(2, gUnknown_08616972[i + 1], gUnknown_08616980[i + 1],
                      gUnknown_08615C84,
                      ((gUnknown_0861696C[DivRem(p->unk52 + i, 6)] + 2) << 12) | 0x800
                          | (gUnknown_0861696C[DivRem(p->unk52 + i, 6)] * 32 + 0x1D8));
    }

    if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0)
    {
        for (i = 0; i <= 1; i++)
        {
            if (i == 0 && GetCampaignSaveFlag() == 0)
                PutSprite(3, 0x90, 0x48, gUnknown_08615C04, (i * 12 + 0x298) | 0xA800);
            else if (i == DivRem(p->unk66, 2))
                PutSprite(3, 0x90, i * 16 + 0x48, gUnknown_08615C04, (i * 12 + 0x298) | 0x8800);
            else
                PutSprite(3, 0x90, i * 16 + 0x48, gUnknown_08615C04, (i * 12 + 0x298) | 0x2800);
        }
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 1)
    {
        for (i = 0; i <= 1; i++)
        {
            if (i == 0 && GetVersusSaveFlag() == 0)
                PutSprite(3, 0x90, 0x48, gUnknown_08615C04, (i * 12 + 0x298) | 0xB800);
            else if (i == DivRem(p->unk66, 2))
                PutSprite(3, 0x90, i * 16 + 0x48, gUnknown_08615C04, (i * 12 + 0x298) | 0x8800);
            else
                PutSprite(3, 0x90, i * 16 + 0x48, gUnknown_08615C04, (i * 12 + 0x298) | 0x3800);
        }
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 5)
    {
        for (i = 0; i <= 1; i++)
        {
            if (i == 0 && GetWarRoomSaveFlag() == 0)
                PutSprite(3, 0x90, 0x48, gUnknown_08615C04, (i * 12 + 0x298) | 0xC800);
            else if (i == DivRem(p->unk66, 2))
                PutSprite(3, 0x90, i * 16 + 0x48, gUnknown_08615C04, (i * 12 + 0x298) | 0x8800);
            else
                PutSprite(3, 0x90, i * 16 + 0x48, gUnknown_08615C04, (i * 12 + 0x298) | 0x7800);
        }
    }
    else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 3)
    {
        for (i = 0; i <= 1; i++)
        {
            if (i == DivRem(p->unk66, 2))
                PutSprite(3, 0x90, i * 16 + 0x48, gUnknown_08615C04, (i * 12 + 0x2B0) | 0x8800);
            else
                PutSprite(3, 0x90, i * 16 + 0x48, gUnknown_08615C04, (i * 12 + 0x2B0) | 0x5800);
        }
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
asm(".global sub_08083A44\n.thumb_set sub_08083A44, MainMenuCarouselWheel_DrawSubmenuOptions\n");

void MainMenuCarouselWheel_DrawCursor(struct Unk8083EE0 *p)
{
    if (p->unk4e != 0)
    {
        if (p->unk64 > 0)
        {
            if (p->unk68 > 0)
            {
                if (p->unk4c <= 7)
                {
                    if (DivRem(p->unk58, 0x20) <= 0xF)
                    {
                        p->unk2c = Interpolate(4, Interpolate(4, 0, 8, DivRem(p->unk58, 0x10), 0x10), 0x80, p->unk4c, 8);
                        p->unk30 = Interpolate(4, 0x64 - Interpolate(4, 0, 4, DivRem(p->unk58, 0x10), 0x10), DivRem(p->unk66, 2) * 16 + 0x54, p->unk4c, 8);
                        p->unk3c = Interpolate(4, 0xA0 - Interpolate(4, 0, 8, DivRem(p->unk58, 0x10), 0x10), 0xD0, p->unk4c, 8);
                        p->unk40 = Interpolate(4, Interpolate(4, 0, 4, DivRem(p->unk58, 0x10), 0x10) + 0x3C, DivRem(p->unk66, 2) * 16 + 0x3C, p->unk4c, 8);
                    }
                    else
                    {
                        p->unk2c = Interpolate(4, Interpolate(1, 8, 0, DivRem(p->unk58, 0x10), 0x10), 0x80, p->unk4c, 8);
                        p->unk30 = Interpolate(4, 0x64 - Interpolate(1, 4, 0, DivRem(p->unk58, 0x10), 0x10), DivRem(p->unk66, 2) * 16 + 0x54, p->unk4c, 8);
                        p->unk3c = Interpolate(4, 0xA0 - Interpolate(1, 8, 0, DivRem(p->unk58, 0x10), 0x10), 0xD0, p->unk4c, 8);
                        p->unk40 = Interpolate(4, Interpolate(1, 4, 0, DivRem(p->unk58, 0x10), 0x10) + 0x3C, DivRem(p->unk66, 2) * 16 + 0x3C, p->unk4c, 8);
                    }
                }
                else
                {
                    p->unk2c = 0x80;
                    p->unk30 = DivRem(p->unk66, 2) * 16 + 0x54;
                    p->unk3c = 0xD0;
                    p->unk40 = DivRem(p->unk66, 2) * 16 + 0x3C;
                }
            }
            else
            {
                if (p->unk4c <= 7)
                {
                    if (DivRem(p->unk58, 0x20) <= 0xF)
                    {
                        p->unk2c = Interpolate(4, Interpolate(4, 0, 8, DivRem(p->unk58, 0x10), 0x10) + 0x80, 0, p->unk4c, 8);
                        p->unk30 = Interpolate(4, DivRem(p->unk66, 2) * 16 + 0x54 - Interpolate(4, 0, 4, DivRem(p->unk58, 0x10), 0x10), 0x64, p->unk4c, 8);
                        p->unk3c = Interpolate(4, 0xD0 - Interpolate(4, 0, 8, DivRem(p->unk58, 0x10), 0x10), 0xA0, p->unk4c, 8);
                        p->unk40 = Interpolate(4, DivRem(p->unk66, 2) * 16 + 0x3C + Interpolate(4, 0, 4, DivRem(p->unk58, 0x10), 0x10), 0x3C, p->unk4c, 8);
                    }
                    else
                    {
                        p->unk2c = Interpolate(4, Interpolate(1, 8, 0, DivRem(p->unk58, 0x10), 0x10) + 0x80, 0, p->unk4c, 8);
                        p->unk30 = Interpolate(4, DivRem(p->unk66, 2) * 16 + 0x54 - Interpolate(1, 4, 0, DivRem(p->unk58, 0x10), 0x10), 0x64, p->unk4c, 8);
                        p->unk3c = Interpolate(4, 0xD0 - Interpolate(1, 8, 0, DivRem(p->unk58, 0x10), 0x10), 0xA0, p->unk4c, 8);
                        p->unk40 = Interpolate(4, DivRem(p->unk66, 2) * 16 + 0x3C + Interpolate(1, 4, 0, DivRem(p->unk58, 0x10), 0x10), 0x3C, p->unk4c, 8);
                    }
                }
                else
                {
                    p->unk2c = 0;
                    p->unk30 = 0x64;
                    p->unk3c = 0xA0;
                    p->unk40 = 0x3C;
                }

                if (p->unk4c == 8)
                    p->unk64 = 0;
            }

            DrawOamObject(0x50, p->unk2c & 0x1FF, p->unk30 & 0xFF, 0, 0);
            DrawOamObject(0x50, (p->unk3c & 0x1FF) | 0x3000, p->unk40 & 0xFF, 0, 0);

            if (p->unk4c == 8)
            {
                p->unk4e = 0;
                p->unk68 = 0;
            }
        }
        else
        {
            if (p->unk4c <= 7)
            {
                if (DivRem(p->unk58, 0x20) <= 0xF)
                {
                    p->unk2c = Interpolate(4, Interpolate(4, 0, 8, DivRem(p->unk58, 0x10), 0x10), 0, p->unk4c, 8);
                    p->unk30 = Interpolate(4, Interpolate(4, 0, 4, DivRem(p->unk58, 0x10), 0x10), 0, p->unk4c, 8);
                }
                else
                {
                    p->unk2c = Interpolate(4, Interpolate(1, 8, 0, DivRem(p->unk58, 0x10), 0x10), 0, p->unk4c, 8);
                    p->unk30 = Interpolate(4, Interpolate(1, 4, 0, DivRem(p->unk58, 0x10), 0x10), 0, p->unk4c, 8);
                }
            }
            else
            {
                p->unk30 = 0;
                p->unk2c = 0;
            }

            DrawOamObject(0x50, p->unk2c & 0x1FF, (0x64 - p->unk30) & 0xFF, 0, 0);
            DrawOamObject(0x50, ((0xA0 - p->unk2c) & 0x1FF) | 0x3000, (p->unk30 + 0x3C) & 0xFF, 0, 0);
        }
    }
    else
    {
        if (DivRem(p->unk58, 0x20) <= 0xF)
        {
            p->unk2c = Interpolate(4, 0, 8, DivRem(p->unk58, 0x10), 0x10);
            p->unk30 = Interpolate(4, 0, 4, DivRem(p->unk58, 0x10), 0x10);
        }
        else
        {
            p->unk2c = Interpolate(1, 8, 0, DivRem(p->unk58, 0x10), 0x10);
            p->unk30 = Interpolate(1, 4, 0, DivRem(p->unk58, 0x10), 0x10);
        }

        if (p->unk64 != 0)
        {
            DrawOamObject(0x50, (p->unk2c + 0x80) & 0x1FF, (DivRem(p->unk66, 2) * 16 + 0x54 - p->unk30) & 0xFF, 0, 0);
            DrawOamObject(0x50, ((0xD0 - p->unk2c) & 0x1FF) | 0x3000, (DivRem(p->unk66, 2) * 16 + 0x3C + p->unk30) & 0xFF, 0, 0);
        }
        else
        {
            DrawOamObject(0x50, p->unk2c & 0x1FF, (0x64 - p->unk30) & 0xFF, 0, 0);
            DrawOamObject(0x50, ((0xA0 - p->unk2c) & 0x1FF) | 0x3000, (p->unk30 + 0x3C) & 0xFF, 0, 0);
        }
    }
}
asm(".global sub_08083EE0\n.thumb_set sub_08083EE0, MainMenuCarouselWheel_DrawCursor\n");
