#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805E9DC.
 * sub_0805E9DC @ 0x0805E9DC, sub_0805EA54 @ 0x0805EA54
 */

/* A file-local bitfield view of gUnknown_030040D8's offset 0x09, which is
 * struct Unk030040D8's unk07[2]. The header records that the byte really is a
 * bitfield container and that it is DELIBERATELY not reshaped, because promoted
 * code reads unk07[0], unk07[1] and unk07[4] as an array -- AiTryRideInsteadOfWalk uses
 * the same file-local view. */
struct Unk5E9DCFlags
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 5;
};
struct Unk5EA54Flags
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 5;
};

void AiRunResupplyOrRepairMission(void)
{
    void (*fns[2])(void) = { AiSeekSupplier, AiSeekRepairProperty };

    SetWorkingMapPlane(gMap->move);

    if ((gUnknown_030040D8->unk05 & 0xf8) == 0
        && ((struct Unk5E9DCFlags *)gUnknown_030040D8)->unk09_0 != 0
        && ((struct Unk5E9DCFlags *)gUnknown_030040D8)->unk09_0 <= 2)
    {
        gUnknown_030045CC.unk00_1 = 1;
        AiTryJoinHealthiestSameTypeUnit();
        fns[((struct Unk5E9DCFlags *)gUnknown_030040D8)->unk09_0 - 1]();
    }
}
asm(".global sub_0805E9DC\n.thumb_set sub_0805E9DC, AiRunResupplyOrRepairMission\n");

void AiTryJoinHealthiestSameTypeUnit(void)
{
    int best;
    int bestX;
    int bestY;
    int i;

    best = 0;
    bestX = -1;
    bestY = 0;

    if (gUnknown_030040D8->unk04 > 0x32)
        return;

    GenerateUnitMovementMap(gUnknown_030040D8);

    for (i = gUnknown_03003F2C; i < gUnknown_03003F2C + 0x40; i++)
    {
        struct Unit *p;

        p = &gUnits[i];

        if (gUnknown_030040D8->unk00 != p->type)
            continue;
        if (gUnknown_030040D8->unk04 + p->hp > 0x64)
            continue;
        if (gUnknown_030040D8->unk07[0] != -p->unk07)
            continue;
        if ((struct Unit *)gUnknown_030040D8 == p)
            continue;
        if (p->flags & 8)
            continue;
        if (p->hp <= best)
            continue;
        if ((s8)gUnknown_03003340[p->y][p->x] <= 0)
            continue;

        bestX = p->x;
        bestY = p->y;
        best = p->hp;
    }

    if (bestX == -1)
        return;

    ((struct Unk5EA54Flags *)gUnknown_030040D8)->unk09_0 = 0;
    AiPublishAction(bestX, bestY, 0xa, 0, 0);
}
asm(".global sub_0805EA54\n.thumb_set sub_0805EA54, AiTryJoinHealthiestSameTypeUnit\n");
