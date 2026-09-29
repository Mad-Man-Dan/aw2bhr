#include "global.h"

void SelectBattleWeapon(struct BattleUnit *a, struct BattleUnit *b, s16 c, u8 d)
{
    const struct UnitType *t;
    u16 army;
    u32 v1;
    s16 v2;
    u32 v3;

    army = ((struct ArmyUnitBlock *)a->unit
            - (struct ArmyUnitBlock *)gUnits) + 1;
    v3 = 0;
    v1 = 0;
    v2 = 0;
    t = &gUnknown_085D5ABC[a->unit->type];

    if (c == 1)
    {
        v2 = GetUnitBaseDamage(a->unit->type, b->unit->type, 1);
        if (v2 != 0)
            v1 = (u16)GetCoAdjustedBaseDamage(gPlayers[army].co,
                              gPlayers[army].coMode,
                              a->unit->type, b->unit->type, c);
        if (t->minRange == 1 && a->unit->ammo != 0)
        {
            if ((b->unit->flags & 0x20) != 0)
                v3 = (u16)GetCoAdjustedBaseDamage(gPlayers[army].co,
                                  gPlayers[army].coMode,
                                  a->unit->type, 0x19, v3);
            else
                v3 = (u16)GetCoAdjustedBaseDamage(gPlayers[army].co,
                                  gPlayers[army].coMode,
                                  a->unit->type, b->unit->type, v3);
        }
    }
    else if (t->minRange <= c && c <= GetUnitFiringRangeWithCoBonus(army, a->unit->type)
             && a->unit->ammo != 0 && d == 1)
    {
        if ((b->unit->flags & 0x20) != 0)
            v3 = (u16)GetCoAdjustedBaseDamage(gPlayers[army].co,
                              gPlayers[army].coMode,
                              a->unit->type, 0x19, v3);
        else
            v3 = (u16)GetCoAdjustedBaseDamage(gPlayers[army].co,
                              gPlayers[army].coMode,
                              a->unit->type, b->unit->type, v3);
    }

    if (v3 <= v1)
    {
        if (v2 != 0)
        {
            a->attackType = 5;
            a->baseDamage = v1;
        }
    }
    else
    {
        a->attackType = 1;
        a->baseDamage = v3;
        a->ammo--;
    }
}

asm(".global sub_08024ABC\n.thumb_set sub_08024ABC, SelectBattleWeapon\n");

void CalcBattleUnitAttackAndDefense(struct BattleUnit *a1, int a2, u8 a3)
{
    int idx;
    int firepower;
    int defense;
    int acc;
    int hp;
    int r;
    int q;

    idx = (a1->unit - gUnits) >> 6;
    /* gPlayers[idx + 1].tempFirepower / .tempDefense -- idx is 0-based here and
     * gPlayers is 1-based, and 0x3c + 0x26 == 0x62, 0x3c + 0x28 == 0x64. The
     * NAMED spelling does not reproduce and three forms were measured: both
     * `gPlayers[idx + 1].tempFirepower` and `(&gPlayers[idx])[1].tempFirepower`
     * put the +1 where agbcc will not fold it into the member displacement, and
     * a `(struct PlayerStruct *)((u8 *)&gPlayers[idx] + sizeof ...)` cast fails
     * the same way. The ROM keeps ONE element base in r1 and reaches both
     * fields off it with `adds r0,r1,#0; adds r0,#0x62` / `adds r1,#0x64`, which
     * is what a byte-pointer displacement off gPlayers[idx] emits. The s16 on
     * both members (include/co.h) is what makes these `ldrsh`. */
    firepower = *(s16 *)((u8 *)&gPlayers[idx] + 0x62);
    defense = *(s16 *)((u8 *)&gPlayers[idx] + 0x64);
    acc = 100;

    if (a1->unit->hp != 0)
        hp = Div(a1->unit->hp - 1, 10) + 1;
    else
        hp = 0;

    a1->terrainDefense = GetBattleUnitTerrainDefense(a1);
    a1->totalDefense = Div(hp * a1->terrainDefense, 10);
    a1->totalDefense = a1->totalDefense
              + GetUnitDefenceWithCoBonus(((a1->unit - gUnits) >> 6) + 1, a1->unit->type)
              + defense;
    a1->damage = a1->baseDamage;

    if (a2 == 1)
        acc += GetBattleUnitCounterattackBonus(a1);

    acc += GetBattleUnitTerrainFirepowerBonus(a1);
    acc += firepower;

    a1->damage = Div(acc * a1->damage, 100);
    a1->displayDamage = a1->damage;

    if (a1->damage != 0)
    {
        if (gPlaySt.campaignRelated == 0 || a3 != 0)
        {
            r = DivRem(GetNextRandomNumber(),
                       GetPlayerCoLuckBonus(((a1->unit - gUnits) >> 6) + 1));
            q = GetPlayerCoNegativeLuckBonus(((a1->unit - gUnits) >> 6) + 1);
            if (q != 0)
                q = DivRem(GetNextRandomNumber(), q);

            a1->damage = a1->damage + r - q;
            if (a1->damage < 0)
                a1->damage = 0;
        }
    }
}
asm(".global sub_08024C58\n.thumb_set sub_08024C58, CalcBattleUnitAttackAndDefense\n");

