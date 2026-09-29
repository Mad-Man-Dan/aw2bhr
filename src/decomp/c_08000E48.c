#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08000E48.
 * sub_08000E48 @ 0x08000E48
 */

/* The map-screen bring-up, entered as a Proc callback: everything it does is
 * keyed off proc->unk1e, which is 0 on a fresh start and non-zero on a resume,
 * and the two `if (a == 0)` blocks are the fresh-start-only halves.
 *
 * gActiveMap is reached TWO ways in one function and that is agbcc,
 * not two source spellings: -fforce-addr parks &gActiveMap in this
 * unit's own pool (0x0808D6EC in the ROM) for the reads inside the loop, while
 * the later statements get an ordinary inline pool word. Likewise 0x0808D6F0 /
 * 0x0808D6F4 hold &MapVBlankCallback / &MapMainLoopCallback with the THUMB bit set, and
 * 0x0808D6F8 / 0x0808D6FC hold &gUnknown_03003F3C and a script blob at
 * 0x084858AC -- all five are written honestly here and agbcc rebuilds the pool.
 *
 * 0x084858AC has no symbol of its own: data/data.s covers 0x084857AC..0x084858DC
 * as one incbin, so the script is named as an offset into it. */
void DesignRoomProc_Init(struct Unk03001470 *proc)
{
    int a;
    int i;

    a = proc->unk1e;

    gActiveMap = &gUnknown_0200B000;
    gUnknown_0200B000.designName[0] = 0;

    if (a == 0)
    {
        sub_08001124((u8 *)&gUnknown_0200B000, 0xB0);
        sub_08001124((u8 *)gDesignRing, 0x134);
        DesignRoomBuildItemList(0, 1);
    }

    gUnknown_030040E4 = 0;
    gUnknown_030040E8 = 0;
    gMap->unk4233 = 4;
    ResetAllPlayers();
    SetDefaultRules();
    InitPlayersFromSettings();
    gPlayers[0].co = 0;
    gPlayers[1].co = 1;
    gPlayers[2].co = 3;
    gPlayers[3].co = 8;
    gPlayers[4].co = 6;
    gPlaySt.event20 = 0;
    gUnknown_030033EC = 0;
    gUnknown_03004080 = 1;
    AdvanceToNextActiveArmy();
    sub_08000DF8(a);
    ClearAllUnits();
    ResetInventionRecords();
    StartArmyTurn2();
    RebuildMapUnitLayers();
    sub_08023348();

    if (a == 0)
    {
        gActiveMap->flags |= 0x1000;
        gActiveMap->selectionIndex = 0;
        gActiveMap->propertyCount = 0;

        for (i = 0; i <= 3; i++)
        {
            gActiveMap->hqX[i] |= 0xFF;
            gActiveMap->hqY[i] |= 0xFF;
        }

        gActiveMap->spriteId = 0xFF;
        gActiveMap->countPanelX = 0xFC00;
        gActiveMap->sidePanelX = 0x1220;
        gActiveMap->panelSide = 1;
    }

    InstallMapFrameCallbacks();
    sub_080366D0(MapVBlankCallback);
    sub_080366C4(MapMainLoopCallback);
    gUnknown_03003F3C = 0;
    InitTextTileCache(0);
    DesignRoomLoadGraphics();
    SetMapCursorPosition(7, 4);
    gActiveMap->designSlot = 0xFF;
    gActiveMap->tilePanelX = 0xFD80;
    gActiveMap->tilePanelY = 0xB8;
    gActiveMap->tilePanelState = 0x64;
    gActiveMap->selectedTerrain = 1;
    gActiveMap->propertyArmy = 1;
    gActiveMap->unitArmy = 1;
    DesignRoomGetPreviousRingIndex();
    DesignRoomSetMode(0);
    sub_080152C0((s32)&gUnknown_084857AC[0x100], 0);
    DesignRoomSelectItem(0x28);
    DesignRoomStartCoordBox();
    PlayMusic(0xD8);
}
asm(".global sub_08000E48\n.thumb_set sub_08000E48, DesignRoomProc_Init\n");
