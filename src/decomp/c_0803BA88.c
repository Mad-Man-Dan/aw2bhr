#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BA88.
 * sub_0803BA88 @ 0x0803BA88
 */

#include "proc.h"

/* Picks link-slot 1, refreshes the two caches, and either hands the slot to
 * StartResumeScript or falls back to mode 1 and the gUnknown_0849EB7C script.
 *
 * The slot is `u8` -- GetSuspendIdForGameMode's `s8` return is re-narrowed
 * `lsls #0x18; lsrs #0x18` at the call site, which is the UNSIGNED half of the
 * pair -- and it is cast back to `s8` for GetSuspendFlag and passed as-is
 * everywhere else. The two predicates are one `&&`: both failures land on the
 * same else block.
 *
 * `movs r1,#3` is PROC_TREE_3, not a bare integer. */
void MainMenuCampaign_Continue(void)
{
    u8 v;

    v = GetSuspendIdForGameMode(1);

    ReloadCampaignFlagBank2FromProfile();
    BackupBattleMapPoints();

    if (GetSuspendFlag(v) && sub_08016E04(v))
    {
        StartResumeScript(v);
    }
    else
    {
        gPlaySt.gameMode = 1;
        Proc_Start(gUnknown_0849EB7C, PROC_TREE_3);
    }
}
asm(".global sub_0803BA88\n.thumb_set sub_0803BA88, MainMenuCampaign_Continue\n");
