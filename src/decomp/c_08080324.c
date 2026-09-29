#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080324.
 * sub_08080324 @ 0x08080324
 */

#include "proc.h"
struct Unk08080324Proc
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
};

void SuperCoPowerScene_BgShakeLoop(struct Unk08080324Proc *proc)
{
    if (proc->unk4c <= 3)
    {
        gUnknown_03001FF8 = Interpolate(0, 0, 8, proc->unk4c, 4);
        gUnknown_030030A0 = Interpolate(0, 0, -8, proc->unk4c, 4);
    }
    else if (proc->unk4c <= 7)
    {
        gUnknown_03001FF8 = Interpolate(0, 8, 0, proc->unk4c - 4, 4);
        gUnknown_030030A0 = Interpolate(0, -8, 0, proc->unk4c - 4, 4);
    }
    else if (proc->unk4c <= 9)
    {
        gUnknown_03001FF8 = Interpolate(0, 0, 4, proc->unk4c - 8, 2);
        gUnknown_030030A0 = Interpolate(0, 0, -4, proc->unk4c - 8, 2);
    }
    else if (proc->unk4c <= 0xB)
    {
        gUnknown_03001FF8 = Interpolate(0, 4, 0, proc->unk4c - 0xA, 2);
        gUnknown_030030A0 = Interpolate(0, -4, 0, proc->unk4c - 0xA, 2);
    }
    else if (proc->unk4c <= 0xC)
    {
        gUnknown_03001FF8 = Interpolate(0, 0, 2, proc->unk4c - 0xC, 1);
        gUnknown_030030A0 = Interpolate(0, 0, -2, proc->unk4c - 0xC, 1);
    }
    else if (proc->unk4c <= 0xE)
    {
        gUnknown_03001FF8 = Interpolate(0, 2, 0, proc->unk4c - 0xD, 1);
        gUnknown_030030A0 = Interpolate(0, -2, 0, proc->unk4c - 0xD, 1);
    }
    else
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }

    proc->unk4c++;
}
asm(".global sub_08080324\n.thumb_set sub_08080324, SuperCoPowerScene_BgShakeLoop\n");
