#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084F44.
 * sub_08084F44 @ 0x08084F44
 */

/* Slides the actor in over eight frames, unless gUnknown_03005940 says the
 * detail level is past 3, in which case it skips straight to the end state.
 * unk4c is the frame counter and unk4e the "still animating" flag; the two
 * halves of the animation are a kind-1 ease in over frames 0..3 and a kind-4
 * ease out over 4..7, which is why the second arm's `t` is `unk4c - 4`.
 *
 * `++p->unk4c == 8` really is a pre-increment on the s16 member and not a
 * separate read: the ROM does `ldrh; adds #1; strh` and only THEN sign-extends
 * for the compare, which is the increment's own value being reused rather than
 * a re-read.
 *
 * Same struct as MainMenuCarouselWheel_DrawCursor (+0x2c int, +0x4c s16, +0x4e s16, +0x66 s16) and
 * the same actor -- but, as there, that is field-set overlap and not a
 * discriminating use, so the type stays local to this file.
 */

struct Unk8084F44
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ STRUCT_PAD(0x30, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x66);
    /* 66 */ s16 unk66;
};

void CoInfoScreen_DrawCoSlide(struct Unk8084F44 *p)
{
    if (p->unk4e > 0)
    {
        if (gUnknown_03005940 <= 3)
        {
            if (p->unk4c <= 3)
                p->unk2c = Interpolate(1, 0, 0x90, p->unk4c, 4);
            else
                p->unk2c = Interpolate(4, 0x90, 0, p->unk4c - 4, 4);

            sub_08043C28(p->unk2c + 0xB0, 0xA0, 0x516C, 4, 1);

            if (p->unk4c == 5)
                LoadCoFullBodyAndPalette(gPlayers[p->unk66].co, 0xB6 * 2, 5);

            if (++p->unk4c == 8)
                p->unk4e = 0;
        }
        else
        {
            LoadCoFullBodyAndPalette(gPlayers[p->unk66].co, 0xB6 * 2, 5);
            p->unk4e = 0;
        }
    }
    else
    {
        if (gUnknown_03005940 <= 3)
            sub_08043C28(0xB0, 0xA0, 0x516C, 4, 1);
    }
}
asm(".global sub_08084F44\n.thumb_set sub_08084F44, CoInfoScreen_DrawCoSlide\n");
