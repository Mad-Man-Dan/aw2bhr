#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080824D4.
 * sub_080824D4 @ 0x080824D4, sub_08082660 @ 0x08082660, sub_080829B0 @ 0x080829B0, sub_08082C0C @ 0x08082C0C, sub_08083034 @ 0x08083034
 */

#include "hardware.h"

/* Wave 20 integration note. These five functions were drafted as five separate
 * units and each named its own struct tag for the SAME object -- the file
 * convention puts a pointer-parameter's type in the .c that uses it, and that
 * convention has no answer when promote.py merges several drafts into one
 * linker unit. The forward declarations below were each written to suit their
 * CALLER's tag and so disagreed with their own definitions; the split build is
 * the only thing that sees this. Fixed by making every declaration agree with
 * its definition and casting at the call sites: all five tags describe one
 * object, a pointer cast emits nothing, and all five functions were re-verified
 * byte-for-byte with trymatch afterwards. */
struct Unk80824D4;
struct Unk8082660;
struct Unk80831FC;
struct Unk8082C0C;
struct Unk8083034;

/* The block's second dispatcher, and the one that closes the
 * 0x081D93B0-0x081D93D4 pool run: with MainMenuCarouselWheel_DrawRotation matched, every one of the
 * six functions that own a slot in that run is now C. It picks between
 * MainMenuCarouselWheel_DrawRotation, sub_08083A44 and a bare sub_08083738 on three s16 fields, draws
 * sub_08083A44's own five-step sprite loop when it takes the third path (with
 * the i == 2 arm's OAM word collapsed to the literal 0x8998 and no DivRem at
 * all), then unconditionally runs sub_08043C28 and sub_08083EE0 and finally
 * mirrors (unk52, unk64, unk66) into the three bytes at gUnknown_0300591C,
 * calling sub_08084600 only when they changed.
 *
 * The three-way compare is the useful part for typing: gUnknown_0300591C[0] is
 * compared against a `ldrh` of +0x52 and [1] and [2] against `ldrsh` of +0x64
 * and +0x66, so the phase counter is unsigned and the other two are signed --
 * and the write-back of all three is `ldrh ; strb`, which is byte-identical for
 * either signedness because the truncation discards the extension. Only the
 * COMPARISONS discriminate, which is the usual asymmetry.
 * Promotion must carry rodata: ["0x081D93B0", "0x081D93B4"], the -fforce-addr
 * copies of &gUnknown_08616980 and &gUnknown_0300591C. */

struct Unk80824D4
{
    /* 00 */ STRUCT_PAD(0x00, 0x4e);
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x52);
    /* 52 */ u16 unk52;
    /* 54 */ STRUCT_PAD(0x54, 0x64);
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
    /* 68 */ s16 unk68;
};
void MainMenuCarouselWheel_DrawRotation(struct Unk8082660 *);
void sub_08083738(struct Unk80824D4 *);
void sub_08083A44(struct Unk80824D4 *);
void sub_08083EE0(struct Unk80824D4 *);
void sub_08084600(struct Unk80824D4 *);
/* The first of the block's two dispatchers: a five-arm state machine on the s16
 * at +0x4c that runs one of the four sprite builders per arm, reads the D-pad
 * out of gpKeySt->repeated to set a +1/-1 direction at +0x5c, and steps
 * the phase counter at +0x52 around its period of six. Its own sprite loop is
 * MainMenuCarouselWheel_DrawTileFlipIn's with the phase expression `p->unk52 + i + p->unk4e` and six
 * iterations instead of five; see work/MainMenuCarouselWheel_DrawTileFlipIn/ for the OAM-word spelling.
 *
 * ONE spelling here was worth two instructions and it is a control-flow tell,
 * not a codegen one: the two `p->unk5c` arms are
 *     if (p->unk5c < 0 && p->unk4c == 0xE) ... else if (p->unk5c > 0 && ...)
 * and NOT the nested `if (p->unk5c < 0) { if (p->unk4c == 0xE) ... }`. Both are
 * semantically identical because the second test cannot be reached with
 * unk5c < 0, but they differ in where the inner `bne` lands: with `&&` it lands
 * on the NEXT arm's test, which is what lets gcc keep the single `ldr r1,
 * [p, #0x5c]` live across the merge instead of reloading it. Read the branch
 * TARGET, not the branch condition -- an inner test that jumps to a sibling arm
 * rather than past it is an `&&`.
 * Promotion must carry rodata: ["0x081D93B8"], the -fforce-addr copy of
 * &gpKeySt. */

