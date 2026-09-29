#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08023360.
 * LoadGameplayGraphics @ 0x08023360
 */

#include "hardware.h"

/* The `(u16)` cast on GetUnitSheetFrameTileCount's result is load-bearing and is NOT the
 * same thing as declaring the callee `u16`: agbcc trusts a narrow RETURN
 * TYPE to have been narrowed by the callee and emits nothing, while an
 * explicit cast leaves both operands of the `&` in their own pseudos --
 * `adds r2,r0,#0; ldr r1,=0x3ff; adds r0,r1,#0; ands r2,r0` against the
 * two-instruction `ldr r2,=0x3ff; ands r2,r0`. Four bytes, and it is the
 * whole difference on this function. (s16) is byte-identical here, so the
 * signedness is not settled -- only the presence of the cast is.
 */
void LoadGameplayGraphics(int a)
{
    sub_08011B18();

    if (a == 1)
        ForceScreenBlack();

    if (a == 0)
        ForceScreenWhite();

    SetupBackgrounds(gUnknown_0849D16C);
    UpdateMapBgScroll();
    FlushLCDControl();

    CpuCopyAuto(gUnknown_0809175C, (void *)0x06003600, 0xa0);
    CpuCopyAuto(GetUnitSheetGraphics(), (void *)0x060046A0, ((u16)GetUnitSheetFrameTileCount() & 0x3ff) * 0x20);
    CpuCopyAuto(GetUnitExtraGraphics(), (void *)0x06005440, 0x200);

    Decompress(gUnknown_080BD1EC, (void *)0x06008000);

    CpuCopyAuto(gUnknown_0809175C + 0xa0, (void *)0x0600E780, 0x20);
    CpuCopyAuto(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
    CpuCopyAuto(gBG1TilemapBuffer, (void *)0x0600F000, 0x800);
    CpuCopyAuto(gBG2TilemapBuffer, (void *)0x06007800, 0x800);

    ApplyPalette((u16 *)(gUnknown_0810E6E0 + (gPlayers[1].teamColor - 1) * 0x20), 12);
    ApplyPalette((u16 *)(gUnknown_0810E6E0 + (gPlayers[2].teamColor - 1) * 0x20), 13);
    ApplyPalette((u16 *)(gUnknown_0810E6E0 + (gPlayers[3].teamColor - 1) * 0x20), 14);
    ApplyPalette((u16 *)(gUnknown_0810E6E0 + (gPlayers[4].teamColor - 1) * 0x20), 15);

    LoadArmyObjPalettes(8);
    sub_0802D2EC();

    ApplyPalette(gUnknown_0809163C, 18);

    sub_08037150(0x1a6);
    RebuildMapUnitLayers2();
    HideRangeOverlay();

    CpuCopyAuto(gBG3TilemapBuffer, (void *)0x0600F800, 0x800);

    ClearMoveSlideSlots();
    sub_080116E8();
    LoadWeatherData();
    ApplyWeatherPalette(gPlaySt.weather);
    LoadCursorSpriteGraphics();

    LoadBg1WindowFrame(gUnknown_030033EC);
    LoadCoPanelGraphics(gUnknown_030033EC);
    LoadArmyObjPalette(gUnknown_030033EC);
}

asm(".global sub_08023360\n.thumb_set sub_08023360, LoadGameplayGraphics\n");
