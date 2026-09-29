#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080681B8.
 * sub_080681B8 @ 0x080681B8, sub_080682E8 @ 0x080682E8, sub_080684E0 @ 0x080684E0
 */

#include "proc.h"
struct Unk681B8Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 filler_2b[0x01];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 filler_33[0x01];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ u8 unk38[0x14];
    /* 0x4c */ u8 unk4c;
    /* 0x4d */ u8 unk4d;
    /* 0x4e */ u8 unk4e;
    /* 0x4f */ u8 unk4f;
};
#include "hardware.h"
struct Unk682E8Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 filler_2b[0x01];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 filler_33[0x01];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ u8 unk38[0x14];
    /* 0x4c */ u8 unk4c;
    /* 0x4d */ u8 unk4d;
    /* 0x4e */ u8 unk4e;
    /* 0x4f */ u8 unk4f;
};
struct Unk684E0Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 filler_2b[0x01];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 filler_33[0x01];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ u8 unk38[0x14];
    /* 0x4c */ u8 unk4c;
    /* 0x4d */ u8 unk4d;
    /* 0x4e */ u8 unk4e;
    /* 0x4f */ u8 unk4f;
};

void IntroCoNameBanner_SlideIn(struct Unk681B8Proc *proc)
{
    int i;
    int x;
    u16 col;

    x = Interpolate(5, proc->unk34, proc->unk36, proc->unk4f, proc->unk4e);

    if (proc->unk4d == 0)
    {
        for (i = proc->unk2c - 1; i >= 0; i--)
        {
            if (x + i * 36 >= -0x20 && x + i * 36 <= 0xef)
            {
                PutSprite(0, x + i * 36, 0x40, gUnknown_08581208,
                          proc->unk30 + i % proc->unk2a * 16 + (proc->unk32 << 12));
            }
        }
    }
    else
    {
        for (i = 0; i < proc->unk2c; i++)
        {
            if (x + i * 36 >= -0x20 && x + i * 36 <= 0xef)
            {
                col = proc->unk2a - (proc->unk2c - i - 1) % proc->unk2a - 1;
                PutSprite(0, x + i * 36, 0x40, gUnknown_08581208,
                          proc->unk30 + col * 16 + (proc->unk32 << 12));
            }
        }
    }

    if (++proc->unk4f == proc->unk4e)
    {
        proc->unk4f = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_080681B8\n.thumb_set sub_080681B8, IntroCoNameBanner_SlideIn\n");

void IntroCoNameBanner_SquashLetters(struct Unk682E8Proc *proc)
{
    int i;
    int t;
    u16 col;

    t = Interpolate(0, 0x100, 1, proc->unk4f, 0x1a);

    for (i = proc->unk2c - 1; i >= 0; i--)
    {
        if (proc->unk36 + i * 36 >= -0x20 && proc->unk36 + i * 36 <= 0xef)
        {
            SetObjAffine(i, Div(COS_Q12(0) * 16, 0x100), Div(-SIN_Q12(0) * 16, 0x100),
                         Div(SIN_Q12(0) * 16, 0x100), Div(COS_Q12(0) * 16, 0x100));

            if (proc->unk38[i] == 0)
                SetObjAffine(i, Div(COS_Q12(0) * 16, 0x100), Div(-SIN_Q12(0) * 16, t),
                             Div(SIN_Q12(0) * 16, 0x100), Div(COS_Q12(0) * 16, t));

            if (proc->unk4d == 0)
            {
                PutSpriteExt(0, ((proc->unk36 + i * 36) & 0x1ff) | (i << 9), 0x140,
                             gUnknown_08581208,
                             proc->unk30 + i % proc->unk2a * 16 + (proc->unk32 << 12));
            }
            else
            {
                col = proc->unk2a - (proc->unk2c - i - 1) % proc->unk2a - 1;
                PutSpriteExt(0, ((proc->unk36 + i * 36) & 0x1ff) | (i << 9), 0x140,
                             gUnknown_08581208,
                             proc->unk30 + col * 16 + (proc->unk32 << 12));
            }
        }
    }

    if (++proc->unk4f == 0x1a)
    {
        proc->unk4f = 0;
        proc->unk36 -= 0x10;
        Proc_Break(proc);
    }
}
asm(".global sub_080682E8\n.thumb_set sub_080682E8, IntroCoNameBanner_SquashLetters\n");

void IntroCoNameBanner_ZoomLetters(struct Unk684E0Proc *proc)
{
    int i;
    int t;
    u16 col;

    t = Interpolate(0, 0x100, 0x200, proc->unk4f, 6);

    for (i = proc->unk2c - 1; i >= 0; i--)
    {
        if (proc->unk36 + i * 36 >= -0x20 && proc->unk36 + i * 36 <= 0xef)
        {
            SetObjAffine(i, Div(COS_Q12(0) * 16, 0x100), Div(-SIN_Q12(0) * 16, 0x100),
                         Div(SIN_Q12(0) * 16, 0x100), Div(COS_Q12(0) * 16, 0x100));

            if (proc->unk38[i] != 0)
            {
                if (proc->unk4c == i)
                    SetObjAffine(i, Div(COS_Q12(0) * 16, t), Div(-SIN_Q12(0) * 16, t),
                                 Div(SIN_Q12(0) * 16, t), Div(COS_Q12(0) * 16, t));

                if (proc->unk4d == 0)
                {
                    PutSpriteExt(0, ((proc->unk36 + i * 36) & 0x1ff) | (i << 9), 0x330,
                                 gUnknown_08581208,
                                 proc->unk30 + i % proc->unk2a * 16 + (proc->unk32 << 12));
                }
                else
                {
                    col = proc->unk2a - (proc->unk2c - i - 1) % proc->unk2a - 1;
                    PutSpriteExt(0, ((proc->unk36 + i * 36) & 0x1ff) | (i << 9), 0x330,
                                 gUnknown_08581208,
                                 proc->unk30 + col * 16 + (proc->unk32 << 12));
                }
            }
        }
    }

    if (++proc->unk4f == 6)
    {
        proc->unk38[proc->unk4c] = 0;
        proc->unk4c++;
        proc->unk4f = 0;
    }
}
asm(".global sub_080684E0\n.thumb_set sub_080684E0, IntroCoNameBanner_ZoomLetters\n");
