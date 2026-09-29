#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08079470.
 * sub_08079470 @ 0x08079470, sub_080794E8 @ 0x080794E8, sub_080795A8 @ 0x080795A8
 */

#include "proc.h"
#include "hardware.h"
struct Unk8079470
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
};
struct Unk80794E8
{
    /* 0x00 */ u8 filler_00[0x34];
    /* 0x34 */ int unk34;
    /* 0x38 */ u8 filler_38[0x14];
    /* 0x4c */ s16 unk4c;
};

void sub_08079470(struct Unk8079470 *proc)
{
    int i;

    for (i = 0; i < 4; i++)
        PutSprite(0, (i * 0x40 - 8) & 0x1FF, 0x30, gUnknown_0848B6A0,
                  0x4000 | (i * 0x40 + 0x140));

    proc->unk4c++;

    if (proc->unk4c > 0x21)
    {
        proc->unk4c = 0;
        PlayMusicOrSfx2(0x1D7);
        Proc_Break(proc);
    }
}

void sub_080794E8(struct Unk80794E8 *proc)
{
    int i;

    if (proc->unk4c > 0x13)
    {
        sub_080795A8(proc, 0);
    }
    else
    {
        proc->unk34 = Interpolate(4, 0x30, 0x14, proc->unk4c, 0x14);

        for (i = 0; i < 4; i++)
            PutSprite(0, (i * 0x40 - 8) & 0x1FF, proc->unk34,
                      gUnknown_0848B6A0, 0x4000 | (i * 0x40 + 0x140));

        PutSprite(0, 0x32, 0x20 - proc->unk34, gUnknown_08615BE4, 0x52D8);
    }

    proc->unk4c++;

    if (proc->unk4c > 0x2F)
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }
}

void sub_080795A8(ProcPtr proc, u32 a2)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        if (a2 <= 0x54)
            PutSprite(0, (i * 0x40 - 8) & 0x1FF, 0x14 - a2, gUnknown_0848B6A0,
                      0x4000 | (i * 0x40 + 0x140));
    }

    if (a2 <= 0x1C)
        PutSprite(0, 0x32, 0xC - a2, gUnknown_08615BE4, 0x52D8);
}
