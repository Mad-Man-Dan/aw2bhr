#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CA2C.
 * sub_0802CA2C @ 0x0802CA2C, sub_0802CA78 @ 0x0802CA78, sub_0802CB20 @ 0x0802CB20
 */

int UnitMenu_LaunchUsability(void)
{
    if (!UnitMenu_JoinUsability())
        return 1;

    if (!UnitMenu_LoadUsability())
        return 1;

    FillMovementMap(0xff);
    gUnknown_03003340[gUnknown_03003100.pos.unk02][gUnknown_03003100.pos.unk00] = 0;

    if (BuildSiloCellList())
        return 0;

    return 1;
}
asm(".global sub_0802CA2C\n.thumb_set sub_0802CA2C, UnitMenu_LaunchUsability\n");

int UnitMenu_FireUsability(void)
{
    int off;
    u32 cur;

    if (gUnknown_030033E8[0] + gUnknown_030033E8[1] != 0)
        return 1;

    off = gMap->rowOffset[gUnknown_03003100.pos.unk02] + gUnknown_03003100.pos.unk00;

    if (gMap->unit[off] != 0)
        return 1;

    cur = gUnknown_03003100.raw;

    if (cur != gUnknown_03003F24.raw
        && gUnknown_085D5ABC[gUnknown_030040D8->unk00].minRange != 1)
        return 1;

    if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].minRange == 0)
        return 1;

    FillMovementMap(0xff);
    FillUnitAttackRange(gUnknown_03003100.pos.unk00, gUnknown_03003100.pos.unk02,
        (struct Unit *)gUnknown_030040D8);

    if (!BuildAttackTargetList())
        return 1;

    return 0;
}
asm(".global sub_0802CA78\n.thumb_set sub_0802CA78, UnitMenu_FireUsability\n");

int UnitMenu_FireNoTargetUsability(void)
{
    int off;
    u32 cur;

    if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].minRange == 1)
        return 1;

    off = gMap->rowOffset[gUnknown_03003100.pos.unk02] + gUnknown_03003100.pos.unk00;

    if (gMap->unit[off] != 0)
        return 1;

    cur = gUnknown_03003100.raw;

    if (cur != gUnknown_03003F24.raw)
        return 1;

    if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].minRange == 0)
        return 1;

    FillMovementMap(0xff);
    FillUnitAttackRange(gUnknown_03003100.pos.unk00, gUnknown_03003100.pos.unk02,
        (struct Unit *)gUnknown_030040D8);

    if (!BuildAttackTargetList())
        return 2;

    return 1;
}
asm(".global sub_0802CB20\n.thumb_set sub_0802CB20, UnitMenu_FireNoTargetUsability\n");
