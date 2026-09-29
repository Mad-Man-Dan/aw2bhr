#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032DA0.
 * sub_08032DA0 @ 0x08032DA0
 */

#include "proc.h"
#include "hardware.h"
struct Unk32DA0Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void LinkConfirmDialog_Open(struct Unk32DA0Proc *proc)
{
    int t = Interpolate(4, 0x10, 0x100, proc->unk2c, 0x10);

    SetObjAffine(0,
        Div(COS_Q12(0) * 16, 0x100),
        Div(-SIN_Q12(0) * 16, t),
        Div(SIN_Q12(0) * 16, 0x100),
        Div(COS_Q12(0) * 16, t));

    PutSpriteExt(0, 0x78, 0x140, gUnknown_0849B6C8, 0);
    PutSpriteExt(0, 0x70, 0x160, gUnknown_0849B6D6, 0);
    PutSpriteExt(0, 0xa0, 0x160, gUnknown_0849B6D6, 8);

    if (proc->unk2c > 0xf)
    {
        proc->unk2c = 0;
        Proc_Break(proc);
    }
    else
    {
        proc->unk2c++;
    }
}
asm(".global sub_08032DA0\n.thumb_set sub_08032DA0, LinkConfirmDialog_Open\n");
