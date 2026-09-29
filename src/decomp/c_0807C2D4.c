#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807C2D4.
 * sub_0807C2D4 @ 0x0807C2D4
 */

#include "hardware.h"
struct Unk807C2D4
{
    /* 0x00 */ u8 filler_00[0x60];
    /* 0x60 */ int unk60;
};

void MissionTitleLetters_PutMissionLabelAndBar(void *arg)
{
    struct Unk807C2D4 *proc = arg;
    int n;
    int i;
    int x;

    SetObjAffine(0, Div(gSinLut[0x40] << 4, 0x100),
                 Div(-gSinLut[0] << 4,
                     proc->unk60 * 16 + 16 ? proc->unk60 * 16 + 16 : 2),
                 Div(gSinLut[0] << 4, 0x100),
                 Div(gSinLut[0x40] << 4,
                     proc->unk60 * 16 + 16 ? proc->unk60 * 16 + 16 : 2));

    PutSpriteExt(0, 0x68, 0x108, gUnknown_08615C62, 0);
    n = GetCampaignResultCountPlusOne();
    if (n <= 9)
    {
        PutSpriteExt(0, 0xca, 0x108, gUnknown_0848B690, n * 4 + 0x1c);
    }
    else
    {
        PutSpriteExt(0, 0xc8, 0x108, gUnknown_0848B690, Div(n, 10) * 4 + 0x1c);
        if (Div(n, 10) == 1)
            PutSpriteExt(0, 0xd2, 0x108, gUnknown_0848B690, DivRem(n, 10) * 4 + 0x1c);
        else if (Div(n, 10) == 7)
            PutSpriteExt(0, 0xd5, 0x108, gUnknown_0848B690, DivRem(n, 10) * 4 + 0x1c);
        else
            PutSpriteExt(0, 0xd6, 0x108, gUnknown_0848B690, DivRem(n, 10) * 4 + 0x1c);
    }

    proc->unk60++;

    for (i = 0; i < proc->unk60; i++)
    {
        x = i * 16 + 0x78;
        PutSprite(0, x - proc->unk60 * 8, 0x30, gUnknown_0848B6A8, 0x18);
        PutSprite(0, x - proc->unk60 * 8, 0x32, gUnknown_0848B6A8, 0x18);
    }
}
asm(".global sub_0807C2D4\n.thumb_set sub_0807C2D4, MissionTitleLetters_PutMissionLabelAndBar\n");
