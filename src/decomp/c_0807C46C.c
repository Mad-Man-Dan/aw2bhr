#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807C46C.
 * sub_0807C46C @ 0x0807C46C
 */

void MissionTitleLetters_PutFinalLabelAndBar(void *arg)
{
    int n;
    int i;

    PutSprite(0, 0x68, 8, gUnknown_08615C62, 0);
    n = GetCampaignResultCountPlusOne();
    if (n <= 9)
    {
        PutSprite(0, 0xca, 8, gUnknown_0848B690, n * 4 + 0x1c);
    }
    else
    {
        PutSprite(0, 0xc8, 8, gUnknown_0848B690, Div(n, 10) * 4 + 0x1c);
        if (Div(n, 10) == 1)
            PutSprite(0, 0xd2, 8, gUnknown_0848B690, DivRem(n, 10) * 4 + 0x1c);
        else if (Div(n, 10) == 7)
            PutSprite(0, 0xd5, 8, gUnknown_0848B690, DivRem(n, 10) * 4 + 0x1c);
        else
            PutSprite(0, 0xd6, 8, gUnknown_0848B690, DivRem(n, 10) * 4 + 0x1c);
    }
    for (i = 0; i < 8; i++)
    {
        PutSprite(0, i * 32, 0x30, gUnknown_0848B6BE, 0x18);
        PutSprite(0, i * 32, 0x32, gUnknown_0848B6BE, 0x18);
    }
}
asm(".global sub_0807C46C\n.thumb_set sub_0807C46C, MissionTitleLetters_PutFinalLabelAndBar\n");
