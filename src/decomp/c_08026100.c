#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026100.
 * sub_08026100 @ 0x08026100
 */

/* MATCHED, and the first of a byte-identical pair with DamageUnitAtCellCopy. Applies
 * `c` points of damage to the unit standing on cell (x, y), flooring at 1:
 * bounds-check the cell against gMap's width/height, look the
 * unit id up through gMap->unitUnk, and clamp.
 *
 * The unit id is gMap->unitUnk[gMap->rowOffset[y] + x]; binding `idx` before
 * the plane read orders the two adds the way the ROM has them.
 *
 * THE COMMA CHAIN IS NOT DECORATION and it is the whole cost of this function.
 * Three separate facts are read off the ROM and every one of them needs a
 * reference at a position no statement boundary can reach:
 *
 *  - `pp = &gUnits` is the wave-17 anchor. gUnits is a
 *    POINTER global whose address is materialised right after the height test
 *    and whose `ldr` is issued last; `pp` is never read, and what matters is
 *    only that the reference precedes `t = y * 2`. Without it the address load
 *    lands five instructions late, the height goes to r2 instead of r6 and the
 *    frame loses r6. An anchor spelled `u = gUnits;` does NOT work
 *    -- that is a pointer LOAD (`ldr addr; ldr [addr]`), and the ROM's deref is
 *    at the end.
 *  - `idx = ...` sits inside the index expression, ahead of the plane read.
 *
 * Every neighbouring spelling misses by exactly one slot, which is what makes
 * this a readout rather than a lucky arrangement -- see the wave-20 section in
 * docs/agbcc-codegen.md for the table. */
void DamageUnitAtCell(int x, int y, int c)
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
asm(".global sub_08026100\n.thumb_set sub_08026100, DamageUnitAtCell\n");
