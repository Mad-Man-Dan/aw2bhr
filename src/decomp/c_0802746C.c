#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802746C.
 * sub_0802746C @ 0x0802746C
 */

#include "hardware.h"
#include "proc.h"
struct Unk27428Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x44);
    /* 44 */ s16 unk44;
    /* 46 */ STRUCT_PAD(0x46, 0x50);
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
    /* 58 */ STRUCT_PAD(0x58, 0x64);
    /* 64 */ s16 unk64;
};

void SupplyIconEffect_Loop(struct Unk27428Proc *proc)
{
    int v;

    SetObjAffine(0,
                 Div(COS_Q12(0) * 16, proc->unk64 != 0 ? proc->unk64 : 2),
                 Div(-SIN_Q12(0) * 16, proc->unk64 != 0 ? proc->unk64 : 2),
                 Div(SIN_Q12(0) * 16, proc->unk64 != 0 ? proc->unk64 : 2),
                 Div(COS_Q12(0) * 16, proc->unk64 != 0 ? proc->unk64 : 2));

    AP_Update(proc->unk50, proc->unk2c & 0x1ff, (proc->unk30 & 0xff) | 0x300);

    v = proc->unk64;
    v = v - (v - 0x100) / 2;

    if (v - 0x100 == 4)
        v = 0x100;

    proc->unk64 = v;
}
asm(".global sub_0802746C\n.thumb_set sub_0802746C, SupplyIconEffect_Loop\n");
