#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806675C.
 * sub_0806675C @ 0x0806675C, sub_08066808 @ 0x08066808
 */

struct Unk6675CProc
{
    /* 0x00 */ u8 filler_00[0x26];
    /* 0x26 */ s16 unk26;
};
struct Unk66808Proc
{
    /* 0x00 */ u8 filler_00[0x26];
    /* 0x26 */ s16 unk26;
};

void MatchSetupConfirmArmyStage_Loop(struct Unk6675CProc *proc)
{
    if (--proc->unk26 == 6)
        ClearCallbackOfSlotsRunningScript(gUnknown_08580D0C);

    if (proc->unk26 == 3)
    {
        if (gUnknown_08580934->unk08 == 2)
        {
            gUnknown_08580934->unk30 = 0;
            gUnknown_08580934->unk26 = 2;
            ForEachSlotRunningScript(gUnknown_08580AF0, ArmyColumn_StartExitUp);
            MatchSetupDismissArmyColumns();
            MatchSetupSpawnRuleOptions();
            return;
        }

        gUnknown_08580934->unk26 = 1;
        ClearCallbackOfSlotsRunningScript(gUnknown_08580AF0);
        ClearCallbackOfSlotsRunningScript(gUnknown_08580B90);
        ClearCallbackOfSlotsRunningScript(gUnknown_08580BC8);
    }

    if (proc->unk26 < 0)
    {
        ClearSlotScriptCallback(gUnknown_03001FBC);
        gUnknown_08580934->unk2d--;
        LinkRestartKeySync();
    }
}
asm(".global sub_0806675C\n.thumb_set sub_0806675C, MatchSetupConfirmArmyStage_Loop\n");

void MatchSetupConfirmTeamStage_Loop(struct Unk66808Proc *proc)
{
    if (--proc->unk26 == 6)
        ClearCallbackOfSlotsRunningScript(gUnknown_08580D0C);

    if (proc->unk26 == 3)
    {
        PlayMusicOrSfx2(0x67);
        MatchSetupDismissArmyColumns();
        MatchSetupSpawnRuleOptions();
        gUnknown_08580934->unk30 = 0;
    }

    if (proc->unk26 < 0)
    {
        ClearSlotScriptCallback(gUnknown_03001FBC);
        gUnknown_08580934->unk2d--;
        LinkRestartKeySync();
    }
}
asm(".global sub_08066808\n.thumb_set sub_08066808, MatchSetupConfirmTeamStage_Loop\n");
