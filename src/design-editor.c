#include "global.h"
#include "hardware.h"
#include "map.h"


void sub_08003640(void)
{
    u8 v[4];
    int i;

    sub_0808B6E8(v, gUnknown_0808D77C, 4);
    for (i = 0; i < 4; i++)
    {
        RegisterDataMove((void *)(GetTerrainPictureGraphic(v[i], 0) + 0x40),
                     (void *)(gUnknown_08485C9C[i + 1] * 32 + 0x06010000), 0xC0);
        RegisterDataMove(gUnknown_08485A2C,
                     (void *)(gUnknown_08485C9C[i + 1] * 32 + 0x060100C0), 0x40);
    }
}

void sub_080036A4(void)
{
    DrawOamObject(0x30, 0x19, 0x418, 0, 0);
    DrawOamObject(0x2F, 0x19, 0x428, 0, 0);
    DrawOamObject(0x02, 0x19, 0x438, 0, 0);
    DrawOamObject(0x34, 0x18, 0x448, 0, 0);
    DrawOamObject(0x0C, 0x19, 0x458, 0, 0);
}

void sub_08003704(void)
{
    InitTilePool(0, (void *)0x06010000, 0x28D, 0x1D);
    LoadTilePoolGraphic(0x02);
    LoadTilePoolGraphic(0x0C);
    LoadTilePoolGraphic(0x2F);
    LoadTilePoolGraphic(0x30);
    LoadTilePoolGraphic(0x31);
    LoadTilePoolGraphic(0x32);
    LoadTilePoolGraphic(0x33);
    LoadTilePoolGraphic(0x34);
    LoadTilePoolGraphic(0x35);
    LoadTilePoolGraphic(0x36);
    LoadTilePoolGraphic(0x37);
    LoadTilePoolGraphic(0x38);
    LoadTilePoolGraphic(0x3C);
}

void sub_0800376C(void)
{
    DrawOamObject(0x31, 0x18, 0x420, 0, 0);
    DrawOamObject(0x32, 0x18, 0x430, 0, 0);
    DrawOamObject(0x33, 0x18, 0x440, 0, 0);
}

void sub_080037AC(void)
{
    InitTilePool(0, (void *)0x06010000, 0x28D, 0x1D);
    LoadTilePoolGraphic(0x02);
    LoadTilePoolGraphic(0x0C);
    LoadTilePoolGraphic(0x2F);
    LoadTilePoolGraphic(0x30);
    LoadTilePoolGraphic(0x31);
    LoadTilePoolGraphic(0x32);
    LoadTilePoolGraphic(0x33);
    LoadTilePoolGraphic(0x34);
    LoadTilePoolGraphic(0x35);
    LoadTilePoolGraphic(0x36);
    LoadTilePoolGraphic(0x37);
    LoadTilePoolGraphic(0x38);
    LoadTilePoolGraphic(0x3C);
}

void DesignRoomDrawCoordBox(void)
{
    int x;
    int t;

    x = gActiveMap->panelSide == 0 ? 2 : 0xCE;
    t = x & 0x1FF;
    DrawOamObject(0x91, t, 0xE, 0, 0);
    DrawOamObject(0x90, t, 0x18, 0, 0);
    t = (x + 0x18) & 0x1FF;
    DrawSpriteNumberFont2(t, 0xE, gActiveMap->cursorX + 1);
    DrawSpriteNumberFont2(t, 0x18, gActiveMap->cursorY + 1);
}
asm(".global sub_08003814\n.thumb_set sub_08003814, DesignRoomDrawCoordBox\n");

void DesignRoomUpdateCoordBox(void)
{
    s16 * q;
    int v;
    int t;

    if (gActiveMap->flags & 8)
    {
        q = &gActiveMap->introScreenY;
        v = *q;
        v = v >= -2 ? -2 : v + 1;
    }
    else
    {
        q = &gActiveMap->introScreenY;
        v = *q;
        v = v < -10 ? -10 : v - 1;
    }

    *q = v;

    if (v > -3)
    {
        t = gActiveMap->panelSide == 0 ? 0x20 : 0xD0;
        DrawOamObject(0x8E, (t - 0x20) & 0x1FF,
                     (u8)gActiveMap->introScreenY, 0, 0);
        DesignRoomDrawCoordBox();
    }
}
asm(".global sub_08003890\n.thumb_set sub_08003890, DesignRoomUpdateCoordBox\n");