void ApplyBattleHit(struct BattleUnit *a1, struct BattleUnit *a2)
{
    int hp;

    if (a2->unit->hp != 0)
        hp = Div(a2->unit->hp - 1, 10) + 1;
    else
        hp = 0;

    a2->displayDamage = Div((200 - a1->totalDefense) * a2->displayDamage, 100);
    a2->displayDamage = Div(hp * a2->displayDamage, 10);

    if (a2->displayDamage > 998)
        a2->displayDamage = 999;

    a1->hpLoss = Div((200 - a1->totalDefense) * a2->damage, 100);
    a1->remainingHp = a1->unit->hp - a1->hpLoss;
}
asm(".global sub_08024DDC\n.thumb_set sub_08024DDC, ApplyBattleHit\n");

void ResolveBattleExchange(struct BattleUnit *a1, struct BattleUnit *a2)
{
    int v;
    s16 hp;

    if (a1->unit->hp != 0)
        v = a1->damage * (Div(a1->unit->hp - 1, 10) + 1);
    else
        v = 0;

    a1->damage = Div(v, 10);

    ApplyBattleHit(a2, a1);

    if (a2->remainingHp >= 0)
        hp = a2->remainingHp;
    else
        hp = 0;

    if (hp != 0)
        v = a2->damage * (Div(hp - 1, 10) + 1);
    else
        v = 0;

    a2->damage = Div(v, 10);

    ApplyBattleHit(a1, a2);
}
asm(".global sub_08024E60\n.thumb_set sub_08024E60, ResolveBattleExchange\n");

void PreventBattleMutualKill(struct BattleUnit *a, struct BattleUnit *b)
{
    if (a->remainingHp <= 0)
    {
        if (b->remainingHp <= 0)
        {
            if (a->remainingHp < b->remainingHp)
            {
                b->remainingHp = 1;
                a->remainingHp = 0;
            }
            else
            {
                a->remainingHp = 1;
                b->remainingHp = 0;
            }

            return;
        }

        if (a->remainingHp < 0)
            a->remainingHp = 0;
    }

    if (b->remainingHp < 0)
        b->remainingHp = 0;
}
asm(".global sub_08024ED8\n.thumb_set sub_08024ED8, PreventBattleMutualKill\n");

void CalcBattleDamage(s16 a1, s16 a2, struct Unk802C57C *a3)
{
    u32 saved;
    int dx;
    int dy;
    int d;

    saved = gUnits[a1].x;
    gUnits[a1].x = a3->unk00;
    saved |= gUnits[a1].y << 16;
    gUnits[a1].y = a3->unk02;

    InitBattleUnit(gBattleAttacker, a1);
    InitBattleUnit(gBattleDefender, a2);

    dx = gBattleAttacker->unit->x
       - gBattleDefender->unit->x;
    if (dx < 0)
        dx = -dx;

    dy = gBattleAttacker->unit->y
       - gBattleDefender->unit->y;
    if (dy < 0)
        dy = -dy;

    d = dx + dy;

    SelectBattleWeapon(gBattleAttacker,
                 gBattleDefender, d, 1);
    SelectBattleWeapon(gBattleDefender,
                 gBattleAttacker, d, 0);
    CalcBattleUnitAttackAndDefense(gBattleAttacker, 0, 1);
    CalcBattleUnitAttackAndDefense(gBattleDefender, 1, 1);

    if ((GetPlayerSpecialAbilities(((gBattleDefender->unit
                        - gUnits) >> 6) + 1) & 4) != 0)
        ResolveBattleExchange(gBattleDefender,
                     gBattleAttacker);
    else
        ResolveBattleExchange(gBattleAttacker,
                     gBattleDefender);

    PreventBattleMutualKill(gBattleAttacker,
                 gBattleDefender);

    gUnits[a1].x = saved;
    gUnits[a1].y = saved >> 16;
}

