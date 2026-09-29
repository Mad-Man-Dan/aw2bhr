#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A268.
 * sub_0807A268 @ 0x0807A268
 */

#include "proc.h"
#include "hardware.h"
struct Unk807A268
{
    /* 0x00 */ u8 filler_00[0x34];
    /* 0x34 */ int unk34;
    /* 0x38 */ u8 filler_38[0x04];
    /* 0x3c */ int unk3c;
    /* 0x40 */ u8 filler_40[0x0c];
    /* 0x4c */ s16 unk4c;
};

void ResultsVersus_SlideOutShowCo_Loop(struct Unk807A268 *proc)
{
    int i;

    proc->unk34 = Interpolate(4, 0, 0xF0, proc->unk4c, 0x30);

    if (proc->unk34 <= 0x70)
    {
        for (i = 0; i < 4; i++)
            PutSprite(0, (i * 0x40 - 8) & 0x1FF, (0x30 - proc->unk34) & 0xFF,
                      gUnknown_0848B6A0, 0x4000 | (i * 0x40 + 0x140));
    }

    gUnknown_03001400 = proc->unk34 - 0xF0;

    if ((s16)gUnknown_03001400 > -0x30)
        gDispIo.disp_ct.bg2_enable = 1;

    if (proc->unk4c > 0x1B)
    {
        if (proc->unk4c <= 0x2E)
            proc->unk3c = Interpolate(4, -0x40, 0x40, proc->unk4c - 0x1E, 0x10);

        sub_08043C28(proc->unk3c & 0x1FF, 0xA0, 0xB000, 0, 0);
    }

    if (proc->unk4c > 0x2F)
    {
        ResultsScreen_ShowVictoryQuote();
        Proc_Break(proc);
    }

    proc->unk4c++;
}
asm(".global sub_0807A268\n.thumb_set sub_0807A268, ResultsVersus_SlideOutShowCo_Loop\n");
