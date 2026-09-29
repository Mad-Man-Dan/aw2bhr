#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027308.
 * sub_08027308 @ 0x08027308
 */

#include "hardware.h"
#include "proc.h"
struct Unk27308Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x10];
    /* 0x44 */ s16 unk44;
    /* 0x46 */ u8 filler_46[0x0a];
    /* 0x50 */ struct Unk0801C210 *unk50;
    /* 0x54 */ u8 filler_54[0x10];
    /* 0x64 */ s16 unk64;
};

void sub_08027308(struct Unk27308Proc *proc)
{
    int v;

    SetObjAffine(0,
                 Div(COS_Q12(0) * 16, proc->unk64 != 0 ? proc->unk64 : 2),
                 Div(-SIN_Q12(0) * 16, proc->unk64 != 0 ? proc->unk64 : 2),
                 Div(SIN_Q12(0) * 16, proc->unk64 != 0 ? proc->unk64 : 2),
                 Div(COS_Q12(0) * 16, proc->unk64 != 0 ? proc->unk64 : 2));

    AP_Update(proc->unk50, proc->unk2c & 0x1ff, (proc->unk30 & 0xff) | 0x300);

    if (proc->unk44 > 0)
    {
        proc->unk44--;
        if (proc->unk44 == 0)
            Proc_Break(proc);
    }
    else
    {
        v = proc->unk64;
        v -= (v - 0x100) / 2;
        if (v - 0x100 == 4)
        {
            v = 0x100;
            proc->unk44 = 0x3c;
        }
        proc->unk64 = v;
    }
}
