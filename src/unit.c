#include "global.h"
#include "proc.h"
#include "map.h"

void sub_08042B70(void)
{
    sub_080152EC(gUnknown_0849A0F0, 0);
}

void sub_08042B84(void)
{
    struct Unk03001470 *p = sub_080152EC(gUnknown_0849A0F0, 0);

    p->unk20 = 1;
}

void EndCurrentArmyTurn(void)
{
    u8 *q;

    gPlaySt.unk2e = sub_08035170();
    q = gUnknown_030044B0;
    *(u32 *)(q + 8) = gUnknown_03001FD4;

    if (gPlaySt.savingEnabled == 0)
    {
        gPlayers[gUnknown_030033EC].cursorX = gUnknown_030033E4.unk00;
        gPlayers[gUnknown_030033EC].cursorY = gUnknown_030033E4.unk02;
    }

    gUnknown_030032D8 = 1;
    ReadyCurrentArmyUnits();
}
asm(".global sub_08042B9C\n.thumb_set sub_08042B9C, EndCurrentArmyTurn\n");

void StartSiloTargetSelect(void)
{
    Proc_Start(gUnknown_0849FC0C, PROC_TREE_3);
}
asm(".global sub_08042C10\n.thumb_set sub_08042C10, StartSiloTargetSelect\n");

struct Unk08042C24Proc
{
    STRUCT_PAD(0x00, 0x2c);
    int unk2c;
    int unk30;
    STRUCT_PAD(0x34, 0x4a);
    u16 unk4a;
    STRUCT_PAD(0x4c, 0x64);
    u16 unk64;
    u16 unk66;
};

void StartSiloFire(int a, int b, int c, int d, ProcPtr parent)
{
    struct Unk08042C24Proc *proc;

    if ((int)parent <= 7)
        proc = Proc_Start(gUnknown_0849FCA4, parent);
    else
        proc = Proc_StartBlocking(gUnknown_0849FCA4, parent);

    proc->unk64 = a;
    proc->unk66 = b;
    proc->unk2c = c;
    proc->unk30 = d;
    proc->unk4a = 0x1e;
}
asm(".global sub_08042C24\n.thumb_set sub_08042C24, StartSiloFire\n");

int GetCoDailyFuelBurnBonus(int a, int b)
{
    int lo;
    int hi;

    hi = 3;
    lo = 2;

    if ((GetPlayerSpecialAbilities(a) & 0x80) != 0
        && gUnknown_085D5ABC[b].unitClass <= hi
        && gUnknown_085D5ABC[b].unitClass >= lo)
        return -2;

    return 0;
}
asm(".global sub_08042C68\n.thumb_set sub_08042C68, GetCoDailyFuelBurnBonus\n");

int GetUnitCostWithCoBonus(int a, int b)
{
    return Div(GetUnitBaseCost(b)
        * (GetCoCostBonus(gPlayers[a].co, gPlayers[a].coMode, b)
            + 0x64), 100);
}

int GetUnitAttackWithCoBonus(int a, int b)
{
    return GetCoAttackBonus(gPlayers[a].co, gPlayers[a].coMode, b)
        + 0x64;
}

int GetUnitDefenceWithCoBonus(int a, int b)
{
    return GetCoDefenceBonus(gPlayers[a].co, gPlayers[a].coMode, b)
        + 0x64;
}

int GetUnitMovementWithCoBonus(int a, int b)
{
    return GetUnitBaseMovement(b)
        + GetCoMovementBonus(gPlayers[a].co, gPlayers[a].coMode, b);
}

int GetUnitFiringRangeWithCoBonus(int a, int b)
{
    return GetUnitBaseFiringRange(b)
        + GetCoRangeBonus(gPlayers[a].co, gPlayers[a].coMode, b);
}

