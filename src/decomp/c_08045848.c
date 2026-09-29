#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045848.
 * sub_08045848 @ 0x08045848, sub_08045874 @ 0x08045874, sub_080458A0 @ 0x080458A0, sub_080458CC @ 0x080458CC, sub_080458F8 @ 0x080458F8
 */

/* One of five near-identical predicates over gMap: read a row offset from
 * gMap->rowOffset[], index gMap->terrain with it (plus a small fixed cell
 * offset), and test the top three bits of that byte for the value 1. All five
 * use `lsrs #5` and `cmp #1`; only the rowOffset slot and the cell offset
 * differ.
 *
 * `if (c) return 1; return 0;` and not `return c;` -- the ROM materialises
 * BOTH constants and puts its literal pool between the two return blocks,
 * which the short boolean form cannot produce. A leaf: no push, and the
 * epilogue is a bare `bx lr`. */
int MapEventCond_Army1OwnsCellX13Y5(void)
{
    if (gMap->terrain[gMap->rowOffset[5] + 13] >> 5 == 1)
        return 1;

    return 0;
}
asm(".global sub_08045848\n.thumb_set sub_08045848, MapEventCond_Army1OwnsCellX13Y5\n");

/* Sibling of the predicate above; only the rowOffset slot and cell offset differ. */
int MapEventCond_Army1OwnsCellX15Y2(void)
{
    if (gMap->terrain[gMap->rowOffset[2] + 15] >> 5 == 1)
        return 1;

    return 0;
}
asm(".global sub_08045874\n.thumb_set sub_08045874, MapEventCond_Army1OwnsCellX15Y2\n");

/* Sibling of the predicate above; only the rowOffset slot and cell offset differ. */
int MapEventCond_Army1OwnsCellX3Y8(void)
{
    if (gMap->terrain[gMap->rowOffset[8] + 3] >> 5 == 1)
        return 1;

    return 0;
}
asm(".global sub_080458A0\n.thumb_set sub_080458A0, MapEventCond_Army1OwnsCellX3Y8\n");

/* Sibling of the predicate above; only the rowOffset slot and cell offset differ. */
int MapEventCond_Army1OwnsCellX6Y8(void)
{
    if (gMap->terrain[gMap->rowOffset[8] + 6] >> 5 == 1)
        return 1;

    return 0;
}
asm(".global sub_080458CC\n.thumb_set sub_080458CC, MapEventCond_Army1OwnsCellX6Y8\n");

/* Fifth sibling: reads gMap->terrain[gMap->rowOffset[9]] with no cell offset. */
int MapEventCond_Army1OwnsCellX0Y9(void)
{
    if (gMap->terrain[gMap->rowOffset[9]] >> 5 == 1)
        return 1;

    return 0;
}
asm(".global sub_080458F8\n.thumb_set sub_080458F8, MapEventCond_Army1OwnsCellX0Y9\n");
