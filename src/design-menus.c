#include "global.h"
#include "hardware.h"
#include "map.h"


void sub_08004724(void)
{
    u16 tiles[4];
    const u8 *p;
    struct Map *m;
    int x;
    int y;
    int c;

    sub_0808B6E8(tiles, gUnknown_0808D7A0, 8);
    c = 7;
    p = gUnknown_08486FC4;

    for (y = 0; y < gMap->height; y++)
        for (x = 0; x < gMap->width; x++)
        {
            m = gMap;
            m->tile[gMap->rowOffset[y] + x] = 0x2A;
            m->terrain[gMap->rowOffset[y] + x] = c;
        }

    for (y = 0; y < gMap->height; y++)
        for (x = 0; x < gMap->width; x++)
            sub_08003F44(x, y, tiles[*p++]);
}

void DesignRoomMenu_EnterPaintMode(void)
{
    DesignRoomSetMode(1);
}
asm(".global sub_08004818\n.thumb_set sub_08004818, DesignRoomMenu_EnterPaintMode\n");

void sub_08004824(void)
{
    PlayMusicOrSfx2(0x66);
    DesignRoomSetMode(1);
}

int sub_08004838(void)
{
    return 0;
}

void DesignRoomMenu_CloseToPaint(void)
{
    CloseTopMenu();
    DesignRoomSetMode(1);
}
asm(".global sub_0800483C\n.thumb_set sub_0800483C, DesignRoomMenu_CloseToPaint\n");

void DesignRoomMenu_CloseToMainMenu(void)
{
    CloseTopMenu();
    DesignRoomOpenMainMenu();
}
asm(".global sub_0800484C\n.thumb_set sub_0800484C, DesignRoomMenu_CloseToMainMenu\n");

void DesignRoomOpenMainMenu(void)
{
    PopMenu();
    PlayMusicOrSfx2(0x66);
    gActiveMap->mode = 3;
    sub_08004C5C();
}
asm(".global sub_0800485C\n.thumb_set sub_0800485C, DesignRoomOpenMainMenu\n");

void sub_0800487C(void)
{
    struct Unk03001470 * p;

    p = &gUnknown_03001470[gUnknown_03001FBC];

    if (sub_0808B694(&p->unk1e, gActiveMap->designName))
        gActiveMap->flags |= 0x1000;

    sub_08023348();
    InstallMapFrameCallbacks();
    RebuildMapUnitLayers2();
    ReloadGameplayPalettes();
    DesignRoomLoadTerrainNamePalettes();
}

void DesignRoomStartNameEntry(void)
{
    struct Unk03001470 *p;
    u8 *d;
    u8 *q;
    int i;

    sub_08011B18();
    p = &gUnknown_03001470[gUnknown_03001FBC];
    i = 0;
    d = (u8 *)&p->unk1e;

    do
    {
        d[i] = gActiveMap->designName[i];
        q = gActiveMap->designName;
        if (q[i] == 0)
            break;
        i++;
    } while (i <= 0x12);

    StartNameEntryMode1((int)q, 8);
}
asm(".global sub_080048D4\n.thumb_set sub_080048D4, DesignRoomStartNameEntry\n");

void DesignRoomOnNameEntryDone(void)
{
    PlayMusic(0xd8);
    DesignRoomSetMode(1);
    gActiveMap->designName[0x12] = 0;
}
asm(".global sub_0800492C\n.thumb_set sub_0800492C, DesignRoomOnNameEntryDone\n");

void sub_0800494C(void)
{
    DesignRoomSetMode(4);
}

void sub_08004958(void)
{
    CloseTopMenu();
    sub_080152EC(gUnknown_0848721C, 0);
}

void sub_08004970(void)
{
    DesignRoomDrawPropertyCounts();

    if ((gpKeySt->pressed & 7) != 0)
    {
        FillTilemapRect(gBG2TilemapBuffer, 9, 2, 0xB, 0x11, 0x360);
        BG_EnableSyncBG2();
        sub_08002E5C();
        sub_08002E3C();
        ClearSlotScriptCallback(gUnknown_03001FBC);

        if ((gpKeySt->pressed & 2) == 0)
            gUnknown_03002F1C = 1;
    }
}

void sub_080049DC(void)
{
    sub_08002FE4();
}

void sub_080049E8(void)
{
    PushMenu();
    CloseTopMenu();
    DesignRoomSetMode(6);
    sub_080152EC(gUnknown_084872B4, 0);
    gActiveMap->menuCursorX = 0x57;
    gActiveMap->menuCursorY = 0x10;
}

void DesignRoomMenu_CloseToMainMenu2(void)
{
    CloseTopMenu();
    DesignRoomOpenMainMenu();
}
asm(".global sub_08004A20\n.thumb_set sub_08004A20, DesignRoomMenu_CloseToMainMenu2\n");

void DesignRoomMenu_NewMap(int a)
{
    CloseTopMenu();
    DesignRoomNewMap(a);
    sub_08002E3C();
    RebuildMapUnitLayers2();
    gActiveMap->flags |= 0x1000;
}
asm(".global sub_08004A30\n.thumb_set sub_08004A30, DesignRoomMenu_NewMap\n");

void DesignRoomMenu_NewMapSea(void)
{
    DesignRoomMenu_NewMap(7);
}
asm(".global sub_08004A60\n.thumb_set sub_08004A60, DesignRoomMenu_NewMapSea\n");