int GetUnitVisionWithCoBonus(int a, int b)
{
    int n;

    n = GetUnitBaseVision(b)
        + GetCoVisionBonus(gPlayers[a].co, gPlayers[a].coMode);

    if (gPlaySt.weather == 2)
        n--;

    if (n > 1)
        return n;
    else
        return 1;
}

asm(".global sub_08042C9C\n.thumb_set sub_08042C9C, GetUnitCostWithCoBonus\n"
    ".global sub_08042CD4\n.thumb_set sub_08042CD4, GetUnitAttackWithCoBonus\n"
    ".global sub_08042CF8\n.thumb_set sub_08042CF8, GetUnitDefenceWithCoBonus\n"
    ".global sub_08042D1C\n.thumb_set sub_08042D1C, GetUnitMovementWithCoBonus\n"
    ".global sub_08042D50\n.thumb_set sub_08042D50, GetUnitFiringRangeWithCoBonus\n"
    ".global sub_08042D84\n.thumb_set sub_08042D84, GetUnitVisionWithCoBonus\n");

int GetCoCountry(int a1)
{
    return gUnknown_085D3DD0[a1].unk15 + 1;
}
asm(".global sub_08042DCC\n.thumb_set sub_08042DCC, GetCoCountry\n");

int GetPlayerCoCountry(int a1)
{
    return GetCoCountry(gPlayers[a1].co);
}
asm(".global sub_08042DE0\n.thumb_set sub_08042DE0, GetPlayerCoCountry\n");

int GetPlayerCoDefaultTeamColor(int a1)
{
    return GetCoDefaultTeamColor(gPlayers[a1].co);
}
asm(".global sub_08042DFC\n.thumb_set sub_08042DFC, GetPlayerCoDefaultTeamColor\n");

int GetCoDefaultTeamColor(int a)
{
    return gUnknown_085D3DD0[a].unk16;
}
asm(".global sub_08042E18\n.thumb_set sub_08042E18, GetCoDefaultTeamColor\n");

int GetCoLuckBonus(int a, int b)
{
    if (gPlaySt.coAbilities == 0)
        return 10;

    return gUnknown_085D3DD0[a].power[b].luckPositive;
}

asm(".global sub_08042E2C\n.thumb_set sub_08042E2C, GetCoLuckBonus\n");

int GetPlayerCoLuckBonus(int a1)
{
    return GetCoLuckBonus(gPlayers[a1].co, gPlayers[a1].coMode);
}

asm(".global sub_08042E64\n.thumb_set sub_08042E64, GetPlayerCoLuckBonus\n");

int GetCoNegativeLuckBonus(int a, int b)
{
    if (gPlaySt.coAbilities == 0)
        return 0;

    return gUnknown_085D3DD0[a].power[b].luckNegative;
}

asm(".global sub_08042E84\n.thumb_set sub_08042E84, GetCoNegativeLuckBonus\n");

int GetPlayerCoNegativeLuckBonus(int a1)
{
    return GetCoNegativeLuckBonus(gPlayers[a1].co, gPlayers[a1].coMode);
}

asm(".global sub_08042EBC\n.thumb_set sub_08042EBC, GetPlayerCoNegativeLuckBonus\n");

int GetCoCaptureRate(int a, int b)
{
    if (gPlaySt.coAbilities == 0)
        return 100;

    return gUnknown_085D3DD0[a].power[b].captureRateModifier + 100;
}

asm(".global sub_08042EDC\n.thumb_set sub_08042EDC, GetCoCaptureRate\n");

int GetPlayerCoCaptureRate(int a1)
{
    return GetCoCaptureRate(gPlayers[a1].co, gPlayers[a1].coMode);
}
asm(".global sub_08042F14\n.thumb_set sub_08042F14, GetPlayerCoCaptureRate\n");

int GetCoRainBringerPercent(int a, int b)
{
    if (gPlaySt.coAbilities == 0)
        return 0;

    return gUnknown_085D3DD0[a].rainBringerPercent;
}
asm(".global sub_08042F34\n.thumb_set sub_08042F34, GetCoRainBringerPercent\n");

