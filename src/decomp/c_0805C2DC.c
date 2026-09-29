#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805C2DC.
 * sub_0805C2DC @ 0x0805C2DC, sub_0805C514 @ 0x0805C514, sub_0805C720 @ 0x0805C720
 */

/* Scores every unit in the armies whose bit is set in
 * gPlayers[a1].unk2c and returns the slot number of the best one.
 *
 * struct Map is the local cast-on view of gUnknown_08499590 that
 * include/unknown-globals.h prescribes: the ROM computes every plane address as
 * `(map + K) + idx`, and only a COMPONENT_REF preserves that association.
 * gUnknown_0816D9D8 is NOT a global -- the ROM word there holds 0x08499590, so
 * it is agbcc's own -fforce-addr constant for gUnknown_08499590; the honest
 * spelling reproduces it and the promotion carries the rodata word.
 *
 * Three things were each worth the whole match:
 *  - `e` is ONE pointer local reused for the outer slot record and the cell's
 *    record. The outer one has to be a pointer, not a subscript, or the base
 *    reload after FillMovementMap costs three instructions.
 *  - `n` must be DECLARED BEFORE `i`: their reload spill slots are handed out in
 *    declaration order, and the ROM's are 0x18 (n) then 0x1c (i). Same lever
 *    orders best/score/bestN into 0xc/0x10/0x14.
 *  - the score term is `unk04_0 * (call / 10)`, field FIRST. gcc 2.x's
 *    preexpand_calls hoists the GetUnitCostWithCoBonus call out ahead of the whole
 *    expression, so the field load lands between it and __divsi3 exactly as the
 *    ROM has it; writing `call / 10 * unk04_0` puts the load after the divide.
 *
 * MATCHED. */

u8 AiPickMissileTargetByValue(u16 a1, u8 a2)
{
    int n;
    int i;
    int x;
    int y;
    int best;
    int score;
    u8 bestN;
    struct Unit *e;

    best = 0;
    bestN = 0;

    for (i = 0; i <= 3; i++)
    {
        if (((gPlayers[a1].unk2c >> i) & 1) == 0)
            continue;

        for (n = i * 64; n < i * 64 + 64; n++)
        {
            e = &gUnits[n];
            if (e->type == 0)
                continue;

            score = 0;
            FillMovementMap(0xff);
            MapSetInRange(e->x, e->y, 2, 0);

            for (y = 0; y < gMap->height; y++)
            {
                for (x = 0; x < gMap->width; x++)
                {
                    if ((s8)gUnknown_03003340[y][x] < 0)
                        continue;
                    if (gMap->unitUnk[gMap->rowOffset[y] + x] == 0)
                        continue;
                    if (a2 != 0 && !IsCellVisibleToArmy(a1, x, y))
                        continue;
                    e = &gUnits[gMap->unitUnk[gMap->rowOffset[y] + x]];
                    if (e->type == 0x18)
                    {
                        if ((e->flags & 0x20) != 0)
                            continue;
                        if (!IsUnitVisibleToCurrentTeam(gMap->unit[gMap->rowOffset[y] + x]))
                            continue;
                    }
                    if (e->hp <= 10)
                        continue;
                    if (AreArmiesOnSameTeam(a1, (gMap->unitUnk[gMap->rowOffset[y] + x] >> 6) + 1) == 1)
                        score -= e->hp * (GetUnitCostWithCoBonus(gUnknown_030033EC, e->type) / 10);
                    else
                        score += e->hp * (GetUnitCostWithCoBonus(gUnknown_030033EC, e->type) / 10);
                }
            }

            if (score > best)
            {
                bestN = n;
                best = score;
            }
        }
    }

    return bestN;
}
asm(".global sub_0805C2DC\n.thumb_set sub_0805C2DC, AiPickMissileTargetByValue\n");

