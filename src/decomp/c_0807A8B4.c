#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A8B4.
 * sub_0807A8B4 @ 0x0807A8B4
 */

void ResultsScreen_BeginClose(ProcPtr proc)
{
    int i;

    if (IsCampaignMilestoneMission() == 0)
        FadeOutMusic(0);

    for (i = 0; i <= 6; i++)
        StartPalFadeToWhite(i, 0x10, proc);

    StartPalFadeToWhite(8, 0x10, proc);
    StartPalFadeToWhite(0x1b, 0x10, proc);
}
asm(".global sub_0807A8B4\n.thumb_set sub_0807A8B4, ResultsScreen_BeginClose\n");
