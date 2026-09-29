#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080730D0.
 * sub_080730D0 @ 0x080730D0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk0D0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2a);
    /* 2a */ s16 unk2a[14];
    /* 46 */ u8 unk46[14];
    /* 54 */ u8 unk54;
    /* 55 */ STRUCT_PAD(0x55, 0x58);
    /* 58 */ int unk58;
    /* 5c */ u16 unk5c;
    /* 5e */ u8 unk5e;
    /* 5f */ STRUCT_PAD(0x5f, 0x64);
    /* 64 */ int unk64;
    /* 68 */ u8 unk68;
};

void HeaderBanner_FlyInLoop(struct Unk0D0Proc *proc)
{
    int i;
    s32 v;
    s32 scale;

    for (i = 0; i < proc->unk58; i++)
    {
        v = Interpolate(0, proc->unk2a[i], proc->unk46[i], proc->unk64, proc->unk68);
        scale = Interpolate(0, 0x10, 0x100, proc->unk64, proc->unk68);

        SetObjAffine(i,
            Div(COS_Q12(0) * 16, scale),
            Div(-SIN_Q12(0) * 16, 0x100),
            Div(SIN_Q12(0) * 16, scale),
            Div(COS_Q12(0) * 16, 0x100));

        PutSpriteExt(1,
            (v & 0x1FF) | (i << 9),
            proc->unk54 | 0x100,
            gUnknown_081CC01C,
            i * 8 + proc->unk5c + (proc->unk5e << 12));
    }

    proc->unk64++;

    if (proc->unk64 == proc->unk68)
    {
        for (i = 0; i < proc->unk58; i++)
            proc->unk2a[i] = proc->unk46[i];

        proc->unk68 = 0;
        Proc_Break(proc);
    }
}

asm(".global sub_080730D0\n.thumb_set sub_080730D0, HeaderBanner_FlyInLoop\n");