int GetPlayerCoRainBringerPercent(int a1)
{
    return GetCoRainBringerPercent(gPlayers[a1].co, gPlayers[a1].coMode);
}
asm(".global sub_08042F5C\n.thumb_set sub_08042F5C, GetPlayerCoRainBringerPercent\n");

int GetCoSnowBringerPercent(int a, int b)
{
    if (gPlaySt.coAbilities == 0)
        return 0;

    return gUnknown_085D3DD0[a].snowBringerPercent;
}
asm(".global sub_08042F7C\n.thumb_set sub_08042F7C, GetCoSnowBringerPercent\n");

int GetPlayerCoSnowBringerPercent(int a1)
{
    return GetCoSnowBringerPercent(gPlayers[a1].co, gPlayers[a1].coMode);
}
asm(".global sub_08042FA4\n.thumb_set sub_08042FA4, GetPlayerCoSnowBringerPercent\n");

int GetCoCounterattackBonus(int a, int b)
{
    if (gPlaySt.coAbilities == 0)
        return 0;

    return gUnknown_085D3DD0[a].power[b].counterMultiplier;
}

asm(".global sub_08042FC4\n.thumb_set sub_08042FC4, GetCoCounterattackBonus\n");

int GetPlayerCoCounterattackBonus(int a1)
{
    return GetCoCounterattackBonus(gPlayers[a1].co, gPlayers[a1].coMode);
}

asm(".global sub_08042FFC\n.thumb_set sub_08042FFC, GetPlayerCoCounterattackBonus\n");

u32 GetCoSpecialAbilities(int a, int b)
{
    if (gPlaySt.coAbilities == 0)
        return 0;

    return gUnknown_085D3DD0[a].power[b].specialAbilities;
}
asm(".global sub_0804301C\n.thumb_set sub_0804301C, GetCoSpecialAbilities\n");

u32 GetPlayerSpecialAbilities(int a1)
{
    return GetCoSpecialAbilities(gPlayers[a1].co, gPlayers[a1].coMode);
}

asm(".global sub_08043050\n.thumb_set sub_08043050, GetPlayerSpecialAbilities\n");

int GetCoAdjustedBaseDamage(int a1, int a2, int a3, int a4, int a5)
{
    int v = gUnknown_085D5ABC[a3].baseDamage[a5][a4];
    int base = GetCoAttackBonus(a1, a2, a3) + 100;
    int r;

    if (v > 0)
    {
        r = Div(v * base, 100);
        if (r == 0)
            r = 1;
    }
    else
    {
        r = 0;
    }

    return r;
}
asm(".global sub_08043070\n.thumb_set sub_08043070, GetCoAdjustedBaseDamage\n");

int GetCoAttackBonus(int a, int b, int c)
{
    s16 *p;
    s16 *q;

    p = gUnknown_085D3DD0[a].power[b].unk24[gUnknown_085D5ABC[c].unitClass];
    q = gUnknown_085D3DD0[a].power[b].unk24[GetUnitCombatClassColumn(c)];

    if (gPlaySt.coAbilities == 0)
        return 0;

    if (gUnknown_085D5ABC[c].unitClass == 0)
        return p[0];

    return p[0] + q[0];
}

int GetCoDefenceBonus(int a, int b, int c)
{
    s16 *p;
    s16 *q;

    p = gUnknown_085D3DD0[a].power[b].unk24[gUnknown_085D5ABC[c].unitClass];
    q = gUnknown_085D3DD0[a].power[b].unk24[GetUnitCombatClassColumn(c)];

    if (gPlaySt.coAbilities == 0)
        return 0;

    if (gUnknown_085D5ABC[c].unitClass == 0)
        return p[1];

    return p[1] + q[1];
}

