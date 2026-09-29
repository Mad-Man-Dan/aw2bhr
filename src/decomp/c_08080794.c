#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080794.
 * sub_08080794 @ 0x08080794
 */

#include "proc.h"
struct Unk8080794
{
    /* 00 */ STRUCT_PAD(0x00, 0x34);
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x58);
    /* 58 */ int unk58;
};

void SuperCoPowerName_LettersFlyInLoop(struct Unk8080794 *proc)
{
    int i;

    sub_08043C28(0xa8, 0xa0, 0x11ca, 2, 1);

    for (i = 0; i < proc->unk58; i++)
    {
        if (proc->unk4c >= i * 8 && proc->unk4c < i * 8 + 0xc)
        {
            if (DivRem(i, 4) == 0)
            {
                proc->unk34 = Div(Interpolate(0, 0xf0, -0x20, proc->unk4c - i * 8, 0xc)
                                      + Interpolate(1, 0xf0, -0x20, proc->unk4c - i * 8, 0xc) * 2,
                                  3);
                proc->unk38 = 0x40;

                if (proc->unk34 < (int)(gUnknown_030058D0 + gUnknown_030059A0[i]))
                    PutSprite(0, gUnknown_030058D0 + gUnknown_030059A0[i], 0x40,
                              gUnknown_0848B6E6, i * 8 + 0x30a);
            }
            else if (DivRem(i, 4) == 1)
            {
                proc->unk34 = gUnknown_030058D0 + gUnknown_030059A0[i];
                proc->unk38 = Div(Interpolate(0, -0x20, 0xa0, proc->unk4c - i * 8, 0xc)
                                      + Interpolate(1, -0x20, 0xa0, proc->unk4c - i * 8, 0xc) * 2,
                                  3);

                if (proc->unk38 > 0x40)
                    PutSprite(0, gUnknown_030058D0 + gUnknown_030059A0[i], 0x40,
                              gUnknown_0848B6E6, i * 8 + 0x30a);
            }
            else if (DivRem(i, 4) == 2)
            {
                proc->unk34 = Div(Interpolate(0, -0x20, 0xf0, proc->unk4c - i * 8, 0xc)
                                      + Interpolate(0, -0x20, 0xf0, proc->unk4c - i * 8, 0xc) * 2,
                                  3);
                proc->unk38 = 0x40;

                if (proc->unk34 > (int)(gUnknown_030058D0 + gUnknown_030059A0[i]))
                    PutSprite(0, gUnknown_030058D0 + gUnknown_030059A0[i], 0x40,
                              gUnknown_0848B6E6, i * 8 + 0x30a);
            }
            else if (DivRem(i, 4) == 3)
            {
                proc->unk34 = gUnknown_030058D0 + gUnknown_030059A0[i];
                proc->unk38 = Div(Interpolate(0, 0xa0, -0x20, proc->unk4c - i * 8, 0xc)
                                      + Interpolate(0, 0xa0, -0x20, proc->unk4c - i * 8, 0xc) * 2,
                                  3);

                if (proc->unk38 < 0x40)
                    PutSprite(0, gUnknown_030058D0 + gUnknown_030059A0[i], 0x40,
                              gUnknown_0848B6E6, i * 8 + 0x30a);
            }

            PutSprite(0, proc->unk34, proc->unk38, gUnknown_0848B6E6, i * 8 + 0x30a);
        }
        else if (proc->unk4c >= i * 8 + 0xc)
        {
            if (proc->unk4c == i * 8 + 0xc)
                PlayMusicOrSfx2(0x1dd);

            PutSprite(0, gUnknown_030058D0 + gUnknown_030059A0[i], 0x40, gUnknown_0848B6E6,
                      i * 8 + 0x30a);
        }
    }

    if (proc->unk4c < proc->unk58 * 8 + 0xc)
    {
        proc->unk4c++;
    }
    else
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_08080794\n.thumb_set sub_08080794, SuperCoPowerName_LettersFlyInLoop\n");
