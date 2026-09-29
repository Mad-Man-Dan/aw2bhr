#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804247C.
 * IsTerrainSilo @ 0x0804247C
 */

/* The c_08001158.c map-cell idiom: gUnknown_08499590 is a `u8 *` whose +0x417A
 * is a table of per-row halfword offsets and whose +0x1432 is the cell array
 * this one reads (c_08001158.c uses +0xA22 for tiles). The four locals are what
 * keep the two constants in their own pool words and the adds on the base,
 * rather than folding into the load displacement. */

/* Wave 38 (W38-K): return retyped `int` -> `bool8`. Byte-neutral here (the body
 * returns literal 0/1), but the sole caller BuildSiloCellList re-narrows the result
 * with `lsls #0x18; lsrs #0x18` before `cmp #1`, which agbcc emits only for a
 * narrow-returning callee. See include/unknown-functions.h. */
bool8 IsTerrainSilo(s16 a, s16 b)
{
    int off;

    off = gMap->rowOffset[b] + a;

    if ((gMap->terrain[off] & 0x1f) == TERRAIN_SILO)
        return 1;
    else
        return 0;
}

asm(".global sub_0804247C\n.thumb_set sub_0804247C, IsTerrainSilo\n");
