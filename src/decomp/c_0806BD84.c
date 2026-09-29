#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806BD84.
 * sub_0806BD84 @ 0x0806BD84, sub_0806BE7C @ 0x0806BE7C
 */

#include "hardware.h"
#include "proc.h"
/* One frame of a spinning-sprite intro: an Interpolate ramp drives the affine
 * scale, the four matrix terms are Div'd out of it, then the sprite is drawn
 * and the tile is committed on the last frame.
 *
 * The matrix is cos/-sin/sin/cos at angle 0, and the source really does index
 * gSinLut past its declared end for the cosine -- there is ONE pool word
 * (=gSinLut) and the 0x40 entry is reached as `adds r0, #0x80` off it, which a
 * separate &gCosLut would not do.
 *
 * +0x64 is s16: PutSpriteExt's fifth argument reads it with a bare `ldrsh` and
 * WriteBg0TilePair's u16 parameter reads the same offset with a bare `ldrh`, and
 * only an s16 member serves both without a shift pair. */
struct Unk6BD84Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x24];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 filler_60[0x04];
    /* 0x64 */ s16 unk64;
};
/* Spawns one CreditsNameLetter_Loop sprite proc every eighth frame, walking a u16 table
 * that starts at +0x2a, and ends itself once the index reaches +0x54. The five
 * members it writes into the child are exactly the five CreditsNameLetter_Loop reads,
 * which is how the child's layout is settled independently of that function.
 *
 * `proc->unk60 & 7` is still live in r5 when the child's +0x58 is zeroed, so
 * the zero store re-uses it rather than materialising a fresh 0. */
struct Unk6BE7CParent
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a[0x15];
    /* 0x54 */ int unk54;
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};
struct Unk6BE7CChild
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x24];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 filler_60[0x04];
    /* 0x64 */ s16 unk64;
};

void CreditsNameLetter_Loop(struct Unk6BD84Proc *proc)
{
    int scale;

    scale = Interpolate(0, 0x200, 0x100, proc->unk58, 6);

    SetObjAffine(proc->unk5c,
        Div(gSinLut[0x40] * 0x10, scale),
        Div(-gSinLut[0] * 0x10, scale),
        Div(gSinLut[0] * 0x10, scale),
        Div(gSinLut[0x40] * 0x10, scale));

    PutSpriteExt(1,
        ((proc->unk2c - 4) & 0x1ff) | (proc->unk5c << 9),
        ((proc->unk30 - 8) & 0xff) | 0x300,
        gUnknown_0816E174,
        proc->unk64);

    if (proc->unk58 > 5)
    {
        WriteBg0TilePair(gBG0TilemapBuffer + ((proc->unk30 >> 3) * 0x20 + (proc->unk2c >> 3)), proc->unk64);
        Proc_Break(proc);
    }
    else
    {
        proc->unk58++;
    }
}
asm(".global sub_0806BD84\n.thumb_set sub_0806BD84, CreditsNameLetter_Loop\n");

void CreditsNameLine_Loop(struct Unk6BE7CParent *proc)
{
    struct Unk6BE7CChild *child;

    if ((proc->unk60 & 7) == 0)
    {
        child = Proc_Start(gUnknown_08581A24, (ProcPtr)3);
        child->unk2c = proc->unk5c * 8 + 0x48;
        child->unk30 = proc->unk58;
        child->unk58 = 0;
        child->unk5c = proc->unk5c;
        child->unk64 = proc->unk2a[proc->unk5c];

        proc->unk5c++;

        if (proc->unk5c == proc->unk54)
            Proc_Break(proc);
    }

    proc->unk60++;
}
asm(".global sub_0806BE7C\n.thumb_set sub_0806BE7C, CreditsNameLine_Loop\n");