struct Unk8082660
{
    /* 00 */ STRUCT_PAD(0x00, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x52);
    /* 52 */ u16 unk52;
    /* 54 */ STRUCT_PAD(0x54, 0x5c);
    /* 5c */ int unk5c;
};
void MainMenuCarouselWheel_DrawTileFlipIn(struct Unk80831FC *, int);
void MainMenuCarouselWheel_DrawRotateWithFlip(struct Unk8082C0C *, int);
void MainMenuCarouselWheel_DrawRotate(struct Unk8083034 *, int);
void sub_08083484(struct Unk8082660 *, int);
/* First of the four sprite builders at 0x080829B0-0x08083A44, the block whose
 * tail (MainMenuCarouselWheel_DrawLabelPlateSlideIn / sub_08083484 / sub_08083738) is already promoted in
 * src/decomp/c_080831FC.c. Everything here is that file's vocabulary plus a
 * loop: the affine head is the c_08032E88.c / c_08027B68.c idiom with `* 16`
 * and not `<< 4`, and the five-step loop draws gUnknown_08616972 /
 * gUnknown_08616980 as (x, y) pairs with the phase permutation gUnknown_0861696C
 * supplying the OBJ palette. The two `bl DivRem` per iteration are one source
 * expression written twice -- a call is not CSE-able -- exactly as in the
 * exemplars.
 *
 * Four things here are NOT free and each is recorded in docs/agbcc-codegen.md
 * (wave 20, W20-A):
 *   - The OAM word must be spelled `A | 0x800 | B`, constant in the MIDDLE.
 *     `A | (B | 0x800)` reassociates and `A | B | 0x800` puts it last.
 *   - `gUnknown_08616980[i + 1] | 0x100` needs no cast: the C front end shortens
 *     BIT_IOR to HImode, so an s16 element loads with `ldrh` and the result is
 *     sign-extended back. The sibling `(g[i + 1] - 8) | 0x100` in the other arm
 *     does NOT shorten (its left operand is an SImode MINUS) and keeps `ldrsh`
 *     with no re-extension. That pair is what PROVES both tables are s16.
 *   - `DivRem(p->unk52 + i, 6)` const-folds to `+ 2` in the i == 2 arm, and
 *     `[i + 1]` to `[3]`, because cse records i == 2 on the fall-through of
 *     `cmp r5, #2 / bne`. One source expression, two spellings in the ROM.
 *   - The two Interpolate arms are cross-jumped by gcc into one `bl` and one
 *     shared affine tail. That is the compiler, not the source: the sibling
 *     MainMenuCarouselWheel_DrawRotateWithFlip has the identical two-arm shape and is NOT cross-jumped, and
 *     writing both honestly reproduces each.
 * This function has NO .rodata pool word, unlike the three straight-line
 * exemplars, because its loop hoists &gUnknown_0861696C into one pseudo. */

