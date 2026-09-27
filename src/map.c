#include "global.h"
#include "hardware.h"
#include "map.h"

void LoadMapData(u16 a1)
{
    void *p;

    if (a1 >= 0xb4 && a1 <= 0xbf)
    {
        sub_08037B84(sub_08014E44(0x724));
        sub_0801AC58(8, (u8 *)gUnknown_03003F68);
    }
    else
    {
        sub_08037B84(sub_08014E44(0xa14));

        p = gUnknown_085C77A0[a1].mapData[IsHardCampaignMode()];
        if (p == NULL)
            p = gUnknown_085C77A0[a1].mapData[0];

        LZ77UnCompWram(p, gUnknown_03003F68);
    }
}

asm(".global sub_080247A4\n.thumb_set sub_080247A4, LoadMapData\n");

void WarRoomScroll_CB_0802481D(void)
{
    sub_08014ED4(gUnknown_03003F68);
}

asm(".global sub_0802481C\n.thumb_set sub_0802481C, WarRoomScroll_CB_0802481D\n");

void sub_08024830(void)
{
    ApplyPaletteExt((u16 *)(gUnknown_0810E6E0 + (gPlayers[1].teamColor - 1) * 0x20),
                    0x180, 0x20);
    ApplyPaletteExt((u16 *)(gUnknown_0810E6E0 + (gPlayers[2].teamColor - 1) * 0x20),
                    0x1A0, 0x20);
    ApplyPaletteExt((u16 *)(gUnknown_0810E6E0 + (gPlayers[3].teamColor - 1) * 0x20),
                    0x1C0, 0x20);
    ApplyPaletteExt((u16 *)(gUnknown_0810E6E0 + (gPlayers[4].teamColor - 1) * 0x20),
                    0x1E0, 0x20);

    sub_0803F80C(8);
    sub_0802D2EC();

    ApplyPaletteExt(gUnknown_0809163C, 0x240, 0x20);

    sub_08035020(gPlaySt.weather);
    sub_08022A34();

    sub_0801A5B0(gUnknown_030033EC);
    sub_08043834(gUnknown_030033EC);
    sub_0801A57C(gUnknown_030033EC);
}

u8 *sub_080248E4(void)
{
    return gMap->unk421a;
}

u8 sub_080248F8(void)
{
    return gMap->unk4233;
}

u8 sub_0802490C(u16 a1)
{
    if ((u16)(a1 - 0xB4) <= 0xB)
        return sub_0803CD14(a1 + 0x4C);

    return gUnknown_085C77A0[a1].unk18;
}

u8 *sub_08024944(u16 a1)
{
    if ((u16)(a1 - 0xB4) <= 0xB)
        return sub_0803CCEC(a1 + 0x4C);

    return gTextTable[gUnknown_085C77A0[a1].nameIndex];
}

int sub_08024984(int a1)
{
    int r = gUnknown_085C77A0[gPlaySt.mapID].unk58;

    if (r == 0 || (a1 & 0x1F) == 8)
    {
        int i = a1 & 0xE0;

        if (i != 0)
            r = sub_08042DE0(i >> 5);
        else
            r = 0;
    }

    return r;
}

int sub_080249C8(int a)
{
    int i = a & 0xE0;

    if (i == 0)
        return 0;

    return gPlayers[i >> 5].teamColor;
}

int GetTerrainDefense(int a1, s8 a2, u8 a3)
{
    if (gUnknown_085D5ABC[a3].deployLocation == 0x10)
        return 0;

    return (s8)(gUnknown_085D583C[a2].defense * 10);
}

asm(".global sub_080249EC\n.thumb_set sub_080249EC, GetTerrainDefense\n");

void sub_08024A2C(struct BattleUnit *a1, s16 a2)
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
