#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080210C8.
 * sub_080210C8 @ 0x080210C8
 */

/* Dispatches one of three record runs in gMap->visible to sub_08020EDC:
 * cases 1 and 3 select it with gUnknown_03004070, case 2 with
 * gUnknown_03004088 (each run is MAP_POOL_SIZE bytes), and case 3 FALLS
 * THROUGH into case 2 so it issues both.
 *
 * The dispatch tree pivots on 1 (`cmp #1; beq` then `cmp #1; ble`), which three
 * reachable arms cannot produce -- balance_case_nodes puts a three-node tree's
 * root at 2. The fourth node is `case 0:` sharing `default:`, the recipe
 * docs/agbcc-codegen.md records from waves 28 and 29; it is invisible in the
 * branch targets because it shares the end label, so only the TEST COUNT shows
 * it.
 *
 * `(u8)a6` (wave 90, W90-C): sub_08020EDC's 6th parameter is `int`, and
 * this cast is the `lsls #0x18; lsrs #0x18` each call site emits. */
void StampVisionByPlaneMask(s16 a1, s16 a2, s16 a3, s16 kind, s8 a5, int a6)
{
    switch (kind)
    {
    case 0:
    default:
        break;
    case 1:
        sub_08020EDC(a1, a2, a3,
                     &gMap->visible[gUnknown_03004070 * 1288],
                     a5, (u8)a6);
        break;
    case 3:
        sub_08020EDC(a1, a2, a3,
                     &gMap->visible[gUnknown_03004070 * 1288],
                     a5, (u8)a6);
    case 2:
        sub_08020EDC(a1, a2, a3,
                     &gMap->visible[gUnknown_03004088 * 1288],
                     a5, (u8)a6);
        break;
    }
}
asm(".global sub_080210C8\n.thumb_set sub_080210C8, StampVisionByPlaneMask\n");
