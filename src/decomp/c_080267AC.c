#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080267AC.
 * sub_080267AC @ 0x080267AC
 */

/* gUnknown_08090A74 and gUnknown_08090A78 are NOT globals: they are
 * `-fforce-addr` .rodata address-constant words holding &gUnknown_030033EC and
 * &gPlayers (dumped from baserom.gba), so both globals are named
 * directly here and agbcc rebuilds the pool. Each is read on both sides of the
 * gUnknown_03004080 test, which is the control-flow merge that forces the
 * words in the first place.
 *
 * The embedded `(n = ...)` is load-bearing on BOTH counts and neither is
 * cosmetic. The ROM holds one loaded halfword in r2 across the first and THIRD
 * store and re-`ldrh`s only for the middle one, so all three spelled as the
 * global is +4 bytes (three loads) and all three spelled as `n` is -2
 * (`subs r0,r2,#1`). And the assignment has to happen INSIDE the first
 * statement rather than in one of its own: a leading `n = gUnknown_030033EC;`
 * creates the force-addr pseudo before gUnknown_03004084's, which swaps the
 * first two pool words. */
void StartArmyTurn(void)
{
    u16 n;

    gUnknown_03004084 = (n = gUnknown_030033EC) * 0x20;
    gUnknown_03003F2C = (gUnknown_030033EC - 1) * 0x40;
    gUnknown_03004480 = n;
    gUnknown_030032C0 = 0;
    gPlayers[gUnknown_030033EC].destroyedThisTurn = 0;
    gPlayers[gUnknown_030033EC].unk24 = 0;
    RecomputeArmyVisionMasks();
    AddPlayerIncomeToFunds();
    ApplyArmyWindowFramePalette(gUnknown_030033EC);
    LoadArmyObjPalette(gUnknown_030033EC);
    LoadCoPanelGraphics(gUnknown_030033EC);
    if (gUnknown_03004080 == 1)
    {
        if (DoesArmyHaveHq(gUnknown_030033EC))
            SetMapCursorPosition(gPlayers[gUnknown_030033EC].hqX & 0x7f,
                         gPlayers[gUnknown_030033EC].hqY & 0x7f);
        else
            SetMapCursorPosition(gMap->scrollX / 16 + 7,
                         gMap->scrollY / 16 + 4);
        gPlayers[gUnknown_030033EC].cursorX = gUnknown_030033E4.unk00;
        gPlayers[gUnknown_030033EC].cursorY = gUnknown_030033E4.unk02;
    }
    SetMapCursorPosition(gPlayers[gUnknown_030033EC].cursorX,
                 gPlayers[gUnknown_030033EC].cursorY);
}
asm(".global sub_080267AC\n.thumb_set sub_080267AC, StartArmyTurn\n");
