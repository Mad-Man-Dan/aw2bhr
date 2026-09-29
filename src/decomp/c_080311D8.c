#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080311D8.
 * sub_080311D8 @ 0x080311D8
 */

#include "proc.h"
#include "hardware.h"
struct Unk080311D8
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void LinkStartPrompt_Grow(struct Unk080311D8 *proc)
{
    s32 t;

    t = Interpolate(5, 0x10, 0x100, proc->unk58, 0x10);

    SetObjAffine(0xc,
        Div(COS_Q12(0) << 4, t),
        Div(-SIN_Q12(0) << 4, t),
        Div(SIN_Q12(0) << 4, t),
        Div(COS_Q12(0) << 4, t));

    DrawOamObject(0x4f, 0x188c, 0x170, 0, 0);

    if (proc->unk58 > 0xf)
        Proc_Break(proc);
    else
        proc->unk58++;
}
asm(".global sub_080311D8\n.thumb_set sub_080311D8, LinkStartPrompt_Grow\n");