void DesignRoomStartCoordBox(void)
{
    sub_080152EC(gUnknown_08485D8C, 0);
    gActiveMap->introScreenY = 0xFFF6;
}
asm(".global sub_08003910\n.thumb_set sub_08003910, DesignRoomStartCoordBox\n");

void DesignRoomShowCoordBox(void)
{
    gActiveMap->flags |= 8;
}
asm(".global sub_08003934\n.thumb_set sub_08003934, DesignRoomShowCoordBox\n");

void DesignRoomHideCoordBox(void)
{
    gActiveMap->flags &= ~8;
}
asm(".global sub_08003948\n.thumb_set sub_08003948, DesignRoomHideCoordBox\n");

void sub_08003960(void)
{
    sub_080039E4();
}

void sub_0800396C(void)
{
    int v = gUnknown_03001FFC + 1;

    if (v > 6)
    {
        v = 6;
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }

    gUnknown_03001FFC = v;
}

void sub_08003994(void)
{
    int v = gUnknown_03001FFC - 1;

    if (v <= 0)
    {
        v = 0;
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }

    gUnknown_03001FFC = v;
}

void sub_080039BC(void)
{
    sub_080152EC(gUnknown_08485DB4, 0);
}

void sub_080039D0(void)
{
    sub_080152EC(gUnknown_08485D9C, 0);
}

void sub_080039E4(void)
{
    SetDefaultColorEffects();

    gUnknown_030030E0.bits.effect = 3;
    gUnknown_030030E0.bits.target1_enable_bg0 = 0;
    gUnknown_030030E0.bits.target1_enable_bg1 = 1;
    gUnknown_030030E0.bits.target1_enable_bg2 = 1;
    gUnknown_030030E0.bits.target1_enable_bg3 = 1;
    gUnknown_030030E0.bits.target1_enable_obj = 1;
    gUnknown_030030E0.bits.target2_enable_bg0 = 0;
    gUnknown_030030E0.bits.target2_enable_bg1 = 0;
    gUnknown_030030E0.bits.target2_enable_bg2 = 0;
    gUnknown_030030E0.bits.target2_enable_bg3 = 0;
    gUnknown_030030E0.bits.target2_enable_obj = 0;

    gUnknown_03001FFC = 0;
    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = 0;

    gDispIo.disp_ct.win0_enable = 0;
    gDispIo.disp_ct.win1_enable = 0;

    gUnknown_03002B40 = 0;
    gUnknown_03002B4C = 0;
    gUnknown_03002EFC = 0xA0;
    gUnknown_03002B44 = 0xA0;
}

void sub_08003A80(int a1, int a2, int a3, int a4)
{
    u16 v;

    SetDefaultColorEffects();

    gUnknown_030030E0.bits.effect = 3;
    gUnknown_030030E0.bits.target1_enable_bg1 = 0;
    gUnknown_030030E0.bits.target1_enable_bg2 = 0;
    gUnknown_030030E0.bits.target1_enable_bg3 = 1;
    gUnknown_030030E0.bits.target1_enable_obj = 1;
    gUnknown_030030E0.bits.target2_enable_bg1 = 1;
    gUnknown_030030E0.bits.target2_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;

    gUnknown_03001FFC = v = 0;
    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = v;

    gDispIo.disp_ct.win0_enable = 1;

    gUnknown_03002B40 = a1;
    gUnknown_03002B4C = a2;
    gUnknown_03002EFC = a3;
    gUnknown_03002B44 = a4;

    gUnknown_030030A4.bits.win0_enable_bg0 = 1;
    gUnknown_030030A4.bits.win0_enable_bg1 = 1;
    gUnknown_030030A4.bits.win0_enable_bg2 = 1;
    gUnknown_030030A4.bits.win0_enable_bg3 = 1;
    gUnknown_030030A4.bits.win0_enable_obj = 1;
    gUnknown_030030A4.bits.win0_enable_blend = 1;

    gUnknown_030030DC.raw = v;
    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 0;
}

int RandRange(int a, int b)
{
    return (DivRem(GetNextRandomNumber(), (a - b) * 32) >> 5) + b;
}

