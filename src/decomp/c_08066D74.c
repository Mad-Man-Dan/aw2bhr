#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066D74.
 * sub_08066D74 @ 0x08066D74
 */

#include "hardware.h"

void MatchSetupHandleRulesStageInput(void)
{
    int sc;

    gUnknown_08580934->unk2a++;

    MatchSetupMoveRuleCursor();
    RuleOption_ChangeValue(gUnknown_08580934->unk54[gUnknown_08580934->unk33]);
    RuleOption_DrawArrows(gUnknown_08580934->unk33);
    MatchSetupHighlightSelectedRuleOption();

    sc = (SinDegrees(gUnknown_08580934->unk2a * 16 % 360) >> 9) + 0x100;

    SetObjAffine(0,
                 Div(gSinLut[0x40] * 16, sc != 0 ? sc : 2),
                 Div(-gSinLut[0] * 16, sc != 0 ? sc : 2),
                 Div(gSinLut[0] * 16, sc != 0 ? sc : 2),
                 Div(gSinLut[0x40] * 16, sc != 0 ? sc : 2));

    if (gpKeySt->pressed & 1)
    {
        ClearCallbackOfSlotsRunningScript(gUnknown_08580DD8);
        LockMainMenu();
    }
    else if (gpKeySt->pressed & 2)
    {
        gUnknown_08580934->unk31 = 1;
        sub_080733B8();
        MatchSetupDismissRuleOptions(2);
        gUnknown_08580934->unk30 = 1;
        PlayMusicOrSfx2(0x66);

        if (gUnknown_08580934->unk08 == 2)
        {
            gUnknown_08580934->unk26 = 0;
            MatchSetupSpawnArmyColumnsSlide();
        }
        else
        {
            MatchSetupSpawnArmyColumnsWithBadges();
            gUnknown_08580934->unk26 = 1;
        }
    }
}
asm(".global sub_08066D74\n.thumb_set sub_08066D74, MatchSetupHandleRulesStageInput\n");
