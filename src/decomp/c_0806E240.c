#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E240.
 * SoundRoomButton_Loop @ 0x0806E240
 */

#include "hardware.h"
/* The affine-sprite tick: four Div calls build the 2x2 matrix from a fixed
 * (angle 0) sin/cos pair scaled by 16 and divided by a scale factor, then the
 * sprite goes out at a fixed OAM slot. Four arms differing only in the divisor:
 * the requested scale while it is changing, 0x200 on the frame the pop starts,
 * an Interpolate ramp for the sixteen frames after that, and the resting scale.
 * The struct is local because it only ever describes this parameter. */

/* The affine-sprite tick: four Div calls build the 2x2 matrix from a fixed
 * (angle 0) sin/cos pair scaled by 16 and divided by a scale factor, then the
 * sprite goes out at a fixed OAM slot. Four arms differing only in the divisor:
 * the requested scale while it is changing, 0x200 on the frame the pop starts,
 * an Interpolate ramp for the sixteen frames after that, and the resting scale.
 * The struct is local because it only ever describes this parameter. */

struct Unk0806E240Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ u16 *unk38;
    /* 0x3c */ u16 unk3c;
    /* 0x3e */ u16 unk3e;
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u8 filler_42[0x02];
    /* 0x44 */ int unk44;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 filler_49[0x03];
    /* 0x4c */ int unk4c;
};

void SoundRoomButton_Loop(struct Unk0806E240Proc *proc)
{
    u16 scale;

    if (proc->unk3c != proc->unk3e)
    {
        SetObjAffine(proc->unk44,
                     Div(COS_Q12(0) * 16, proc->unk3c),
                     Div(-SIN_Q12(0) * 16, proc->unk3c),
                     Div(SIN_Q12(0) * 16, proc->unk3c),
                     Div(COS_Q12(0) * 16, proc->unk3c));
        PutSpriteExt(0, ((proc->unk2c - 0x10) & 0x1ff) | (proc->unk44 << 9),
                     ((proc->unk30 - proc->unk34) & 0xff) | 0x300,
                     proc->unk38, proc->unk40);
        proc->unk3e = proc->unk3c;
    }
    else if (proc->unk48 != 0)
    {
        proc->unk48 = 0;
        proc->unk4c = 0x10;
        SetObjAffine(proc->unk44,
                     Div(COS_Q12(0) * 16, 0x200),
                     Div(-SIN_Q12(0) * 16, 0x200),
                     Div(SIN_Q12(0) * 16, 0x200),
                     Div(COS_Q12(0) * 16, 0x200));
        PutSpriteExt(0, ((proc->unk2c - 0x10) & 0x1ff) | (proc->unk44 << 9),
                     ((proc->unk30 - proc->unk34) & 0xff) | 0x300,
                     proc->unk38, proc->unk40);
    }
    else if (proc->unk4c != 0)
    {
        proc->unk4c--;
        scale = Interpolate(4, 0x200, 0x100, 0x10 - proc->unk4c, 0x10);
        SetObjAffine(proc->unk44,
                     Div(COS_Q12(0) * 16, scale),
                     Div(-SIN_Q12(0) * 16, scale),
                     Div(SIN_Q12(0) * 16, scale),
                     Div(COS_Q12(0) * 16, scale));
        PutSpriteExt(0, ((proc->unk2c - 0x10) & 0x1ff) | (proc->unk44 << 9),
                     ((proc->unk30 - proc->unk34) & 0xff) | 0x300,
                     proc->unk38, proc->unk40);
    }
    else
    {
        SetObjAffine(proc->unk44,
                     Div(COS_Q12(0) * 16, proc->unk3c),
                     Div(-SIN_Q12(0) * 16, proc->unk3c),
                     Div(SIN_Q12(0) * 16, proc->unk3c),
                     Div(COS_Q12(0) * 16, proc->unk3c));
        PutSpriteExt(0, ((proc->unk2c - 0x10) & 0x1ff) | (proc->unk44 << 9),
                     ((proc->unk30 - proc->unk34) & 0xff) | 0x300,
                     proc->unk38, proc->unk40);
    }
}
asm(".global sub_0806E240\n.thumb_set sub_0806E240, SoundRoomButton_Loop\n");
