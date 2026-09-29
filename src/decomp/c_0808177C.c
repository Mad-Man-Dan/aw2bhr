#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808177C.
 * sub_0808177C @ 0x0808177C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* One of the block's sprite builders, and the closest relative of the
 * MainMenuCarouselWheel_DrawTileFlipIn / MainMenuCarouselWheel_DrawRotateWithFlip / MainMenuCarouselWheel_DrawRotate group in src/decomp/c_080824D4.c:
 * the same affine head and the same five-step PutSprite loop, but it owns its
 * own phase counter (+0x4c) rather than taking a step as a parameter, and it
 * ends the proc with Proc_Break when that counter runs past 0xB.
 *
 * The one structural fact worth recording is the SHAPE OF THE HEAD. The ROM has
 * a SINGLE `bl Interpolate` that both arms branch to, with only the argument
 * setup duplicated. That is NOT gcc cross-jumping the two arms of
 *     if (t <= 3) { unk38 = Interpolate(..); SetObjAffine(..); }
 *     else        { unk38 = Interpolate(..); SetObjAffine(..); }
 * the way MainMenuCarouselWheel_DrawTileFlipIn's head is -- that spelling was probed here and gcc
 * emitted BOTH affine tails in full (two gSinLut pool words, ~90 extra bytes).
 * The difference is that cross-jumping walks backwards from a jump and stops at
 * the first label, and this affine tail contains two of them (the `unk38 != 0 ?
 * unk38 : 2` branches), so it can never be merged. The head must therefore be
 * written with the SetObjAffine call OUTSIDE the if/else -- only the
 * Interpolate arms differ -- and the merge at the `bl` is then the two-insn
 * cross jump of `bl Interpolate; str r0, [p, #0x38]`.
 * Read backwards: a merge point that lands ON a call, with a tail containing
 * conditional branches, means the tail was written once in the source. */

struct Unk808177C
{
    /* 00 */ STRUCT_PAD(0x00, 0x38);
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x52);
    /* 52 */ u16 unk52;
};
void sub_08084700(struct Unk808177C *);

void MainMenuCarouselWheel_CentreTilePopLoop(struct Unk808177C *p)
{
    int i;

    if (p->unk4c <= 3)
        p->unk38 = Interpolate(1, 0x100, 8, p->unk4c, 4);
    else
        p->unk38 = Interpolate(4, 8, 0x100, p->unk4c - 4, 8);

    SetObjAffine(0,
                 Div(COS_Q12(0) * 16, 0x100),
                 Div(-SIN_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2),
                 Div(SIN_Q12(0) * 16, 0x100),
                 Div(COS_Q12(0) * 16, p->unk38 != 0 ? p->unk38 : 2));

    for (i = 0; i <= 4; i++)
    {
        if (i == 2)
        {
            if (p->unk4c <= 3)
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

    if (p->unk4c <= 0xB)
        p->unk4c = p->unk4c + 1;
    else
    {
        p->unk4c = 0;
        Proc_Break(p);
    }

    sub_08084700(p);
}

asm(".global sub_0808177C\n.thumb_set sub_0808177C, MainMenuCarouselWheel_CentreTilePopLoop\n");
