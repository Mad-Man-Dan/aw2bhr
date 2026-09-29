#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045CE8.
 * sub_08045CE8 @ 0x08045CE8
 */

/* MapEventCond_Army1UnitAtX7Y2's twin: a different row-pointer slot (rowOffset[1]), a different
 * byte in the record (unit[... + 12]), and it asks for top-two-bits == 1 instead of 0.
 * See MapEventCond_Army1UnitAtX7Y2 for why the cell is held in an `int`. */
int MapEventCond_Army2UnitAtX12Y1(void)
{
    int v;

    v = gMap->unit[gMap->rowOffset[1] + 12];

    if (v == 0)
        return 0;
    if ((v >> 6) == 1)
        return 1;

    return 0;
}
asm(".global sub_08045CE8\n.thumb_set sub_08045CE8, MapEventCond_Army2UnitAtX12Y1\n");
