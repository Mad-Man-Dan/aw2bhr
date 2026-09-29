#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080206B0.
 * sub_080206B0 @ 0x080206B0, FillMapBuffer @ 0x080206E4
 */

/* Fills one byte per map cell of `dst` with `value`, indexed through the map's
 * +0x417A row-offset table the way every other reader of this object does.
 *
 * The counters are `s16`: agbcc's PROMOTE_MODE keeps a short local ZERO-extended
 * (the `lsrs #0x10` that defines the variable) and sign-extends it again at each
 * use (`lsls #0x10; asrs #0x10`), which is why one `lsls` feeds both an `lsrs`
 * and an `asrs`. `int` counters lose all four shifts. The `adds r0, r5, 0x10000`
 * carrying `y << 16` is the loop optimiser's giv, not source. */
#define MAP gMap

/* Linear scan of the 0x5c-stride ROM table for the entry whose unk2c[0] equals
 * the caller's word; returns its index, or the first index past the 0xbf bound
 * when there is none.
 *
 * The `ldr r0,[r3,#0x2c]` before the loop is gcc's duplicate_loop_exit_test
 * copying the `while` condition to the top, not a hand-written special case for
 * entry 0 -- and the early exit MUST be spelled `return i;`.  Written as
 * `break;` the duplication does not happen at all (44 bytes, and the +0x2c
 * folds into the pool word's addend instead of the `adds r3, #0x2c` hoist). */
u16 FindMapIdByMapData(u32 a1)
{
    u16 i;

    i = 0;
    while ((u32)gUnknown_085C77A0[i].mapData[0] != a1)
    {
        i++;
        if (i > 0xbf)
            return i;
    }
    return i;
}
asm(".global sub_080206B0\n.thumb_set sub_080206B0, FindMapIdByMapData\n");


/* Wave 49, W49-K RETYPES the second parameter from `u16` to `int`, with the
 * narrowing moved into an explicit `u16` local -- byte-identical here (still
 * `lsls r1,#0x10; lsrs r7,#0x10` at entry) and REQUIRED by the newly matched
 * caller RebuildVisionPlanes, which passes `1 - gPlaySt.unk0d` with no
 * narrowing at all.  agbcc narrows a `u16` argument AT THE CALL SITE as well as
 * at entry (measured: +4 bytes per site, and LoadMapIntoGMap's `LoadMapData(a)`
 * shows the same pair on the caller side), so a `u16` prototype cannot produce
 * the ROM's caller.  The original almost certainly had no prototype in the
 * caller's translation unit -- default argument promotions passed an int and
 * the callee re-narrowed -- and since this tree has one shared header, spelling
 * the parameter `int` plus a local is the only way to say that once. */
void FillMapBuffer(u8 *dst, int value)
{
    s16 x, y;
    u16 v;

    v = value;

    for (y = 0; y < MAP->height; y++)
        for (x = 0; x < MAP->width; x++)
            dst[MAP->rowOffset[y] + x] = v;
}

asm(".global sub_080206E4\n.thumb_set sub_080206E4, FillMapBuffer\n");
