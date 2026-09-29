#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038568.
 * sub_08038568 @ 0x08038568
 */

void sub_08038568(void)
{
    ResetRulesAfterCampaignMap();

    if (gPlaySt.savingEnabled == 0)
    {
        sub_0803D73C(sub_08016D04(gPlaySt.gameMode), sub_0803B8B8);
    }
    else
    {
        sub_0803BCB8();
        StartMainMenu();
        LinkShutdown();
    }
}
