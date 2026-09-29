#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044854.
 * sub_08044854 @ 0x08044854
 */

/* MATCHED. Byte-for-byte the same function as DamageUnitAtCell. Read that one --
 * the comma chain in the index expression is a position readout that no
 * statement boundary reaches. */
void DamageUnitAtCellCopy(int x, int y, int c)
{
    int idx;
    struct Unit *u;
    struct Unit **pp;

    if (x < 0)
        return;
    if (y < 0)
        return;

    if (x >= gMap->width)
        return;
    if (y >= gMap->height)
        return;

    u = &gUnits[(pp = &gUnits,
        idx = gMap->rowOffset[y] + x,
        gMap->unitUnk[idx])];

    if (u->type == 0)
        return;
    if ((u->flags & 8) != 0)
        return;

    if (u->hp < c + 1)
        u->hp = 1;
    else
        u->hp = u->hp - c;
}
asm(".global sub_08044854\n.thumb_set sub_08044854, DamageUnitAtCellCopy\n");
