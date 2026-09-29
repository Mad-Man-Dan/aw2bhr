#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080362E0.
 * sub_080362E0 @ 0x080362E0, sub_08036384 @ 0x08036384
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

struct Unk080362E0
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ s8 unk38;
    /* 0x39 */ u8 filler_39[5];
    /* 0x3e */ u16 unk3e;
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u16 unk42;
    /* 0x44 */ u16 unk44;
};
#include "proc.h"
struct Unk36384Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x35);
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 filler_36[0x08];
    /* 0x3e */ u16 unk3e;
};

void MoveSlideState_Walk(struct Unk080362E0 *p)
{
    p->unk3e += p->unk40;
    p->unk42 += p->unk40 * (u16)gUnknown_0849BE10[p->unk38 * 2];
    p->unk44 += p->unk40 * (u16)gUnknown_0849BE10[p->unk38 * 2 + 1];

    if (p->unk3e > 0xf)
    {
        p->unk3e -= 0x10;
        p->unk42 -= p->unk3e * gUnknown_0849BE10[p->unk38 * 2];
        p->unk44 -= p->unk3e * gUnknown_0849BE10[p->unk38 * 2 + 1];
        p->unk3e = 0;
    }
}
asm(".global sub_080362E0\n.thumb_set sub_080362E0, MoveSlideState_Walk\n");

void MoveSlide_Loop(ProcPtr procArg)
{
    struct Unk36384Proc *proc = procArg;

    if (proc->unk35 != 0)
    {
        if ((u8)(proc->unk35 - 2) <= 1 && proc->unk3e == 0)
        {
            RunMoveSlideCommand(proc);

            if (proc->proc_script == NULL)
                return;
        }

        gUnknown_0849BE20[proc->unk35](proc);
    }

    sub_080359A4(proc);
}

asm(".global sub_08036384\n.thumb_set sub_08036384, MoveSlide_Loop\n");
