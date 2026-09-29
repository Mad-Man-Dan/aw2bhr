#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BA4C.
 * sub_0803BA4C @ 0x0803BA4C
 */

#include "proc.h"

/* The tree-3 starter for ProcScr_Campaign, mode 1 -- the fourth member of the
 * sub_0803B8C4 / StartWarRoom / StartCampaignAfterMap family.
 *
 * The two SetHardCampaignFlag calls are an if/else and NOT a ternary: a ternary
 * computes one constant and falls into a shared tail, whereas this has two
 * separate `bl`s. GetHardCampaignToggle's u8 return is tested with a bare `lsls #24`
 * and no `lsrs`, which is all a zero test needs. */
void MainMenuCampaign_NewGame(void)
{
    ClearCampaignFlags60To9F();
    if (GetHardCampaignToggle() != 0)
        SetHardCampaignFlag(1);
    else
        SetHardCampaignFlag(0);
    BackupBattleMapPoints();
    gPlaySt.gameMode = 1;
    Proc_Start(ProcScr_Campaign, PROC_TREE_3);
}
asm(".global sub_0803BA4C\n.thumb_set sub_0803BA4C, MainMenuCampaign_NewGame\n");
