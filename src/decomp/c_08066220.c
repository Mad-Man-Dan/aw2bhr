#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066220.
 * sub_08066220 @ 0x08066220
 */

#include "hardware.h"

void MatchSetupHandleTeamStageInput(void)
{
    u16 v;

    MatchSetupMoveArmyCursor(gUnknown_08580934->unk08);
    MatchSetupCycleTeam(gpKeySt->repeated, gUnknown_08580934->unk32, 1);
    MatchSetupDrawTeamArrows();

    if (gUnknown_08580934->unk08 == 2)
    {
        if (gUnknown_08580934->unk31 == 1)
            gpKeySt->pressed = 2;
        else
            gpKeySt->pressed = 1;
    }

    v = gpKeySt->pressed & 2;

    if (v != 0)
    {
        gUnknown_08580934->unk26 = 0;
        gUnknown_08580934->unk32 *= 2;

        ForEachSlotRunningScript(gUnknown_08580AF0, sub_08066200);
        ForEachSlotRunningScript(gUnknown_08580B90, sub_08066200);
        ForEachSlotRunningScript(gUnknown_08580BC8, sub_08066200);
        ForEachSlotRunningScript(gUnknown_08580A38, TeamBadge_StartExitDown);
        ForEachSlotRunningScript(gUnknown_08580A08, TeamBadge_StartExitDown);

        PlayMusicOrSfx2(0x66);
    }
    else if (gpKeySt->pressed & 1)
    {
        MatchSetupDismissArmyColumns();
        MatchSetupSpawnRuleOptions();
        gUnknown_08580934->unk30 = 0;
        PlayMusicOrSfx2(0x71);
    }
}
asm(".global sub_08066220\n.thumb_set sub_08066220, MatchSetupHandleTeamStageInput\n");