asm(".global sub_08003B6C\n.thumb_set sub_08003B6C, RandRange\n");

void GenerateRandomMap(void)
{
    int i;
    int keys;

    ClearAllUnits();
    gActiveMap->army1UnitCount = 0;
    gMap->width = 0x1E;
    gMap->height = 0x14;
    gMap->unk10 = 0;
    for (i = 0; i < gMap->height; i++)
        gMap->rowOffset[i] = i * gMap->width;

    keys = gpKeySt->held & (R_BUTTON | L_BUTTON);
    if (keys == (R_BUTTON | L_BUTTON))
    {
        sub_08004724();
        FixAllShorelines();
    }
    else if (keys == L_BUTTON)
    {
        sub_080040C8();
        FixAllShorelines();
    }
    else if (keys == R_BUTTON)
    {
        sub_0800449C();
        FixAllShorelines();
    }
    else if (DivRem(GetNextRandomNumber(), 100) > 0x1D)
    {
        sub_080040C8();
        FixAllShorelines();
    }
    else
    {
        sub_0800449C();
        FixAllShorelines();
    }

    RecountArmyProperties();
    RegisterArmyHqs();
    gActiveMap->propertyCount = CountProperties();
}

asm(".global sub_08003B8C\n.thumb_set sub_08003B8C, GenerateRandomMap\n");

void DesignRoomNewMap(int a1)
{
    int x, y;

    if (a1 < 0)
    {
        GenerateRandomMap();
        return;
    }

    ClearAllUnits();
    gActiveMap->army1UnitCount = 0;

    for (x = 0; x < gMap->height; x++)
        gMap->rowOffset[x] = x * gMap->width;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            switch (a1)
            {
            case 7:
                gMap->tile[gMap->rowOffset[y] + x] = 0x2A;
                gMap->terrain[gMap->rowOffset[y] + x] = a1;
                break;
            case 1:
                gMap->tile[gMap->rowOffset[y] + x] = a1;
                gMap->terrain[gMap->rowOffset[y] + x] = 1;
                break;
            case 3:
                SetTerrainAt(x, y, 3);
                MakeMountain(x, y);
                break;
            case 4:
                gMap->tile[gMap->rowOffset[y] + x] = 0x87;
                gMap->terrain[gMap->rowOffset[y] + x] = a1;
                break;
            }
        }
    }

    RecountArmyProperties();
    RegisterArmyHqs();
    gActiveMap->propertyCount = CountProperties();
}
asm(".global sub_08003C48\n.thumb_set sub_08003C48, DesignRoomNewMap\n");

void FixShorelineAt(int x, int y, int kind)
{
    switch (kind)
    {
    case 7:
        if (x < gMap->width - 1)
        {
            int idx;

            idx = gMap->rowOffset[y];
            idx++;
            idx += x;
            if (gMap->terrain[idx] == 1 || gMap->terrain[idx] == 0xD)
                RepaintNeighbours(x + 1, y);
        }
        if (x > 0)
        {
            int idx;

            idx = gMap->rowOffset[y];
            idx--;
            idx += x;
            if (gMap->terrain[idx] == 1 || gMap->terrain[idx] == 0xD)
                RepaintNeighbours(x - 1, y);
        }
        break;

    case 1:
        if (x & 1)
        {
            if (y > 0)
            {
                if (gMap->terrain[gMap->rowOffset[y - 1] + x] == 7
                 || gMap->terrain[gMap->rowOffset[y - 1] + x] == 0xD)
                    RepaintNeighbours(x, y);
            }
            if (y < gMap->height - 1)
            {
                if (gMap->terrain[gMap->rowOffset[y + 1] + x] == 7
                 || gMap->terrain[gMap->rowOffset[y + 1] + x] == 0xD)
                    RepaintNeighbours(x, y);
            }
        }
        break;

    case 3:
        sub_0800AF74(x, y);
        break;
    }
}

asm(".global sub_08003DC4\n.thumb_set sub_08003DC4, FixShorelineAt\n");

void FixAllShorelines(void)
{
    int x, y;

    for (y = 0; y < gMap->height; y++)
        for (x = 0; x < gMap->width; x++)
            FixShorelineAt(x, y, gMap->terrain[gMap->rowOffset[y] + x]);
}