int GetCoMovementBonus(int a, int b, int c)
{
    s16 *p;
    s16 *q;

    p = gUnknown_085D3DD0[a].power[b].unk24[gUnknown_085D5ABC[c].unitClass];
    q = gUnknown_085D3DD0[a].power[b].unk24[GetUnitCombatClassColumn(c)];

    if (gPlaySt.coAbilities == 0)
        return 0;

    if (gUnknown_085D5ABC[c].unitClass == 0)
        return p[2];

    return p[2] + q[2];
}

int GetCoRangeBonus(int a, int b, int c)
{
    s16 *p;
    s16 *q;

    p = gUnknown_085D3DD0[a].power[b].unk24[gUnknown_085D5ABC[c].unitClass];
    q = gUnknown_085D3DD0[a].power[b].unk24[GetUnitCombatClassColumn(c)];

    if (gPlaySt.coAbilities == 0)
        return 0;

    if (gUnknown_085D5ABC[c].unitClass == 0)
        return p[3];

    return p[3] + q[3];
}

asm(".global sub_080430B0\n.thumb_set sub_080430B0, GetCoAttackBonus\n"
    ".global sub_08043120\n.thumb_set sub_08043120, GetCoDefenceBonus\n"
    ".global sub_08043190\n.thumb_set sub_08043190, GetCoMovementBonus\n"
    ".global sub_08043200\n.thumb_set sub_08043200, GetCoRangeBonus\n");

int GetCoCostBonus(int a, int b, int c)
{
    if (gPlaySt.coAbilities == 0)
        return 0;

    return gUnknown_085D3DD0[a].power[b].unitCostModifier;
}

int GetCoVisionBonus(int a, int b)
{
    if (gPlaySt.coAbilities == 0)
        return 0;

    return gUnknown_085D3DD0[a].power[b].visionBonus;
}

asm(".global sub_08043270\n.thumb_set sub_08043270, GetCoCostBonus\n"
    ".global sub_080432A8\n.thumb_set sub_080432A8, GetCoVisionBonus\n");

int GetUnitCombatClassColumn(int a)
{
    u32 v = gUnknown_085D5ABC[a].minRange;

    if (v > 1)
        return 6;

    if (v == 1)
        return 5;

    return 7;
}

asm(".global sub_080432E0\n.thumb_set sub_080432E0, GetUnitCombatClassColumn\n");

int GetBattleUnitTerrainDefense(struct BattleUnit *p)
{
    if ((GetPlayerSpecialAbilities(((p->unit - gUnits) >> 6) + 1) & 0x20) == 0)
        return p->terrainDefense;

    return p->terrainDefense * 2;
}
asm(".global sub_08043304\n.thumb_set sub_08043304, GetBattleUnitTerrainDefense\n");

int GetBattleUnitTerrainFirepowerBonus(struct BattleUnit *p)
{
    int r;

    if (GetPlayerSpecialAbilities(((p->unit - gUnits) >> 6) + 1) & 0x40)
        r = p->terrainDefense;
    else
        r = 0;

    return r;
}
asm(".global sub_0804334C\n.thumb_set sub_0804334C, GetBattleUnitTerrainFirepowerBonus\n");

int GetBattleUnitCounterattackBonus(struct BattleUnit *p)
{
    return GetPlayerCoCounterattackBonus(((p->unit - gUnits) >> 6) + 1);
}
asm(".global sub_0804338C\n.thumb_set sub_0804338C, GetBattleUnitCounterattackBonus\n");

int GetUnitBaseMovement(int a)
{
    return gUnknown_085D5ABC[a].movement;
}

int GetUnitBaseFiringRange(int a)
{
    return gUnknown_085D5ABC[a].maxRange;
}

int GetUnitBaseCost(int a)
{
    return gUnknown_085D5ABC[a].cost;
}

int GetUnitBaseVision(int a)
{
    return gUnknown_085D5ABC[a].vision;
}

