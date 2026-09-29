#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805EB58.
 * sub_0805EB58 @ 0x0805EB58
 */

/* Wave 63, W63-C: MATCHED. Splitting the packed store into `xp = x << 16`,
 * `yp = y << 16`, then `(xp >> 16) | yp` produces the ROM's interleaving.
 *
 * The map reads use the canonical gMap fields directly: move, unit, terrain
 * and rowOffset.
 */
union Unk5EB58Sel
{
    u32 raw;
    struct
    {
        u32 unk00 : 16;
        u32 unk02 : 16;
    } f;
};

void AiMoveToNearestNonTeamCell(void)
{
    union Unk5EB58Sel v;
    s16 best;
    int x;
    int y;
    int idx;
    int t;
    u8 *tbl;
    u32 xp;
    u32 yp;

    best = 0x7fff;
    SetWorkingMapPlane(gMap->move);
    GenerateUnitMovementMap(gUnknown_030040D8);
    v.f.unk00 = 0x270f;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;
            idx = gMap->rowOffset[y] + x;
            if (gMap->unit[idx] != 0)
                continue;
            if (IsTerrainOwnedByUnitsTeam(gUnknown_03003F38, gMap->terrain[idx]) == 1)
                continue;
            tbl = gUnknown_085D5ABC[gUnknown_030040D8->unk00].transportTable;
            t = gMap->terrain[gMap->rowOffset[y] + x] & 0x1f;
            tbl += 0x1a;
            if (tbl[t] == 0)
                continue;
            if (((s8 *)gUnknown_03003340[y])[x] > best)
                continue;
            xp = (u32)x << 16;
            yp = (u32)y << 16;
            v.raw = (xp >> 16) | yp;
            best = (s8)gUnknown_03003340[y][x];
        }
    }

    if (v.f.unk00 != 0x270f)
        AiAdvanceToward(&v);
    else
        AiFallbackMove();
}
asm(".global sub_0805EB58\n.thumb_set sub_0805EB58, AiMoveToNearestNonTeamCell\n");
