#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807B574.
 * sub_0807B574 @ 0x0807B574
 */

#include "proc.h"
#include "hardware.h"
struct Unk0807B574Proc
{
    /* 0x00 */ u8 unk00[0x38];
    /* 0x38 */ int unk38;
    /* 0x3c */ u8 unk3c[0x1c];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};

void MatchSummaryPanel_CountPoints_Loop(struct Unk0807B574Proc * proc)
{
    int i;

    ApplyPaletteExt((u16 *)(DivRem(Div((u16)proc->unk38, 3), 0x10) * 2 + (int)gUnknown_0822AC60), 0x238, 2);
    proc->unk38++;

    PutSpriteNumberRightAligned(0x5b, 0x54, proc->unk58, 0);
    PutSpriteNumberRightAligned(0x5b, 0x67, proc->unk5c, 1);

    PutSprite(0, 0x14, 0x54, gUnknown_0848B690, 0x1058);
    PutSprite(0, 0xa, 0x67, gUnknown_0848B6C6, 0x105c);

    for (i = 0; i < 2; i++)
        PutSprite(0, 0x6c, 0x54 + i * 0x13, gUnknown_0848B6C6, 0x1050);

    sub_0807B738(proc);

    if (proc->unk60 != 0)
    {
        proc->unk60--;
    }
    else
    {
        if (proc->unk58 != 0)
        {
            proc->unk58--;
            proc->unk5c++;

            if (proc->unk58 != 0)
            {
                proc->unk58--;
                proc->unk5c++;
            }
        }
        else
        {
            Proc_Break(proc);
        }

        if (gpKeySt->pressed & 1)
        {
            proc->unk5c = proc->unk5c + proc->unk58;
            proc->unk58 = 0;
            Proc_Break(proc);
        }

        if (proc->unk5c > 0x270e)
            proc->unk5c = 0x270f;

        if (proc->unk38 & 1)
            PlayMusicOrSfx2(0x7c);
    }
}
asm(".global sub_0807B574\n.thumb_set sub_0807B574, MatchSummaryPanel_CountPoints_Loop\n");
