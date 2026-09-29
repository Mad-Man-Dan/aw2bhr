#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080655B0.
 * sub_080655B0 @ 0x080655B0
 */

/* sub_080655B0 @ 0x080655B0 */

struct Unk655B0Proc
{
    /* 0x00 */ u8 filler_00[0x24];
    /* 0x24 */ int unk24;
    /* 0x28 */ int unk28;
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x0c];
    /* 0x3c */ void *unk3c;
    /* 0x40 */ u8 filler_40[0x04];
    /* 0x44 */ s16 unk44;
};

void sub_080655B0(struct Unk655B0Proc *proc)
{
    int v;

    if (proc->unk2c != gUnknown_08580934->unk30)
    {
        if (gUnknown_08580934->unk30 == 0)
            proc->unk3c = gUnknown_08580C4E;
        else
            proc->unk3c = gUnknown_08580C40;

        proc->unk44 = 14;
    }

    if (gUnknown_08580934->unk30 == 0)
        v = gUnknown_08580C5C[proc->unk44];
    else
        v = 0xc0 - gUnknown_08580C5C[proc->unk44];

    proc->unk24 = v;
    proc->unk28 = (0xc0 - v) / 16 + 20;
    PutOamHi(v, proc->unk28, proc->unk3c, 0);

    proc->unk2c = gUnknown_08580934->unk30;

    if (proc->unk44 != 0)
        proc->unk44--;
}