u8 AiPickMissileTargetByHp(u16 a1, u8 a2)
{
    int n;
    int i;
    int x;
    int y;
    int best;
    int score;
    u8 bestN;
    struct Unit *e;

    best = 0;
    bestN = 0;

    for (i = 0; i <= 3; i++)
    {
        if (((gPlayers[a1].unk2c >> i) & 1) == 0)
            continue;

        for (n = i * 64; n < i * 64 + 64; n++)
        {
            e = &gUnits[n];
            if (e->type == 0)
                continue;

            score = 0;
            FillMovementMap(0xff);
            MapSetInRange(e->x, e->y, 2, 0);

            for (y = 0; y < gMap->height; y++)
            {
                for (x = 0; x < gMap->width; x++)
                {
                    if ((s8)gUnknown_03003340[y][x] < 0)
                        continue;
                    if (gMap->unitUnk[gMap->rowOffset[y] + x] == 0)
                        continue;
                    if (a2 != 0 && !IsCellVisibleToArmy(a1, x, y))
                        continue;
                    e = &gUnits[gMap->unitUnk[gMap->rowOffset[y] + x]];
                    if (e->type == 0x18)
                    {
                        if ((e->flags & 0x20) != 0)
                            continue;
                        if (!IsUnitVisibleToCurrentTeam(gMap->unit[gMap->rowOffset[y] + x]))
                            continue;
                    }
                    if (e->hp <= 10)
                        continue;
                    if (AreArmiesOnSameTeam(a1, (gMap->unitUnk[gMap->rowOffset[y] + x] >> 6) + 1) == 1)
                        score -= e->hp;
                    else
                        score += e->hp;
                }
            }

            if (score > best)
            {
                bestN = n;
                best = score;
            }
        }
    }

    return bestN;
}
asm(".global sub_0805C514\n.thumb_set sub_0805C514, AiPickMissileTargetByHp\n");

u8 AiPickMissileTargetWeightingIndirect(u16 a1, u8 a2)
{
    int n;
    int i;
    int x;
    int y;
    int best;
    int score;
    u8 bestN;
    int mul;
    struct Unit *e;

    best = 0;
    bestN = 0;

    for (i = 0; i <= 3; i++)
    {
        if (((gPlayers[a1].unk2c >> i) & 1) == 0)
            continue;

        for (n = i * 64; n < i * 64 + 64; n++)
        {
            e = &gUnits[n];
            if (e->type == 0)
                continue;

            score = 0;
            FillMovementMap(0xff);
            MapSetInRange(e->x, e->y, 2, 0);

            for (y = 0; y < gMap->height; y++)
            {
                for (x = 0; x < gMap->width; x++)
                {
                    if ((s8)gUnknown_03003340[y][x] < 0)
                        continue;
                    if (gMap->unitUnk[gMap->rowOffset[y] + x] == 0)
                        continue;
                    if (a2 != 0 && !IsCellVisibleToArmy(a1, x, y))
                        continue;
                    e = &gUnits[gMap->unitUnk[gMap->rowOffset[y] + x]];
                    if (e->type == 0x18)
                    {
                        if ((e->flags & 0x20) != 0)
                            continue;
                        if (!IsUnitVisibleToCurrentTeam(gMap->unit[gMap->rowOffset[y] + x]))
                            continue;
                    }
                    if (e->hp <= 10)
                        continue;
                    mul = 1;
                    if (gUnknown_085D5ABC[e->type].minRange > 1)
                        mul = 2;
                    if (AreArmiesOnSameTeam(a1, (gMap->unitUnk[gMap->rowOffset[y] + x] >> 6) + 1) == 1)
                        score -= e->hp * (GetUnitCostWithCoBonus(gUnknown_030033EC, e->type) / 10) * mul;
                    else
                        score += e->hp * (GetUnitCostWithCoBonus(gUnknown_030033EC, e->type) / 10) * mul;
                }
            }

            if (score > best)
            {
                bestN = n;
                best = score;
            }
        }
    }

    return bestN;
}
asm(".global sub_0805C720\n.thumb_set sub_0805C720, AiPickMissileTargetWeightingIndirect\n");
