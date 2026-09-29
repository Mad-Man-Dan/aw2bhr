#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028F84.
 * sub_08028F84 @ 0x08028F84
 */

struct Unk28F84Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
};

void CameraScroll_Loop(struct Unk28F84Proc *proc)
{
    int x;
    int y;
    int v;
    int c;
    int d;

    if ((proc->unk26 < 0 ? -proc->unk26 : proc->unk26)
        > (proc->unk28 < 0 ? -proc->unk28 : proc->unk28))
    {
        proc->unk2c += gUnknown_020237B0[proc->unk2a];

        c = proc->unk2c;
        v = proc->unk26 > 0 ? c : (proc->unk26 < 0 ? -c : 0);

        d = (proc->unk28 << 16) / proc->unk26;
        x = (proc->unk1e + v) << 16;
        y = (proc->unk20 << 16) + v * d;
    }
    else
    {
        proc->unk2c += gUnknown_020237B0[proc->unk2a];

        c = proc->unk2c;
        v = proc->unk28 > 0 ? c : (proc->unk28 < 0 ? -c : 0);

        d = (proc->unk26 << 16) / proc->unk28;
        x = (proc->unk1e << 16) + v * d;
        y = (proc->unk20 + v) << 16;
    }

    gUnknown_0201E450.unk04 = x / 0x10000;
    gUnknown_0201E450.unk06 = y / 0x10000;

    if (proc->unk2a == 0)
    {
        gUnknown_0201E450.unk04 = proc->unk22;
        gUnknown_0201E450.unk06 = proc->unk24;
        sub_080153B8((struct Unk03001470 *)proc);
        DecrementMapLock();
    }
    else
    {
        proc->unk2a--;
    }

    UpdateMapBgScroll();
}
asm(".global sub_08028F84\n.thumb_set sub_08028F84, CameraScroll_Loop\n");
