#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805ECDC.
 * AiChargeAggressively @ 0x0805ECDC, AiMoveWithFrontLine @ 0x0805ED70, AiAdvanceToAllocatedEnemyProperty @ 0x0805EE40, AiAdvanceToAllocatedEnemyPropertyUsingReach @ 0x0805EF00, AiHuntNearestEnemy @ 0x0805EF9C, AiMoveUpConservatively @ 0x0805F074
 */

void AiChargeAggressively(void)
{
    union Unk802C57CBuf v;
    u8 x;
    int t;

    if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation == 0x20)
        t = 0x11;
    else
        t = gUnknown_030040D8->unk00;

    AiGetReachBudget(&x);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03, t, x, 0);
    AiMarkAttackRings();

    if (AiFindNearestEnemyHq(&v) == -1)
        AiEmbarkOrFallback();
    else if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation == 0x20)
        sub_080590DC(&v);
    else
        AiAdvanceToward(&v);

    AiFallbackMove();
}

asm(".global sub_0805ECDC\n.thumb_set sub_0805ECDC, AiChargeAggressively\n");

void AiMoveWithFrontLine(void)
{
    union Unk802C57CBuf v;
    struct Unk03003338 *p;
    u8 x;

    p = gUnknown_03003F20;
    AiGetReachBudget(&x);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, x, 0);
    AiMarkAttackRings();
    if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation == 0x20)
        AiListUnescortedLanders(p);
    else
        AiListUnescortedFootUnits(p);
    v.pos.unk00 = 0x270F;
    AiPopLastNearestCandidate(p, (u16 *)&v);
    if (v.pos.unk00 == 0x270F)
        AiEmbarkOrFallback();
    gUnknown_03004730[gMap->unit[
        gMap->rowOffset[v.pos.unk02]
        + v.pos.unk00] & 0x3f]++;
    AiAdvanceToward(&v);
    AiFallbackMove();
}

asm(".global sub_0805ED70\n.thumb_set sub_0805ED70, AiMoveWithFrontLine\n");

void AiAdvanceToAllocatedEnemyProperty(void)
{
    union Unk802C57CBuf v;
    struct Unk03003338 *p;
    u8 x;
    int q;
    int a;
    int b;

    p = gUnknown_03003F20;
    AiGetReachBudget(&x);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, x, 0);
    AiMarkAttackRings();
    AiListEnemyPropertyCells(p);
    if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].minRange > 1)
    {
        q = CountUnitsWithTypeTag(4);
        a = gUnknown_085766E0->unk0c;
        b = 1;
    }
    else
    {
        q = CountUnitsWithTypeTag(5);
        a = gUnknown_085766E0->unk0c;
        b = 2;
    }
    v.pos.unk00 = 0x270F;
    AiAllocateTerritoryTarget(q, a, b, p, &v);
    if (v.pos.unk00 == 0x270F)
        AiEmbarkOrFallback();
    AiAdvanceToward(&v);
    AiFallbackMove();
}
asm(".global sub_0805EE40\n.thumb_set sub_0805EE40, AiAdvanceToAllocatedEnemyProperty\n");

void AiAdvanceToAllocatedEnemyPropertyUsingReach(void)
{
    union Unk802C57CBuf v;
    struct Unk03003338 *p;
    int q;
    int a;
    int b;

    p = gUnknown_03003F20;
    AiListEnemyPropertyCells(p);
    if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].minRange > 1)
    {
        q = CountUnitsWithTypeTag(4);
        a = gUnknown_085766E0->unk0c;
        b = 1;
    }
    else
    {
        q = CountUnitsWithTypeTag(5);
        a = gUnknown_085766E0->unk0c;
        b = 2;
    }
    v.pos.unk00 = 0x270F;
    AiAllocateTerritoryTarget(q, a, b, p, &v);
    if (v.pos.unk00 == 0x270F)
        AiEmbarkOrFallback();
    AiAdvanceToward(&v);
    AiFallbackMove();
}
asm(".global sub_0805EF00\n.thumb_set sub_0805EF00, AiAdvanceToAllocatedEnemyPropertyUsingReach\n");

void AiHuntNearestEnemy(void)
{
    union Unk802C57CBuf v;
    struct Unk03003338 *p;
    u8 x;

    p = gUnknown_03003F20;
    AiGetReachBudget(&x);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, x, 0);
    AiMarkAttackRings();
    AiListHuntTargets(p);
    v.pos.unk00 = 0x270F;
    AiPopLastNearestCandidate(p, (u16 *)&v);
    if (v.pos.unk00 == 0x270F)
        AiEmbarkOrFallback();
    else if ((s8)gUnknown_03003340[v.pos.unk02][v.pos.unk00] <= 0x79)
        AiAdvanceToward(&v);
    SetWorkingMapPlane(gMap->danger);
    gUnknown_030013EC(v.pos.unk00, v.pos.unk02, 0x10, 0x78, 0);
    AiAdvanceTowardUnseeded(&v);
    AiFallbackMove();
}
asm(".global sub_0805EF9C\n.thumb_set sub_0805EF9C, AiHuntNearestEnemy\n");

void AiMoveUpConservatively(void)
{
    union Unk802C57CBuf v;
    struct Unk03003338 *p;
    u8 x;

    p = gUnknown_03003F20;
    AiGetReachBudget(&x);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, x, 0);
    AiMarkAttackRings();
    AiListThreatenedProperties(p);
    v.pos.unk00 = 0x270F;
    AiPopLastNearestCandidate(p, (u16 *)&v);
    if (v.pos.unk00 == 0x270F)
        AiAdvanceToAllocatedEnemyPropertyUsingReach();
    AiAdvanceToward(&v);
    AiFallbackMove();
}

asm(".global sub_0805F074\n.thumb_set sub_0805F074, AiMoveUpConservatively\n");