asm(".global sub_08024F20\n.thumb_set sub_08024F20, CalcBattleDamage\n");

void sub_08025058(void)
{
}

void WriteBackBattleUnit(void *a1)
{
    struct BattleUnit *p = a1;
    int v;
    u16 w;

    if (p->unit->hp != 0)
        v = Div(p->unit->hp - 1, 10) + 1;
    else
        v = 0;

    if (p->remainingHp != 0)
        w = v - 1 - Div(p->remainingHp - 1, 10);
    else
        w = v;

    sub_08025B24(p->unit, w);

    p->unit->hp = p->remainingHp;
    p->unit->ammo = p->ammo;

    if (p->unit->hp == 0)
        StartUnitDestroyed(p->unit);
}
asm(".global sub_0802505C\n.thumb_set sub_0802505C, WriteBackBattleUnit\n");

void WriteBackBattleResult(void)
{
    if (gBattleAttacker->remainingHp <= 0)
        RecordUnitDestroyed(
            ((gBattleDefender->unit - gUnits) >> 6) + 1,
            ((gBattleAttacker->unit - gUnits) >> 6) + 1,
            gBattleAttacker->unit->type);

    if (gBattleDefender->remainingHp <= 0)
        RecordUnitDestroyed(
            ((gBattleAttacker->unit - gUnits) >> 6) + 1,
            ((gBattleDefender->unit - gUnits) >> 6) + 1,
            gBattleDefender->unit->type);

    WriteBackBattleUnit(gBattleAttacker);
    WriteBackBattleUnit(gBattleDefender);
}
asm(".global sub_080250E8\n.thumb_set sub_080250E8, WriteBackBattleResult\n");

void WriteBackBattleAttacker(void)
{
    WriteBackBattleUnit(gBattleAttacker);
}
asm(".global sub_080251AC\n.thumb_set sub_080251AC, WriteBackBattleAttacker\n");

void CalcAttackOutcome(int a1, int a2, struct Unk802C57C *a3)
{
    if (a2 == 0)
        CalcBattleDamageVsStructure(a1);
    else
        CalcBattleDamage(a1, a2, a3);
}
asm(".global sub_080251BC\n.thumb_set sub_080251BC, CalcAttackOutcome\n");

void CalcBattleDamageVsStructure(int a1)
{
    struct Unit *e;
    int a;
    int b;
    int ok;
    int t;

    a = 0;
    b = 0;
    ok = 0;

    e = &gUnits[a1];

    InitBattleUnit(gBattleAttacker, a1);

    if (e->ammo != 0)
    {
        a = GetCoAdjustedBaseDamage(gPlayers[gUnknown_030033EC].co,
                         gPlayers[gUnknown_030033EC].coMode,
                         e->type, 3, 0);
    }
    else
    {
        ok = GetUnitBaseDamage(e->type, 3, 1);
        if (ok != 0)
            b = GetCoAdjustedBaseDamage(gPlayers[gUnknown_030033EC].co,
                             gPlayers[gUnknown_030033EC].coMode,
                             e->type, 3, 1);
    }

    if (a <= b)
    {
        if (ok != 0)
        {
            gBattleAttacker->attackType = 5;
            gBattleAttacker->baseDamage = b;
        }
    }
    else
    {
        gBattleAttacker->attackType = 1;
        gBattleAttacker->baseDamage = a;
        gBattleAttacker->ammo--;
    }

    CalcBattleUnitAttackAndDefense(gBattleAttacker, 0, 0);

    if (gBattleAttacker->unit->hp != 0)
        t = gBattleAttacker->displayDamage
          * (Div(gBattleAttacker->unit->hp - 1, 10) + 1);
    else
        t = 0;

    gBattleAttacker->displayDamage =
        gBattleAttacker->damage = Div(t, 10);
}
asm(".global sub_080251D8\n.thumb_set sub_080251D8, CalcBattleDamageVsStructure\n");

void sub_080252E8(void *arg)
{
}

void sub_080252EC(void)
{
    sub_080252E8(gBattleAttacker);
    sub_080252E8(gBattleDefender);
}