asm(".global sub_080433B8\n.thumb_set sub_080433B8, GetUnitBaseMovement\n"
    ".global sub_080433C8\n.thumb_set sub_080433C8, GetUnitBaseFiringRange\n"
    ".global sub_080433D8\n.thumb_set sub_080433D8, GetUnitBaseCost\n"
    ".global sub_080433E8\n.thumb_set sub_080433E8, GetUnitBaseVision\n");

int GetUnitBaseDamage(int a, int b, int c)
{
    return gUnknown_085D5ABC[a].baseDamage[c][b];
}
asm(".global sub_080433F8\n.thumb_set sub_080433F8, GetUnitBaseDamage\n");

void DrawMapCursorSprite(int x, int y, int id)
{
    int idx;
    int col;
    int row;

    switch (id)
    {
    case 0:
        x += 8;
        y += 8;
        row = (y + gMap->scrollY) >> 4;
        idx = gMap->rowOffset[row];
        col = (x + gMap->scrollX) >> 4;
        idx += col;
        if (gMap->unit[idx] == 0 && IsOwnFactoryCell(col, row))
            id = 4;
        else
            id = 0;
        id = GetCursorPointerSpriteId(x, y, id);
        break;
    case 1:
        x += 8;
        y += 8;
        id = GetCursorPointerSpriteId(x, y, 0);
        break;
    case 2:
        x += 8;
        y += 8;
        id = 0x10;
        break;
    case 3:
        id = 0x11;
        break;
    case 5:
        x += 8;
        y += 8;
        id = GetCursorPointerSpriteId(x, y, 8);
        break;
    case 6:
        x += 8;
        y += 8;
        id = GetCursorPointerSpriteId(x, y, 0xc);
        break;
    case 7:
        x += 8;
        y += 8;
        id = 0x12;
        break;
    case 12:
        x += 8;
        y += 0x10;
        id = 0x14;
        break;
    case 13:
        x += 8;
        y += 0x10;
        id = 0x13;
        break;
    case 14:
        id = 0x15;
        break;
    case 15:
        id = 0x16;
        break;
    case 16:
        id = 0x17;
        break;
    case 17:
        id = 0x18;
        break;
    case 18:
        x += 8;
        y += 8;
        id = 0x19;
        break;
    }

    AP_PutAnimFrameAtTime(gUnknown_08101EC0, id, gGameClock,
                 x & 0x1FF, y & 0xFF, 0x1365, 1);
}
asm(".global sub_08043418\n.thumb_set sub_08043418, DrawMapCursorSprite\n");

int GetCursorPointerSpriteId(int x, int y, int id)
{
    if (x > 0xcf)
    {
        if (y > 0x7f)
            id += 2;
        else
            id += 1;
    }
    else if (y > 0x7f)
    {
        id += 3;
    }

    return id;
}
asm(".global sub_08043574\n.thumb_set sub_08043574, GetCursorPointerSpriteId\n");

void AnimateCoPowerStatusPalette(void)
{
    volatile u32 t = gGameClock;

    if ((u16)(gUnknown_030005D0 - 1) > 3)
        return;
    if (gPlaySt.coPowersEnabled == 0)
        return;

    if (IsCoPowerReady(gUnknown_030005D0))
        ApplyPaletteAndUploadNow((u16 *)((((t >> 2) & 0xf) * 2) + (int)gUnknown_08104324), 0x2f6, 2);
    else
        ApplyPaletteAndUploadNow((u16 *)((((t >> 1) & 0xf) * 2) + (int)gUnknown_08104304), 0x2f6, 2);
}
asm(".global sub_08043590\n.thumb_set sub_08043590, AnimateCoPowerStatusPalette\n");

void DrawCoPanelWithDaysRemaining(int a)
{
    DrawArmyCoPanel(a, 3, gUnknown_030033EC);
    DrawDaysRemaining(a, 6);
}
asm(".global sub_0804360C\n.thumb_set sub_0804360C, DrawCoPanelWithDaysRemaining\n");
