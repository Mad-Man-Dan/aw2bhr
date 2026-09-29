#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807ED90.
 * sub_0807ED90 @ 0x0807ED90
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk807ED90
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ STRUCT_PAD(0x30, 0x4c);
    /* 4c */ s16 unk4c;
};

void CoSelect_IDLE_0807ED91(struct Unk807ED90 *proc)
{
    int i;
    int x;
    int y;
    int n;

    if (proc->unk4c <= 0x30)
    {
        proc->unk2c = Interpolate(1, 0xf8, 0, proc->unk4c, 0x30);

        if (proc->unk4c == 0x30)
            PlayMusicOrSfx2(0x78);
    }
    else if (proc->unk4c <= 0x34)
        proc->unk2c = Interpolate(0, 0, 0x10, proc->unk4c - 0x30, 4);
    else if (proc->unk4c <= 0x38)
        proc->unk2c = Interpolate(0, 0x10, 0, proc->unk4c - 0x34, 4);
    else if (proc->unk4c <= 0x3a)
        proc->unk2c = Interpolate(0, 0, 8, proc->unk4c - 0x38, 2);
    else if (proc->unk4c <= 0x3c)
        proc->unk2c = Interpolate(0, 8, 0, proc->unk4c - 0x3a, 2);
    else if (proc->unk4c <= 0x3d)
        proc->unk2c = Interpolate(0, 0, 4, proc->unk4c - 0x3c, 1);
    else if (proc->unk4c <= 0x3e)
        proc->unk2c = Interpolate(0, 4, 0, proc->unk4c - 0x3d, 1);

    for (i = 0; i < 2; i++)
    {
        y = i * 0x40;
        x = proc->unk2c + 0x38;
        PutSprite(0, (y + x) & 0x1ff, 0x30, gUnknown_0848B6A0,
                  0x4000 | (i * 0x40 + 0x280));
    }

    for (i = 0; i < sub_0803BD14(); i++)
    {
        n = sub_0803BD14() - 3;
        sub_08043B60(0xc0, (i - n) * 0x10 + 0x6f, 0x9000 | (i * 0xc + 0x3c0), 0);
    }

    if (proc->unk4c > 0x5f)
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }

    proc->unk4c++;
}

asm(".global sub_0807ED90\n.thumb_set sub_0807ED90, CoSelect_IDLE_0807ED91\n");