asm(".global sub_08003ED0\n.thumb_set sub_08003ED0, FixAllShorelines\n");

void sub_08003F44(int x, int y, int v)
{
    if (x > 0x1D)
        x = 0x1D;
    if (y > 0x13)
        y = 0x13;

    gMap->tile[gMap->rowOffset[y] + x] = v;

    switch (v)
    {
    case 1:
        gMap->terrain[gMap->rowOffset[y] + x] = v;
        break;
    case 0x20:
        gMap->terrain[gMap->rowOffset[y] + x] = 3;
        break;
    case 0x87:
        if (gMap->terrain[gMap->rowOffset[y] + x] != 4)
        {
            gMap->terrain[gMap->rowOffset[y] + x] = 4;
            gMap->tile[gMap->rowOffset[y] + x] = v;
            MakeForest(x, y);
        }
        break;
    case 0x2A:
        MakeSeaSafest(x, y);
        break;
    }
}

void StampTerrainBlob(int cx, int cy, int w, int h, int t)
{
    register int hh asm("r9") = h;
    register int tt asm("r10") = t;
    register int rx asm("r4");
    int y;
    int x;
    int ry;
    int ny;
    y = cy - (hh >> 1);
    if (y < 0)
        y = 0;
    for (ry = hh; ry > 0; ry--)
    {
        x = cx - (w >> 1);
        if (x < 0)
            x = 0;
        rx = w;
        ny = y + 1;
        for (; rx > 0; rx--)
        {
            if (w <= 3 && hh <= 3)
            {
                if (w == 3)
                    sub_08003F44(x, y, tt);
            }
            else if (((ry == hh) || (ry == 1)) && ((rx == w) || (rx == 1)))
            {
            }
            else
            {
                sub_08003F44(x, y, tt);
            }
            x++;
            {
                register struct Map *map asm("r0") = gMap;
                if (x >= map->width)
                    break;
            }
        }
        y = ny;
        {
            register struct Map *map asm("r0") = gMap;
            if (y >= map->height)
                break;
        }
    }
}
asm(".global sub_0800401C\n.thumb_set sub_0800401C, StampTerrainBlob\n");

void sub_080040C8(void)
{
    int x, y;
    int i, n;
    int lim, cnt;
    int a, b, w, h;
    int t;
    int r;

    t = 7;
    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            gMap->tile[gMap->rowOffset[y] + x] = 0x2A;
            gMap->terrain[gMap->rowOffset[y] + x] = t;
        }
    }

    n = RandRange(0xC, 8);
    for (i = n; i > 0; i--)
    {
        a = RandRange(0x1C, 2);
        b = RandRange(0x12, 2);
        w = RandRange(0xA, 2);
        h = RandRange(0xC - w, 2);
        StampTerrainBlob(a, b, w, h, 1);
    }

    n = RandRange(0xC, 8);
    for (i = n; i > 0; i--)
    {
        a = RandRange(0x1C, 2);
        b = RandRange(0x12, 2);
        w = RandRange(0xA, 4);
        h = RandRange(0xE - w, 4);
        StampTerrainBlob(a, b, w, h, 1);
    }

    lim = RandRange(0x32, 0x28);
    cnt = 0;
    for (i = 100; i > 0; i--)
    {
        a = RandRange(0x1A, 4);
        b = RandRange(0x10, 4);
        if (gMap->terrain[gMap->rowOffset[b] + a] == 1)
        {
            SetTerrainAt(a, b, 3);
            MakeTile2(a, b, 0x20);
            cnt++;
        }
        if (cnt > lim)
            break;
    }

    n = RandRange(6, 3);
    for (i = n; i > 0; i--)
    {
        a = RandRange(0x1C, 2);
        b = RandRange(0x12, 2);
        w = RandRange(4, 1);
        h = RandRange(5 - w, 1);
        StampTerrainBlob(a, b, w, h, 0x20);
    }

    n = 7 - n;
    for (i = n; i > 0; i--)
    {
        a = RandRange(0x1C, 2);
        b = RandRange(0x12, 2);
        w = RandRange(4, 2);
        h = RandRange(6 - w, 2);
        StampTerrainBlob(a, b, w, h, 0x20);
    }

    lim = RandRange(0x32, 0x28);
    cnt = 0;
    for (i = 0x32; i > 0; i--)
    {
        a = RandRange(0x1A, 4);
        b = RandRange(0x10, 4);
        if (gMap->terrain[gMap->rowOffset[b] + a] == 1)
        {
            SetTerrainAt(a, b, 4);
            MakeTile2(a, b, 0x87);
            cnt++;
        }
        if (cnt > lim)
            break;
    }

    lim = RandRange(0x32, 0x28);
    cnt = 0;
    for (i = 100; i > 0; i--)
    {
        a = RandRange(0x1C, 2);
        b = RandRange(0x12, 2);
        if (sub_0800BCD0(a, b) != 0)
        {
            SetTerrainAt(a, b, 0x13);
            MakeTile2(a, b, 0x168);
            cnt++;
        }
        if (cnt > lim)
            break;
    }

    for (y = 1; y < gMap->height - 1; y++)
    {
        for (x = n = 1; x < gMap->width - 1; x++)
        {
            if (RandRange(0xC8, 0x64) > 0x95
             && gMap->terrain[gMap->rowOffset[y] + x] == 1)
            {
                if (gMap->terrain[gMap->rowOffset[y - 1] + x] == 7)
                    MakeShoal(x, y - 1);
                if (gMap->terrain[gMap->rowOffset[y + 1] + x] == 7)
                    MakeShoal(x, y + 1);
                r = gMap->rowOffset[y] - 1;
                if (gMap->terrain[r + x] == 7)
                    MakeShoal(x - 1, y);
                r = gMap->rowOffset[y] + 1;
                if (gMap->terrain[r + x] == 7)
                    MakeShoal(x + 1, y);
            }
        }
    }
}

