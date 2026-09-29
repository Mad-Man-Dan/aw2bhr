#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C380.
 * sub_0806C380 @ 0x0806C380, sub_0806C410 @ 0x0806C410
 */

#include "hardware.h"
#include "proc.h"
/* The 16-frame slide-in twin of CreditsRank_HoldLoop: same two sprites, but the second
 * one's x is interpolated instead of fixed, and the frame counter drives a
 * Proc_Break at the end.
 *
 * `(row.unk0c + 0x54) & 0xff | 0x400` is the OAM attribute-0 word: the row's
 * signed y offset biased, clipped to 8 bits, with the 256-colour bit set.
 * 0x400 is `movs r0,#0x80; lsls r0,#3` -- a literal agbcc rebuilds, not a
 * shift in the source. */
struct Unk0806C380Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
};
/* The settled-state twin of CreditsRank_SlideInLoop: the same two sprites with the second
 * one's x pinned at 0xa4 and no frame counter, so the second draw is PutSprite
 * rather than PutSpriteExt. See CreditsRank_SlideInLoop for the attribute-0 expression. */
struct Unk0806C410Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void CreditsRank_SlideInLoop(struct Unk0806C380Proc *proc)
{
    int a;

    a = Interpolate(5, 0xf0, 0xa4, proc->unk5c, 0x10);

    if (gUnknown_0202F2C0 != 0)
        CreditsRank_CycleBlinkPalette();

    PutSpriteExt(0, 0xa4, ((gUnknown_085816F0[proc->unk58].unk0c + 0x54) & 0xff) | 0x400,
                 gUnknown_085816F0[proc->unk58].unk08, 0x1090);
    PutSpriteExt(0, a & 0x1ff, 0x40, gUnknown_08581A98, 0xd0);

    if (proc->unk5c > 0xf)
        Proc_Break(proc);
    else
        proc->unk5c++;
}
asm(".global sub_0806C380\n.thumb_set sub_0806C380, CreditsRank_SlideInLoop\n");

void CreditsRank_HoldLoop(struct Unk0806C410Proc *proc)
{
    if (gUnknown_0202F2C0 != 0)
        CreditsRank_CycleBlinkPalette();

    PutSpriteExt(0, 0xa4, ((gUnknown_085816F0[proc->unk58].unk0c + 0x54) & 0xff) | 0x400,
                 gUnknown_085816F0[proc->unk58].unk08, 0x1090);
    PutSprite(0, 0xa4, 0x40, gUnknown_08581A98, 0xd0);
}
asm(".global sub_0806C410\n.thumb_set sub_0806C410, CreditsRank_HoldLoop\n");
