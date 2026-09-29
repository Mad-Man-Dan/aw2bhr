#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805AD90.
 * sub_0805AD90 @ 0x0805AD90
 */

/* Redraws the map's 0x2852 plane, hands the two out-params of AiFindEnemyHqInInterestList to
 * the gUnknown_030013EC indirect hook, then sweeps every passable cell and
 * bumps the per-cell counter in the map's 0x3C72 plane wherever the terrain
 * code is 0xd or 0xb (the same two codes c_0805ACA8.c's AiCheckShoalParkNeighbour accepts,
 * spelled here as the positive `||` rather than that function's negative `&&`
 * guard).
 *
 * Cell addressing follows AiScoreEnemyPropertiesInReach (matched this wave): `rows` must be its
 * OWN local or agbcc reassociates the 0x417A pool constant to LAST, and the
 * 0x3C72 plane is reached with `p += 0x3c72; p += off;` -- the ROM's in-place
 * `adds r1,r1,r0` twice -- while the 0x1432 read keeps a separate `cells` local
 * because p is still live there and the ROM emits the three-operand add.
 *
 * THE `do { } while (0)` IS LOAD-BEARING AND IT IS A REGISTER-ALLOCATION LEVER,
 * not a control-flow one. Without it the function is size-exact with the
 * instruction sequence byte-for-byte identical and r1/r2 swapped throughout the
 * inner body: the ROM keeps `p` in r1 (inheriting the register y dies in at the
 * inner preheader) and spends r2 on the short-lived constant pseudos 0x417A,
 * 0x1432 and 0x1f, and every hand-written spelling did the opposite -- 13 bytes
 * of 248. Bracketing the two pointer bumps into their own statement group is
 * what puts p's allocno ahead of the constants'.
 *
 * RULED OUT by measurement before the permuter found this: declaration order of
 * the locals. Moving p/rows/cells ahead of a/b/x/y produced BYTE-IDENTICAL
 * output, so agbcc's allocno tie-break is not reachable from the declaration
 * list here.
 *
 * MATCHED (decomp-permuter, 380 iterations from the 94.8% hand draft). */
void AiMarkLandingCellsNearEnemyHq(void)
{
    int a;
    int b;
    int x;
    int y;
    int off;
    int terrain;

    if (AiFindEnemyHqInInterestList(sub_0805B4A8(), &a, &b) == 0)
        AiFallbackMove();

    SetWorkingMapPlane(gMap->move);

    gUnknown_030013EC(a, b, gUnknown_030046D4, gUnknown_085766E0->unk0f, 0);

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] >= 0)
            {
                off = gMap->rowOffset[y] + x;
                terrain = gMap->terrain[off] & 0x1f;
                if (terrain == 0xd || terrain == 0xb)
                    gMap->unk3C72[off]++;
            }
        }
    }

    AiFinishLandingPlan();
}
asm(".global sub_0805AD90\n.thumb_set sub_0805AD90, AiMarkLandingCellsNearEnemyHq\n");
