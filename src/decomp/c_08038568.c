#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038568.
 * sub_08038568 @ 0x08038568
 */

void EndOfGame_FinishVersusMap(void)
{
    ResetRulesAfterCampaignMap();

    if (gPlaySt.savingEnabled == 0)
    {
        StartSaveScreen(GetSuspendIdForGameMode(gPlaySt.gameMode), MainMenuVersus_NewGame);
    }
    else
    {
        RestoreCampaignFlags();
        StartMainMenu();
        LinkShutdown();
    }
}
asm(".global sub_08038568\n.thumb_set sub_08038568, EndOfGame_FinishVersusMap\n");
