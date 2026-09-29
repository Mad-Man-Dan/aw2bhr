#include "global.h"
#include "hardware.h"
#include "map.h"

void LoadMapData(u16 a1)
{
    void *p;

    if (a1 >= 0xb4 && a1 <= 0xbf)
    {
        SetLoadedMapBlob(HeapMalloc(0x724));
        ReadSaveSlot(8, (u8 *)gUnknown_03003F68);
    }
    else
    {
        SetLoadedMapBlob(HeapMalloc(0xa14));

        p = gUnknown_085C77A0[a1].mapData[IsHardCampaignMode()];
        if (p == NULL)
            p = gUnknown_085C77A0[a1].mapData[0];

        LZ77UnCompWram(p, gUnknown_03003F68);
    }
}

asm(".global sub_080247A4\n.thumb_set sub_080247A4, LoadMapData\n");

void FreeMapLoadBuffer(void)
{
    HeapFree(gUnknown_03003F68);
}

asm(".global sub_0802481C\n.thumb_set sub_0802481C, FreeMapLoadBuffer\n");

void ReloadGameplayPalettes(void)
{
    ApplyPaletteExt((u16 *)(gUnknown_0810E6E0 + (gPlayers[1].teamColor - 1) * 0x20),
                    0x180, 0x20);
    ApplyPaletteExt((u16 *)(gUnknown_0810E6E0 + (gPlayers[2].teamColor - 1) * 0x20),
                    0x1A0, 0x20);
    ApplyPaletteExt((u16 *)(gUnknown_0810E6E0 + (gPlayers[3].teamColor - 1) * 0x20),
                    0x1C0, 0x20);
    ApplyPaletteExt((u16 *)(gUnknown_0810E6E0 + (gPlayers[4].teamColor - 1) * 0x20),
                    0x1E0, 0x20);

    LoadArmyObjPalettes(8);
    sub_0802D2EC();

    ApplyPaletteExt(gUnknown_0809163C, 0x240, 0x20);

    ApplyWeatherPalette(gPlaySt.weather);
    LoadCursorSpriteGraphics();

    LoadBg1WindowFrame(gUnknown_030033EC);
    LoadCoPanelGraphics(gUnknown_030033EC);
    LoadArmyObjPalette(gUnknown_030033EC);
}
asm(".global sub_08024830\n.thumb_set sub_08024830, ReloadGameplayPalettes\n");

u8 *GetLoadedMapName(void)
{
    return gMap->unk421a;
}
asm(".global sub_080248E4\n.thumb_set sub_080248E4, GetLoadedMapName\n");

u8 GetLoadedMapArmyCount(void)
{
    return gMap->unk4233;
}
asm(".global sub_080248F8\n.thumb_set sub_080248F8, GetLoadedMapArmyCount\n");

u8 GetMapArmyCount(u16 a1)
{
    if ((u16)(a1 - 0xB4) <= 0xB)
        return GetDesignRoomSlotArmyCount(a1 + 0x4C);

    return gUnknown_085C77A0[a1].unk18;
}
asm(".global sub_0802490C\n.thumb_set sub_0802490C, GetMapArmyCount\n");

u8 *GetMapName(u16 a1)
{
    if ((u16)(a1 - 0xB4) <= 0xB)
        return GetDesignRoomSlotName(a1 + 0x4C);

    return gTextTable[gUnknown_085C77A0[a1].nameIndex];
}
asm(".global sub_08024944\n.thumb_set sub_08024944, GetMapName\n");

int GetCellCountry(int a1)
{
    int r = gUnknown_085C77A0[gPlaySt.mapID].unk58;

    if (r == 0 || (a1 & 0x1F) == 8)
    {
        int i = a1 & 0xE0;

        if (i != 0)
            r = GetPlayerCoCountry(i >> 5);
        else
            r = 0;
    }

    return r;
}
asm(".global sub_08024984\n.thumb_set sub_08024984, GetCellCountry\n");

int GetCellOwnerTeamColor(int a)
{
    int i = a & 0xE0;

    if (i == 0)
        return 0;

    return gPlayers[i >> 5].teamColor;
}
asm(".global sub_080249C8\n.thumb_set sub_080249C8, GetCellOwnerTeamColor\n");

int GetTerrainDefense(int a1, s8 a2, u8 a3)
{
    if (gUnknown_085D5ABC[a3].deployLocation == 0x10)
        return 0;

    return (s8)(gUnknown_085D583C[a2].defense * 10);
}

asm(".global sub_080249EC\n.thumb_set sub_080249EC, GetTerrainDefense\n");

void InitBattleUnit(struct BattleUnit *a1, s16 a2)
{
    struct Unit *e;
    struct Map *map;
    int idx;
    int t;

    e = &gUnits[a2];
    a1->unit = e;

    map = gMap;
    idx = map->rowOffset[e->y] + e->x;
    t = map->terrain[idx] & 0x1f;

    a1->terrainId = t;
    a1->terrainDefense = (s8)GetTerrainDefense((u16)(((e - gUnits) >> 6) + 1),
                                 t, e->type);
    a1->remainingHp = a1->unit->hp;
    a1->ammo = a1->unit->ammo;
    a1->attackType = 0;
    a1->baseDamage = 0;
    a1->hpLoss = 0;
}
asm(".global sub_08024A2C\n.thumb_set sub_08024A2C, InitBattleUnit\n");
