#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065F88.
 * sub_08065F88 @ 0x08065F88
 */

#include "hardware.h"

void MatchSetupHandleArmyStageInput(void)
{
    u16 v;

    MatchSetupMoveArmyCursor(gUnknown_08580934->unk08 * 2);
    MatchSetupChangeSelectedValue();
    MatchSetupDrawSelectionArrows();

    v = gpKeySt->pressed & 2;

    if (v != 0)
    {
        sub_08063A30(GetCurrentSlotScript(), gUnknown_08580D90);

        ForEachSlotRunningScript(gUnknown_08580AF0, ArmyColumn_StartExitDown);
        ForEachSlotRunningScript(gUnknown_08580B90, ArmyColumn_StartExitDown);
        ForEachSlotRunningScript(gUnknown_08580BC8, ArmyColumn_StartExitDown);

        PlayMusicOrSfx2(0x66);
    }
    else if (gpKeySt->pressed & 1)
    {
        PlayMusicOrSfx2(0x71);

        if (gUnknown_08580934->unk08 == 2)
        {
            gUnknown_08580934->unk30 = 0;
            gUnknown_08580934->unk26 = 2;
            ForEachSlotRunningScript(gUnknown_08580AF0, ArmyColumn_StartExitUp);
            MatchSetupDismissArmyColumns();
            MatchSetupSpawnRuleOptions();
        }
        else
        {
            gUnknown_08580934->unk26 = 1;
            gUnknown_08580934->unk31 = 0;
            gUnknown_08580934->unk32 /= 2;
            ClearCallbackOfSlotsRunningScript(gUnknown_08580AF0);
            ClearCallbackOfSlotsRunningScript(gUnknown_08580B90);
            ClearCallbackOfSlotsRunningScript(gUnknown_08580BC8);
        }
    }
}
asm(".global sub_08065F88\n.thumb_set sub_08065F88, MatchSetupHandleArmyStageInput\n");
