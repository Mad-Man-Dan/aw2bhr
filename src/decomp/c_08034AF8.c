#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034AF8.
 * sub_08034AF8 @ 0x08034AF8
 */

#include "hardware.h"

/* gUnknown_08090E24 is a `-fforce-addr` .rodata word holding &gUnknown_030033EC
 * (dumped from baserom.gba), not a global -- it is read on both sides of the
 * ShouldPromptCountryName test. gUnknown_08090D90 immediately before it IS a real u16
 * table (0,1,2,3,3,3,2,1,0,0), reached with `lsls #1; adds; ldrh`.
 *
 * gGameClock is declared s32 but the division is `__udivsi3`/`__umodsi3`,
 * so the source read it unsigned; the cast is on the read rather than a retype
 * of the global, which a dozen other files share. */
void MapState_TurnHandoverPrompt(void)
{
    if (ShouldPromptCountryName())
    {
        PutArmyNameBanner(0x4e, gPlayers[GetNextActiveArmy(gUnknown_030033EC)].teamColor);
        switch (gUnknown_02028E40)
        {
        case 0:
            PutCenteredAsciiStringSprites(0x38, gUnknown_08090DA4);
            PutCenteredAsciiStringSprites(gUnknown_08090D90[(u32)gGameClock / 3 % 10] + 0x68,
                         gUnknown_08090DB0);
            break;
        case 1:
            PutCenteredAsciiStringSprites(0x38, gUnknown_08090DC0);
            PutCenteredAsciiStringSprites(gUnknown_08090D90[(u32)gGameClock / 3 % 10] + 0x68,
                         gUnknown_08090DD0);
            break;
        case 2:
            PutCenteredAsciiStringSprites(0x38, gUnknown_08090DE0);
            PutCenteredAsciiStringSprites(gUnknown_08090D90[(u32)gGameClock / 3 % 10] + 0x68,
                         gUnknown_08090DF0);
            break;
        case 3:
            PutCenteredAsciiStringSprites(0x30, gUnknown_08090E04);
            PutCenteredAsciiStringSprites(gUnknown_08090D90[(u32)gGameClock / 3 % 10] + 0x68,
                         gUnknown_08090E14);
            break;
        }
        if ((gpKeySt->pressed & 1) == 0)
            return;
    }
    AdvanceToNextActiveArmy();
    StartArmyTurn2();
    ClearPlayerCoPowerStatus(gUnknown_030033EC);
    RebuildMapUnitLayers2();
    AiBuildInterestLists();
    if (ShouldPromptCountryName())
        StartScreenRevealWipeLocked();
    sub_08034C8C();
    FadeOutMusicDefault();
    gUnknown_030032D8 = 4;
}
asm(".global sub_08034AF8\n.thumb_set sub_08034AF8, MapState_TurnHandoverPrompt\n");