struct Unk80831FC
{
    /* 00 */ STRUCT_PAD(0x00, 0x38);
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x52);
    /* 52 */ u16 unk52;
};
void MainMenuCarouselWheel_DrawLabelPlateSlideIn(struct Unk80831FC *, int);
/* Second of the block. Same affine head as MainMenuCarouselWheel_DrawTileFlipIn (two Interpolate arms,
 * here NOT cross-jumped -- see that file) and then two six-step loops selected
 * on the sign of the s16 at +0x4e, which is also the phase offset inside the
 * loop. The loops are the same body with different bounds and a different
 * special index: -1..4 with i == 1, and 0..5 with i == 2.
 *
 * Two spellings here are load-bearing and both are wave-20 rules in
 * docs/agbcc-codegen.md:
 *   - `gUnknown_08616972[i + 1 - p->unk4e]` is the ONLY spelling that emits the
 *     ROM's `sub r0, #1 ; sub r0, i, r0`. Writing the constant next to unk4e
 *     puts it on i and vice versa -- fold inverts it.
 *   - `p->unk52 + (k = i + 6)` binds i + 6 into one callee-saved pseudo shared
 *     by both DivRem calls. Every parenthesisation of the honest
 *     `p->unk52 + i + 6` folds the 6 onto the `ldrh` instead, which is the same
 *     instruction count but one fewer live value, and the function then comes
 *     out four instructions short with the parameter in a low register.
 * The `+ 6` is congruent to 0 mod 6 and exists only to keep the argument
 * non-negative when i starts at -1; the sibling loop, which starts at 0, does
 * not have it. */

struct Unk8082C0C
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x38);
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x4e);
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x52);
    /* 52 */ u16 unk52;
};
/* Third of the block, and the cheapest: MainMenuCarouselWheel_DrawRotateWithFlip's two loops with the affine
 * head and the special-index arm removed. It is the function the whole batch's
 * spellings were derived on, because at 456 bytes every probe is a second.
 * See work/MainMenuCarouselWheel_DrawRotateWithFlip/ for the two rules it shares (`i + 1 - p->unk4e`, and
 * the `(k = i + 6)` binding) and docs/agbcc-codegen.md for the measurements.
 * Note the asymmetry between the two loops is real and not a transcription
 * slip: the negative-unk4e loop uses `DivRem(p->unk52 + (i + 6), 6)` and the
 * other uses `DivRem(p->unk52 + i, 6)`. */

struct Unk8083034
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4e);
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x52);
    /* 52 */ u16 unk52;
};

void MainMenuCarouselWheel_DrawFrame(struct Unk80824D4 *p)
{
    int i;

    if (p->unk4e != 0 && p->unk64 == 0 && p->unk68 == 0)
    {
        MainMenuCarouselWheel_DrawRotation((struct Unk8082660 *)p);
    }
    else if (p->unk64 != 0)
    {
        sub_08083A44(p);
    }
    else
    {
        for (i = 0; i <= 4; i++)
        {
            if (i == 2)
                PutSprite(2, gUnknown_08616972[i + 1] & 0x1FF, gUnknown_08616980[i + 1] - 8,
                          gUnknown_08615C76, 0x8998);
            else
                PutSprite(2, gUnknown_08616972[i + 1], gUnknown_08616980[i + 1],
                          gUnknown_08615C84,
                          ((gUnknown_0861696C[DivRem(p->unk52 + i, 6)] + 2) << 12) | 0x800
                              | (gUnknown_0861696C[DivRem(p->unk52 + i, 6)] * 32 + 0x1D8));
        }

        sub_08083738(p);
    }

    sub_08043C28(0xB0, 0xA0, 0x1800, 4, 1);
    sub_08083EE0(p);

    if (p->unk4e == 0)
    {
        if (gUnknown_0300591C[0] != p->unk52 || gUnknown_0300591C[1] != p->unk64
            || gUnknown_0300591C[2] != p->unk66)
        {
            gUnknown_0300591C[0] = p->unk52;
            gUnknown_0300591C[1] = p->unk64;
            gUnknown_0300591C[2] = p->unk66;
            sub_08084600(p);
        }
    }
}
asm(".global sub_080824D4\n.thumb_set sub_080824D4, MainMenuCarouselWheel_DrawFrame\n");

