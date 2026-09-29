#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034780.
 * SetDefaultRules @ 0x08034780
 */

/* Named per Xenesis's AW2 Subroutine List: "Sets the default rules (Used
 * for War Room, used to clear after a campaign map)". The old SetDefaultRules
 * symbol is kept as a linker alias below so every other unit keeps
 * resolving it unchanged. */
void SetDefaultRules(void)
{
    gUnknown_030040F0 = 0;
    gUnknown_030044A0 = 0;

    gPlaySt.aiControlled[1] = 1;
    gPlaySt.aiControlled[2] = 1;
    gPlaySt.aiControlled[3] = 1;
    gPlaySt.aiControlled[4] = 1;
    gPlaySt.armyColor[1] = 1;
    gPlaySt.armyColor[2] = 2;
    gPlaySt.armyColor[3] = 3;
    gPlaySt.armyColor[4] = 4;
    gPlaySt.co[1] = 1;
    gPlaySt.co[2] = 2;
    gPlaySt.co[3] = 4;
    gPlaySt.co[4] = 0xb;

    SetFreeForAllTeams();

    gPlaySt.turnLimit = 0;
    gPlaySt.captureLimit = 0;
    gPlaySt.propertyFunds = 1000;
    gPlaySt.unk24 = 0;
    gPlaySt.unk10[1] = 0;
    gPlaySt.unk10[2] = 0;
    gPlaySt.unk10[3] = 0;
    gPlaySt.unk10[4] = 0;
    gPlaySt.fog = 0;
    gPlaySt.defaultWeather = 0;
    gPlaySt.weather = 0;
    gPlaySt.randomWeatherOn = 0;
    gPlaySt.unk2e = 0;
    gPlaySt.event20 = 3;
    gPlaySt.dispMiniPanel = 1;
    gPlaySt.campaignRelated = 0;
    gPlaySt.coAbilities = 1;
    gPlaySt.coPowersEnabled = 1;
    gPlaySt.animOpts = gUnknown_0200C420.unk0e;
    gPlaySt.bgmOn = (gUnknown_0200C420.unk14 == 0);
}

asm(".global sub_08034780\n.thumb_set sub_08034780, SetDefaultRules\n");
