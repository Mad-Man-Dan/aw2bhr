#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080BF0.
 * sub_08080BF0 @ 0x08080BF0
 */

#include "proc.h"
struct Unk8080BF0
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x58);
    /* 58 */ int unk58;
};

void SuperCoPowerName_ScatterOutLoop(struct Unk8080BF0 *proc)
{
    int i;

    if (proc->unk4c <= 0x10)
    {
        proc->unk2c = Interpolate(1, 0, 0x60, proc->unk4c, 0x10);
        proc->unk30 = Interpolate(1, 0x1e, 0, proc->unk4c, 0x10);
    }

    sub_08043C28(0xa8, 0xa0, 0x11ca, 0, 1);

    for (i = 0; i < proc->unk30; i++)
    {
        PutSpriteExt(0, i * 8, 0x440, (u16 *)gUnknown_0848B688, 0x3ba);
        PutSpriteExt(0, 0xe8 - i * 8, 0x458, (u16 *)gUnknown_0848B688, 0x3ba);
    }

    for (i = 0; i < proc->unk58; i++)
    {
        PutSprite(0,
                  gUnknown_030058D0 + gUnknown_030059A0[i]
                      + proc->unk2c * gUnknown_0861695C[DivRem(i, 4)],
                  proc->unk2c * gUnknown_08616964[DivRem(i, 4)] + 0x40,
                  gUnknown_0848B6E6, i * 8 + 0x30a);
    }

    if (proc->unk4c == 0x30)
        StartSuperCoPowerWhiteFade(proc);

    if (proc->unk4c == 0x40)
        Proc_Break(proc);

    proc->unk4c++;
}
asm(".global sub_08080BF0\n.thumb_set sub_08080BF0, SuperCoPowerName_ScatterOutLoop\n");