void MainMenuCarouselWheel_DrawRotation(struct Unk8082660 *p)
{
    int i;

    if (p->unk4c <= 3)
    {
        sub_08083484(p, p->unk4c);

        for (i = 0; i <= 5; i++)
        {
            if (i == 2)
                PutSprite(2, gUnknown_08616972[i + 1], gUnknown_08616980[i + 1] - 8,
                          gUnknown_08615C76,
                          ((gUnknown_0861696C[DivRem(p->unk52 + i + p->unk4e, 6)] + 2) << 12)
                              | 0x998);
            else
                PutSprite(2, gUnknown_08616972[i + 1], gUnknown_08616980[i + 1],
                          gUnknown_08615C84,
                          ((gUnknown_0861696C[DivRem(p->unk52 + i + p->unk4e, 6)] + 2) << 12)
                              | 0x800
                              | (gUnknown_0861696C[DivRem(p->unk52 + i + p->unk4e, 6)] * 32
                                     + 0x1D8));
        }
    }
    else if (p->unk4c <= 0xD)
    {
        MainMenuCarouselWheel_DrawRotateWithFlip((struct Unk8082C0C *)p, p->unk4c - 4);

        if ((gpKeySt->repeated & DPAD_UP) && p->unk4c > 7 && p->unk4c <= 0xD)
            p->unk5c = -1;
        else if ((gpKeySt->repeated & DPAD_DOWN) && p->unk4c > 7 && p->unk4c <= 0xD)
            p->unk5c = 1;
    }
    else if (p->unk4c <= 0x17)
    {
        if (p->unk5c == 0 && p->unk4c == 0xE)
        {
            MainMenuCarouselWheel_DrawTileFlipIn((struct Unk80831FC *)p, p->unk4c - 0xE);
            p->unk4c += 9;
        }
        else
        {
            if (p->unk5c < 0 && p->unk4c == 0xE)
            {
                if (p->unk52 == 5)
                    p->unk52 = 0;
                else
                    p->unk52 = p->unk52 + 1;

                p->unk4e = p->unk5c;
            }
            else if (p->unk5c > 0 && p->unk4c == 0xE)
            {
                if (p->unk52 == 0)
                    p->unk52 = 5;
                else
                    p->unk52 = p->unk52 - 1;

                p->unk4e = p->unk5c;
            }

            MainMenuCarouselWheel_DrawRotate((struct Unk8083034 *)p, p->unk4c - 0xE);

            if (p->unk4c == 0xE)
            {
                p->unk5c = 0;
                PlayMusicOrSfx2(0x67);
                gUnknown_03005920 = 0;
            }

            if ((gpKeySt->repeated & DPAD_UP) && p->unk4c > 0xD && p->unk4c <= 0x17)
                p->unk5c = -1;
            else if ((gpKeySt->repeated & DPAD_DOWN) && p->unk4c > 0xD && p->unk4c <= 0x17)
                p->unk5c = 1;

            if (p->unk5c != 0 && p->unk4c == 0x17)
                p->unk4c -= 0xA;
        }
    }
    else if (p->unk4c <= 0x21)
    {
        MainMenuCarouselWheel_DrawTileFlipIn((struct Unk80831FC *)p, p->unk4c - 0x18);

        if ((gpKeySt->repeated & DPAD_UP) && p->unk4c > 0x17 && p->unk4c <= 0x1B)
        {
            p->unk5c = -1;
            p->unk4c = 0xD;
        }
        else if ((gpKeySt->repeated & DPAD_DOWN) && p->unk4c > 0x17 && p->unk4c <= 0x1B)
        {
            p->unk5c = 1;
            p->unk4c = 0xD;
        }
    }

    if (p->unk4c <= 3)
        gUnknown_03002020 = 3 - p->unk4c;
    else if (p->unk4c > 0x1D)
        gUnknown_03002020 = p->unk4c - 0x1D;
    else
        gUnknown_03002020 = 0;

    if (p->unk4c == 0x17)
    {
        LoadMainMenuCentreTileGraphic(gUnknown_0861696C[DivRem(p->unk52 + 2, 6)]);
        ApplyPaletteExt(GetMainMenuTilePalette(gUnknown_0861696C[DivRem(p->unk52 + 2, 6)]), 0x300, 0x20);
    }

    if (p->unk4c == 0x21)
        p->unk4e = 0;
}
asm(".global sub_08082660\n.thumb_set sub_08082660, MainMenuCarouselWheel_DrawRotation\n");

