#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807567C.
 * sub_0807567C @ 0x0807567C
 */

#include "hardware.h"
#include "proc.h"
/* A proc body: three Interpolate curves off one (elapsed, total) pair at +0x3c /
 * +0x38, an affine spin whose angle is the ACCUMULATED +0x34 halfword, and a
 * blend coefficient pushed into the BLDALPHA shadows.  Unlike the 0x08083
 * builders the gSinLut angle here is a VARIABLE -- the `ands #0xff` in front of
 * every one of the four loads is COS_Q12/SIN_Q12's own mask, and agbcc keeps both
 * &gSinLut and the 0xff in high registers across the four Div calls.  The same
 * 0xff is then reused for the y word's `& 0xff`, which is why that mask is a
 * register move rather than a fresh immediate.
 *
 * +0x34 is a plain non-volatile u16 and the four reads prove it: the first is
 * forwarded from the `strh` that precedes it, and the other three re-`ldrh`
 * because each `bl Div` kills the memory.
 */

struct Unk807567CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u16 unk34;
    /* 36 */ STRUCT_PAD(0x36, 0x38);
    /* 38 */ int unk38;
    /* 3c */ int unk3c;
    /* 40 */ int unk40;
};

void WorldMapReticle_ShrinkLoop(struct Unk807567CProc *proc)
{
    int scale = Interpolate(5, 0x200, 0x100, proc->unk3c, proc->unk38);
    int step = Interpolate(0, 0xa, 1, proc->unk3c, proc->unk38);
    int coeff = Interpolate(0, 0, 6, proc->unk3c, proc->unk38);

    proc->unk34 += step;

    SetObjAffine(0xd,
                 Div(COS_Q12(proc->unk34) * 16, scale),
                 Div(-SIN_Q12(proc->unk34) * 16, scale),
                 Div(SIN_Q12(proc->unk34) * 16, scale),
                 Div(COS_Q12(proc->unk34) * 16, scale));

    PutSpriteExt(5, ((proc->unk2c - 0x20) & 0x1ff) | 0x1a00,
                 ((proc->unk30 - 0x20) & 0xff) | 0x700, gUnknown_086143D8, 0);

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = coeff;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = 0;

    if (proc->unk3c >= proc->unk38)
    {
        proc->unk40 = 1;
        Proc_Break(proc);
    }
    else
    {
        proc->unk3c++;
    }
}
asm(".global sub_0807567C\n.thumb_set sub_0807567C, WorldMapReticle_ShrinkLoop\n");