void DesignRoomMenu_NewMapPlain(void)
{
    DesignRoomMenu_NewMap(1);
}
asm(".global sub_08004A6C\n.thumb_set sub_08004A6C, DesignRoomMenu_NewMapPlain\n");

void DesignRoomMenu_NewMapMountain(void)
{
    DesignRoomMenu_NewMap(3);
}
asm(".global sub_08004A78\n.thumb_set sub_08004A78, DesignRoomMenu_NewMapMountain\n");

void DesignRoomMenu_NewMapForest(void)
{
    DesignRoomMenu_NewMap(4);
}
asm(".global sub_08004A84\n.thumb_set sub_08004A84, DesignRoomMenu_NewMapForest\n");

void DesignRoomMenu_NewMapRandom(void)
{
    DesignRoomMenu_NewMap(-1);
}
asm(".global sub_08004A90\n.thumb_set sub_08004A90, DesignRoomMenu_NewMapRandom\n");

void sub_08004AA0(void)
{
    CloseTopMenu();
    sub_080152EC(gUnknown_084873BC, 0);
    sub_08000654();
}

void sub_08004ABC(void)
{
    CloseTopMenu();
    sub_080152EC(gUnknown_08487404, 0);
    sub_08000654();
}

void sub_08004AD8(void)
{
    CloseTopMenu();
    sub_080152EC(gUnknown_0848744C, 0);
    sub_08000654();
}

void sub_08004AF4(void)
{
    CloseTopMenu();
    sub_080152EC(gUnknown_08487494, 0);
    sub_08000654();
}

void sub_08004B10(void)
{
    CloseTopMenu();
    sub_080152EC(gUnknown_084874DC, 0);
    sub_08000654();
}

void sub_08004B2C(void)
{
    StartEventScript(gUnknown_08487754);
}

void sub_08004B3C(void)
{
    StartEventScript(gUnknown_084877F4);
}

void sub_08004B4C(void)
{
    StartEventScript(gUnknown_08487894);
}

void sub_08004B5C(void)
{
    StartEventScript(gUnknown_08487934);
}

void sub_08004B6C(void)
{
    StartEventScript(gUnknown_084879D4);
}

void sub_08004B7C(void)
{
    PushMenu();
    CloseTopMenu();
    InitTextTileCache(0x70);
    CreateSubMenu(gUnknown_084872FC, 2, 2, 0);
    DesignRoomSetMode(7);
    DesignRoomLoadTerrainNamePalettes();
    gActiveMap->menuCursorX = 0x15;
    gActiveMap->menuCursorY = 0x10;
}

void sub_08004BC0(void)
{
    gActiveMap->flags |= 0x4000;
}

void sub_08004BD8(void)
{
    CloseTopMenu();
    DesignRoomSetMode(9);
    StartEventScript((gActiveMap->flags & 0x1000) ? gUnknown_08487B64
                                                     : gUnknown_08487AC4);
}

void sub_08004C10(void)
{
    sub_080037AC();
    gActiveMap->state = 2;
    gActiveMap->menuCursorX = 0x15;
    gActiveMap->menuCursorY = 0x18;
}

void sub_08004C34(void)
{
    PushMenu();
    CloseTopMenu();
    RebuildMapUnitLayers2();
    CreateSubMenu(gUnknown_08487C04, 2, 3, 0);
    sub_08004C10();
}

void sub_08004C5C(void)
{
    gActiveMap->stateChanged = 0;
    gActiveMap->state = 1;
    sub_08003704();
    gActiveMap->menuCursorX = 0x15;
    gActiveMap->menuCursorY = 0x10;
    InitTextTileCache(0x70);
    SetMapCursorDisplayPosition(0x10, 0x10);
    DesignRoomHideTilePanel();
    DesignRoomHideCoordBox();
}

void DesignRoomMode_Menu(void)
{
    int x;

    if (gActiveMap->stateChanged != 0)
    {
        sub_08004C5C();
        RebuildMapUnitLayers2();
        CreateRootMenuWithSfx(gUnknown_08487C84, 2, 2, 0);
    }

    switch (gActiveMap->state)
    {
    case 1:
        sub_080036A4();
        break;
    case 2:
        sub_0800376C();
        break;
    case 3:
        x = 1;
        break;
    case 4:
        x = 2;
        break;
    case 5:
        x = 3;
        break;
    case 6:
        x = 4;
        break;
    case 7:
        x = 5;
        break;
    }
}
asm(".global sub_08004CA0\n.thumb_set sub_08004CA0, DesignRoomMode_Menu\n");

void sub_08004D10(void)
{
    DesignRoomHideTilePanel();
    DesignRoomResetArmyPanels();
    StartEventScript(gUnknown_08487D44);
}

void sub_08004D28(void)
{
    switch ((s8)gActiveMap->designSlot)
    {
    case 0:
        DesignRoomShowSlotPreview0(0, 0, 0);
        break;
    case 1:
        DesignRoomShowSlotPreview1(0, 0, 0);
        break;
    case 2:
        DesignRoomShowSlotPreview2(0, 0, 0);
        break;
    }
}

void sub_08004D74(int a, int b)
{
    sub_08004DD4(a, b, gTextTable[0x9fa], 0);
}