void MainMenuCarouselWheel_DrawTileFlipIn(struct Unk80831FC *p, int t)
{
    int i;

    if (t <= 1)
    {
        p->unk38 = Interpolate(1, 0x100, 8, t, 2);

        SetObjAffine(0,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2));
    }
    else if (t <= 5)
    {
        p->unk38 = Interpolate(4, 8, 0x100, t - 2, 4);

        SetObjAffine(0,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2));
    }
    else if (t <= 9)
    {
        SetObjAffine(0,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16, 0x100),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16, 0x100));

        MainMenuCarouselWheel_DrawLabelPlateSlideIn(p, t - 6);
    }

    for (i = 0; i <= 4; i++)
    {
        if (i == 2)
        {
            if (t <= 1)
                PutSpriteExt(2, gUnknown_08616972[i + 1] & 0x1FF,
                             gUnknown_08616980[i + 1] | 0x100, gUnknown_08615C84,
                             ((gUnknown_0861696C[DivRem(p->unk52 + i, 6)] + 2) << 12) | 0x800
                                 | (gUnknown_0861696C[DivRem(p->unk52 + i, 6)] * 32 + 0x1D8));
            else
                PutSpriteExt(2, gUnknown_08616972[i + 1] & 0x1FF,
                             (gUnknown_08616980[i + 1] - 8) | 0x100, gUnknown_08615C76,
                             ((gUnknown_0861696C[DivRem(p->unk52 + i, 6)] + 2) << 12) | 0x998);
        }
        else
        {
            PutSprite(2, gUnknown_08616972[i + 1], gUnknown_08616980[i + 1],
                      gUnknown_08615C84,
                      ((gUnknown_0861696C[DivRem(p->unk52 + i, 6)] + 2) << 12) | 0x800
                          | (gUnknown_0861696C[DivRem(p->unk52 + i, 6)] * 32 + 0x1D8));
        }
    }
}
asm(".global sub_080829B0\n.thumb_set sub_080829B0, MainMenuCarouselWheel_DrawTileFlipIn\n");

