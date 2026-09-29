#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08059674.
 * sub_08059674 @ 0x08059674, sub_08059760 @ 0x08059760, sub_08059824 @ 0x08059824, sub_080598BC @ 0x080598BC, sub_08059978 @ 0x08059978
 */

/* K&R declaration, deliberately file-local and deliberately without a
 * parameter list. AiIsNearEnemyHq's definition takes `u16` parameters and narrows
 * them itself in its prologue; this caller passes values it has just
 * sign-extended, with no conversion instruction before the `bl`. A prototyped
 * declaration in unknown-functions.h cannot satisfy both -- see the note there
 * for the four measurements. */
int AiIsNearEnemyHq();
struct Unk0805DFF4Rec
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 3;
               u8 unk09_6 : 2;
};

u8 AiIsSettleCellOk(s16 x, s16 y)
{
    if (gMap->unit[
            gMap->rowOffset[y] + x]
                != gUnknown_03003F38
        && gMap->unit[
            gMap->rowOffset[y] + x] != 0)
        return 0;
    if ((u8)AiIsOnLaserLine(x, y))
        return 0;
    if (gUnknown_085767D5[gMap->terrain[
            gMap->rowOffset[y] + x] & 0x1f] == 0)
        return 1;
    if ((gMap->terrain[
            gMap->rowOffset[y] + x] & 0xe0)
                != gUnknown_03004084)
    {
        if ((gMap->terrain[
                gMap->rowOffset[y] + x] & 0xe0) == 0)
            return 0;
        if (gUnknown_030040D8->unk00 <= 2)
            return 1;
        if ((u8)AiIsNearEnemyHq(x, y) == 1)
            return 1;
        return 0;
    }
    if (gUnknown_085767B8[gMap->terrain[
            gMap->rowOffset[y] + x] & 0x1f] == 0)
        return 1;
    if (gUnknown_085767B8[gMap->terrain[
            gMap->rowOffset[y] + x] & 0x1f]
                == gUnknown_030046AC)
        return 1;
    if (!gUnknown_030045CC.unk00_1)
        return 0;
    return 1;
}
asm(".global sub_08059674\n.thumb_set sub_08059674, AiIsSettleCellOk\n");

void AiDeliberateApcPickup(void)
{
    union Unk802C57CBuf v;

    SetWorkingMapPlane(gMap->danger);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, 0x78, -1);
    MapMarkHalo(0x79);
    v.pos.unk00 = 0x270F;
    sub_0805A9AC(0, &v);
    if (v.pos.unk00 != 0x270F)
    {
        AiAdvanceToward(&v);
    }
    else
    {
        if (gUnknown_03004784[1] > (u8)(gUnknown_030040D8->unk07[3] % 100))
            AiRetreat();
        if (gUnknown_03004784[0] > (u8)(gUnknown_030040D8->unk07[3] % 100)
            || IsCoPowerActive(gUnknown_030033EC))
            AiTryAttack();
    }
    AiFallbackMove();
}
asm(".global sub_08059760\n.thumb_set sub_08059760, AiDeliberateApcPickup\n");

void AiDeliberateTCopterPickup(void)
{
    union Unk802C57CBuf v;

    SetWorkingMapPlane(gMap->danger);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, 0x78, 0);
    MapMarkHalo(0x79);
    v.pos.unk00 = 0x270F;
    sub_0805A9AC(1, &v);
    if (v.pos.unk00 != 0x270F)
    {
        AiAdvanceToward(&v);
    }
    else
    {
        if (gUnknown_03004784[1] > (u8)(gUnknown_030040D8->unk07[3] % 100))
            AiRetreat();
    }
    AiMoveToNearestNonTeamCell();
}
asm(".global sub_08059824\n.thumb_set sub_08059824, AiDeliberateTCopterPickup\n");

void AiDeliberateApcDeliver(void)
{
    union Unk802C57CBuf v;
    struct Unk03003338 *p;
    int q;

    p = gUnknown_03003F20;
    AiDeliberateDrop();
    SetWorkingMapPlane(gMap->move);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, 0x78, 0);
    AiListEnemyPropertyCells(p);
    q = CountUnitsWithTypeTag(1);
    v.pos.unk00 = 0x270F;
    AiAllocateTerritoryTarget(q, gUnknown_085766E0->unk04[7], 0, p, &v);
    if (v.pos.unk00 != 0x270F)
    {
        AiAdvanceToward(&v);
    }
    else if (gUnknown_030046B8 == 2)
    {
        ((struct Unk0805DFF4Rec *)gUnknown_030040D8)->unk09_3 = 3;
        AiBoardTransport();
        AiEmbarkOrFallback();
    }
    AiFallbackMove();
}
asm(".global sub_080598BC\n.thumb_set sub_080598BC, AiDeliberateApcDeliver\n");

void AiDeliberateTCopterDeliver(void)
{
    union Unk802C57CBuf v;
    struct Unk03003338 *p;
    int q;

    p = gUnknown_03003F20;
    AiDeliberateDrop();
    SetWorkingMapPlane(gMap->move);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, 0x78, 0);
    AiListEnemyPropertyCells(p);
    q = CountUnitsWithTypeTag(1);
    v.pos.unk00 = 0x270F;
    AiAllocateTerritoryTarget(q, gUnknown_085766E0->unk04[6], 0, p, &v);
    if (v.pos.unk00 == 0x270F)
        AiFallbackMove();
    AiAdvanceToward(&v);
}
asm(".global sub_08059978\n.thumb_set sub_08059978, AiDeliberateTCopterDeliver\n");