void sub_0800449C(void)
{
    int x, y;
    int i, n;
    int lim, cnt;
    int a, b, w, h;
    int t;

    t = 7;
    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            gMap->tile[gMap->rowOffset[y] + x] = 0x2A;
            gMap->terrain[gMap->rowOffset[y] + x] = t;
        }
    }

    for (i = RandRange(0x1E, 0x19); i > 0; i--)
    {
        a = RandRange(0x1E, 0);
        b = RandRange(0x1E, 0);
        w = RandRange(0xA, 6);
        h = RandRange(0x10 - w, 6);
        StampTerrainBlob(a, b, w, h, 1);
    }

    for (i = RandRange(0x1E, 0x19); i > 0; i--)
    {
        a = RandRange(0x1E, 0);
        b = RandRange(0x1E, 0);
        w = RandRange(0xC, 4);
        h = RandRange(0x10 - w, 4);
        StampTerrainBlob(a, b, w, h, 1);
    }

    lim = RandRange(100, 90);
    cnt = 0;
    for (i = 100; i > 0; i--)
    {
        a = RandRange(0x1D, 0);
        b = RandRange(0x13, 0);
        if (gMap->terrain[gMap->rowOffset[b] + a] == 1)
        {
            SetTerrainAt(a, b, 3);
            MakeTile2(a, b, 0x20);
            cnt++;
        }
        if (cnt > lim)
            break;
    }

    n = RandRange(0xA, 7);
    for (i = n; i > 0; i--)
    {
        a = RandRange(0x1C, 2);
        b = RandRange(0x12, 2);
        w = RandRange(4, 2);
        h = RandRange(6 - w, 2);
        StampTerrainBlob(a, b, w, h, 0x20);
    }

    for (i = 12 - n; i > 0; i--)
    {
        a = RandRange(0x1C, 2);
        b = RandRange(0x12, 2);
        w = RandRange(4, 2);
        h = RandRange(6 - w, 2);
        StampTerrainBlob(a, b, w, h, 0x20);
    }

    lim = RandRange(100, 0x50);
    cnt = 0;
    for (i = 100; i > 0; i--)
    {
        a = RandRange(0x1A, 4);
        b = RandRange(0x10, 4);
        if (gMap->terrain[gMap->rowOffset[b] + a] == 1)
        {
            SetTerrainAt(a, b, 4);
            MakeTile2(a, b, 0x87);
            cnt++;
        }
        if (cnt > lim)
            break;
    }
}
