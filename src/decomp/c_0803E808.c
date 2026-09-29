#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E808.
 * sub_0803E808 @ 0x0803E808
 */

/* MATCHED (wave 49, W49-C), first attempt. 496/496 bytes.
 *
 * PROMOTION NOTE: the only reloc difference is agbcc's -fforce-addr copy of
 * &gUnknown_08499590 at 0x08091300 (the ROM word there contains 0x08499590),
 * which gen_lds.py names gUnknown_08091300. Do NOT declare it. This entry needs
 *   "rodata": ["0x08091300"]
 * in data/promoted.json, then tools/split_rodata.py + tools/gen_lds.py.
 *
 * Picks the highest-scoring occupied, visible cell on the map and appends it.
 * a3 is genuinely DEAD -- r2 is overwritten before it is ever read, and
 * InventionTurn_PrepareNextFire's call site passes the record's s8 unk07 into it.
 *
 * Readouts worth keeping:
 *  - `best`/`score` are unsigned (`cmp r0,r4; bhi`), while bestX/bestY/bestT are
 *    `u16` locals: PROMOTE_MODE keeps them in SImode stack slots (word `str`),
 *    so the narrowing shows up as `lsls #0x10; lsrs #0x10` AT EACH ASSIGNMENT
 *    and their reads at the PushInventionFireEntry call are free. Declaring them `int`
 *    loses three shift pairs; declaring them `u16` is what puts the pairs where
 *    the ROM has them.
 *  - `(u16)a5` at sub_0801FAC4's third argument is a real narrowing in the
 *    source: the two GetCannonFireCellOffsetX/E7E4 calls above it use a5 un-narrowed, so it
 *    cannot be a parameter declaration.
 *  - GetCannonFireCellOffsetX/GetCannonFireCellOffsetY return `void *` per their promoted definition,
 *    and here the results are ADDED to cell coordinates. The `(int)` casts are
 *    what that costs. This is evidence that struct Unk0849F688's two word
 *    members are integer deltas rather than pointers, but the existing type is
 *    byte-neutral at both sites and c_0803E7C0.c is already promoted, so it is
 *    recorded rather than changed.
 */

#define MAP gMap

void PickBestInventionTarget(int a1, int a2, int a3, int a4, int a5)
{
    struct Unit *u;
    u32 best;
    u32 score;
    u16 bestX;
    u16 bestY;
    u16 bestT;
    u16 x;
    u16 y;
    int i;
    int j;

    best = 0;
    bestX = 0;
    bestY = 0;
    bestT = 0;
    SetWorkingMapPlane(MAP->move);
    FillMovementMap(0xff);
    x = a1 + (int)GetCannonFireCellOffsetX(a4, a5);
    y = a2 + (int)GetCannonFireCellOffsetY(a4, a5);
    sub_0801FAC4(x, y, (u16)a5, gUnknown_0849F6B8[a4][0], bestT);
    for (j = 0; j < MAP->height; j++)
    {
        for (i = 0; i < MAP->width; i++)
        {
            if ((s8)gUnknown_03003340[j][i] < 0)
                continue;
            if (gMap->unit[MAP->rowOffset[j] + i] == 0)
                continue;
            if (AreArmiesOnSameTeam(gUnknown_030033EC, (gMap->unit[MAP->rowOffset[j] + i] >> 6) + 1) == 1)
                continue;
            if (!IsCellVisibleToArmy(gUnknown_030033EC, i, j))
                continue;
            u = &gUnits[gMap->unit[MAP->rowOffset[j] + i]];
            if (u->type == 0x18)
            {
                if ((u->flags & 0x20) != 0)
                    continue;
                if (!IsUnitVisibleToCurrentTeam(gMap->unit[MAP->rowOffset[j] + i]))
                    continue;
            }
            score = u->hp * (u16)(gUnknown_085D5ABC[u->type].cost / 10);
            if (best > score)
                continue;
            best = score;
            bestX = i;
            bestY = j;
            bestT = gMap->unit[MAP->rowOffset[j] + i];
        }
    }
    if (bestT == 0)
        ClearInventionFireList();
    else
        PushInventionFireEntry(bestX, bestY, bestT, a4 == 3 ? 0x32 : 0x1e);
}
asm(".global sub_0803E808\n.thumb_set sub_0803E808, PickBestInventionTarget\n");
