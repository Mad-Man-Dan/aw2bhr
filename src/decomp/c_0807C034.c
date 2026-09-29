#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807C034.
 * sub_0807C034 @ 0x0807C034
 */

#include "hardware.h"
#include "proc.h"
struct Unk807C034
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[0x18];
    /* 0x66 */ s16 unk66;
};

void MissionTitleLetters_PutRevealingLetters(void *arg)
{
    struct Unk807C034 *proc = arg;
    int i;

    gUnknown_030058F8 = proc->unk66;

    SetObjAffine(1,
                 Div(gSinLut[0x40] << 4,
                     480 - DivRem(proc->unk66, 10) * 15
                         ? 480 - DivRem(proc->unk66, 10) * 15
                         : 2),
                 Div(-gSinLut[0] << 4,
                     480 - DivRem(proc->unk66, 10) * 15
                         ? 480 - DivRem(proc->unk66, 10) * 15
                         : 2),
                 Div(gSinLut[0] << 4,
                     480 - DivRem(proc->unk66, 10) * 15
                         ? 480 - DivRem(proc->unk66, 10) * 15
                         : 2),
                 Div(gSinLut[0x40] << 4,
                     480 - DivRem(proc->unk66, 10) * 15
                         ? 480 - DivRem(proc->unk66, 10) * 15
                         : 2));

    for (i = 0; i < proc->unk4c; i++)
    {
        if (proc->unk66 >= i * 10 && proc->unk66 < i * 10 + 10)
        {
            gUnknown_0300590C = Div(proc->unk66, 10);
            gUnknown_03005960 = gUnknown_0202FF78[gUnknown_0300590C + 1] - gUnknown_0202FF78[i] - 8;
            PutSpriteExt(0, (0xd0 - gUnknown_03005960) | 0x200, 0x308, gUnknown_0848B6E6,
                         i * 8 + 0x80);

            if (proc->unk66 == i * 10 && Proc_Find(gUnknown_08616570) == 0)
                PlayMusicOrSfx2(0x1CD);
        }
        else if (proc->unk66 >= i * 10 + 10)
        {
            gUnknown_0300590C = Div(proc->unk66, 10);
            gUnknown_03005960 = gUnknown_0202FF78[gUnknown_0300590C + 1] - gUnknown_0202FF78[i];
            PutSprite(0, 0xe0 - gUnknown_03005960, 0x18, gUnknown_0848B6E6, i * 8 + 0x80);
        }
    }

    proc->unk66++;
}
asm(".global sub_0807C034\n.thumb_set sub_0807C034, MissionTitleLetters_PutRevealingLetters\n");