void MainMenuCarouselWheel_DrawRotateWithFlip(struct Unk8082C0C *p, int t)
{
    int i;
    int k;

    if (t <= 3)
    {
        p->unk38 = Interpolate(4, 0x100, 8, t, 4);

        SetObjAffine(0,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2));
    }
    else if (t <= 6)
    {
        p->unk38 = Interpolate(1, 8, 0x100, t - 4, 2);

        SetObjAffine(0,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2));
    }

    if (p->unk4e < 0)
    {
        for (i = -1; i <= 4; i++)
        {
            p->unk2c = Interpolate(4, gUnknown_08616972[i + 1 - p->unk4e],
                                   gUnknown_08616972[i + 1], t, 10);
            p->unk30 = Interpolate(4, gUnknown_08616980[i + 1 - p->unk4e],
                                   gUnknown_08616980[i + 1], t, 10);

            if (i == 1)
            {
                if (t <= 3)
                    PutSpriteExt(2, p->unk2c & 0x1FF, (p->unk30 - 8) | 0x100,
                                 gUnknown_08615C76,
                                 ((gUnknown_0861696C[DivRem(p->unk52 + (k = i + 6), 6)] + 2) << 12)
                                     | 0x998);
                else
                    PutSpriteExt(2, p->unk2c & 0x1FF, p->unk30 | 0x100, gUnknown_08615C84,
                                 ((gUnknown_0861696C[DivRem(p->unk52 + (k = i + 6), 6)] + 2) << 12)
                                     | 0x800
                                     | (gUnknown_0861696C[DivRem(p->unk52 + k, 6)] * 32 + 0x1D8));
            }
            else
            {
                PutSprite(2, p->unk2c & 0x1FF, p->unk30 & 0xFF, gUnknown_08615C84,
                          ((gUnknown_0861696C[DivRem(p->unk52 + (k = i + 6), 6)] + 2) << 12)
                              | 0x800
                              | (gUnknown_0861696C[DivRem(p->unk52 + k, 6)] * 32 + 0x1D8));
            }
        }
    }
    else
    {
        for (i = 0; i <= 5; i++)
        {
            p->unk2c = Interpolate(4, gUnknown_08616972[i + 1 - p->unk4e],
                                   gUnknown_08616972[i + 1], t, 10);
            p->unk30 = Interpolate(4, gUnknown_08616980[i + 1 - p->unk4e],
                                   gUnknown_08616980[i + 1], t, 10);

            if (i == 2)
            {
                if (t <= 3)
                    PutSpriteExt(2, p->unk2c & 0x1FF, (p->unk30 - 8) | 0x100,
                                 gUnknown_08615C76,
                                 ((gUnknown_0861696C[DivRem(p->unk52 + (k = i + 6), 6)] + 2) << 12)
                                     | 0x998);
                else
                    PutSpriteExt(2, p->unk2c & 0x1FF, p->unk30 | 0x100, gUnknown_08615C84,
                                 ((gUnknown_0861696C[DivRem(p->unk52 + (k = i + 6), 6)] + 2) << 12)
                                     | 0x800
                                     | (gUnknown_0861696C[DivRem(p->unk52 + k, 6)] * 32 + 0x1D8));
            }
            else
            {
                PutSprite(2, p->unk2c & 0x1FF, p->unk30 & 0xFF, gUnknown_08615C84,
                          ((gUnknown_0861696C[DivRem(p->unk52 + (k = i + 6), 6)] + 2) << 12)
                              | 0x800
                              | (gUnknown_0861696C[DivRem(p->unk52 + k, 6)] * 32 + 0x1D8));
            }
        }
    }
}
asm(".global sub_08082C0C\n.thumb_set sub_08082C0C, MainMenuCarouselWheel_DrawRotateWithFlip\n");

void MainMenuCarouselWheel_DrawRotate(struct Unk8083034 *p, int t)
{
    int i;
    int k;

    if (p->unk4e < 0)
    {
        for (i = -1; i <= 4; i++)
        {
            p->unk2c = Interpolate(4, gUnknown_08616972[i + 1 - p->unk4e],
                                   gUnknown_08616972[i + 1], t, 10);
            p->unk30 = Interpolate(4, gUnknown_08616980[i + 1 - p->unk4e],
                                   gUnknown_08616980[i + 1], t, 10);

            PutSprite(2, p->unk2c & 0x1FF, p->unk30 & 0xFF, gUnknown_08615C84,
                      ((gUnknown_0861696C[DivRem(p->unk52 + (k = i + 6), 6)] + 2) << 12) | 0x800
                          | (gUnknown_0861696C[DivRem(p->unk52 + k, 6)] * 32 + 0x1D8));
        }
    }
    else
    {
        for (i = 0; i <= 5; i++)
        {
            p->unk2c = Interpolate(4, gUnknown_08616972[i + 1 - p->unk4e],
                                   gUnknown_08616972[i + 1], t, 10);
            p->unk30 = Interpolate(4, gUnknown_08616980[i + 1 - p->unk4e],
                                   gUnknown_08616980[i + 1], t, 10);

            PutSprite(2, p->unk2c & 0x1FF, p->unk30 & 0xFF, gUnknown_08615C84,
                      ((gUnknown_0861696C[DivRem(p->unk52 + i, 6)] + 2) << 12) | 0x800
                          | (gUnknown_0861696C[DivRem(p->unk52 + i, 6)] * 32 + 0x1D8));
        }
    }
}
asm(".global sub_08083034\n.thumb_set sub_08083034, MainMenuCarouselWheel_DrawRotate\n");
