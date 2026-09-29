#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08061B4C.
 * sub_08061B4C @ 0x08061B4C
 */

/* MATCHED.
 *
 * The map planes go through `gMap` (include/map.h), the typed linker alias
 * for gUnknown_08499590. Plain `u8 *` arithmetic on it reassociates the
 * 0x1432 / 0x417A / 0x12 constants PAST the index -- the ROM adds the plane
 * constant to the base first, then the index -- and costs far more than the
 * tail below. `gMap` is named directly at every use rather than bound to a
 * local: the ROM keeps only its ADDRESS in sl and re-loads the pointer each
 * time. The two `SetWorkingMapPlane`/`FillMapPlane` setup calls must also pass
 * `gMap->move` / `gMap->unk376A` (the plane's own array member, decays to
 * `u8 *`), not `gUnknown_08499590 + offset` or a cast -- agbcc's CSE only
 * reuses a pointer load across identical symbols, and mixing in the raw name
 * anywhere forces a second pool load (see AiPickSafestReachableCell for the fuller
 * writeup of this).
 *
 * gUnits is a REAL declared global, not a pool word, and so is
 * gUnknown_08499590 -- both trip the "named pointer whose target is also named"
 * false positive in the pool-word screen.
 *
 * THE LAST 8 BYTES, and the reason this is worth reading: hand-written C got to
 * 98.0% with the size exact, the relocations exact, the pool exact and all ten
 * words in order -- everything but four instructions:
 *
 *   original            plain `gUnits[c].type`
 *   ldr  r0, [pc,#208]  ldr  r0, [pc,#208]
 *   lsls r1, r4, #1     ldr  r1, [r0]
 *   adds r1, r1, r4     lsls r0, r4, #1
 *   lsls r1, r1, #2     adds r0, r0, r4
 *   ldr  r0, [r0]       lsls r0, r0, #2
 *   adds r0, r0, r1     adds r0, r0, r1
 *
 * The ROM computes `c * 12` BETWEEN the pool load of &gUnits and the
 * `ldr r0,[r0]` that dereferences it, hiding the load in the multiply, and gives
 * the multiply r1 rather than r0. No statement boundary reaches inside that
 * address expression, so no rewriting of the `&&` chain moves it.
 *
 * decomp-permuter closed it in 9,826 iterations, and the mutation it found is
 * the ASSIGNMENT-INSIDE-THE-EXPRESSION below: binding the element address to its
 * own local *within* the comparison creates that pseudo at a point ordinary
 * source cannot name, which is the same lever the comma-operator note in
 * docs/agbcc-codegen.md describes for pool ordering. This is the residual class
 * that doc's tail calls the permuter's own -- instruction ORDER, not stack slot
 * -- and it behaved exactly as advertised.
 */

#define MAP gMap

void AiBuildCapturablePropertyProximityPlane(void)
{
    int i;
    struct Unit *u;
    int j;
    int k;
    u8 c;

    SetWorkingMapPlane(gMap->move);
    FillMapPlane(gMap->unk376A, 0);

    for (i = 0; gUnknown_084995A0[i].unk00 != 0xFF; i++)
    {
        if ((MAP->terrain[MAP->rowOffset[gUnknown_084995A0[i].unk02]
                          + gUnknown_084995A0[i].unk01] & 0x1F) == 0x13)
            continue;

        if (IsTerrainOwnedByUnitsTeam(gUnknown_03003F2C,
                         MAP->terrain[MAP->rowOffset[gUnknown_084995A0[i].unk02]
                                      + gUnknown_084995A0[i].unk01]) == 1)
            continue;

        c = gMap->unit[MAP->rowOffset[gUnknown_084995A0[i].unk02]
                         + gUnknown_084995A0[i].unk01];

        if (c != 0 && AreUnitsOnSameTeam(gUnknown_03003F2C, c) == 1
            && (*(u = &gUnits[c])).type <= 2)
            continue;

        gUnknown_030013EC(gUnknown_084995A0[i].unk01, gUnknown_084995A0[i].unk02, 1, 3, 0);

        for (j = 0; j < MAP->height; j++)
        {
            for (k = 0; k < MAP->width; k++)
            {
                if ((s8)gUnknown_03003340[j][k] >= 0)
                    MAP->unk376A[MAP->rowOffset[j] + k]++;
            }
        }
    }
}
asm(".global sub_08061B4C\n.thumb_set sub_08061B4C, AiBuildCapturablePropertyProximityPlane\n");
