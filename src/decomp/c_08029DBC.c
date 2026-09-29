#include "global.h"
#include "map.h"
/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029DBC.
 * sub_08029DBC @ 0x08029DBC
 */

/* Is the cell (x, y) inside the visible 15x10 window? The camera origin is the
 * pair of halfwords at +4 and +6 of the screen descriptor, in sixteenths of a
 * cell, hence the `>> 4`.
 *
 * `(s16)*(u16 *)` and not `*(s16 *)`: a genuine s16 load is the reg+reg
 * `ldrsh`, two instructions; the ROM has `ldrh` plus `lsls #0x10; asrs #0x14`,
 * which is a u16 read cast to s16 and shifted in one fused pair.
 *
 * Written as a chain of `||` early-outs returning 0, NOT as `&&` returning 1.
 * Both are the same four compares, but the `&&` form lays `movs r0,#1` out
 * first and the ROM has `movs r0,#0` first. The bounds are recomputed inside
 * the chain rather than bound to locals up front: locals hoist the second
 * `ldrh` above the first compare and cost the extra register copy. */
int IsCellOnScreen(int x, int y)
{
    if (x < (gMap->scrollX >> 4)
     || y < (gMap->scrollY >> 4)
     || x > (gMap->scrollX >> 4) + 0xE
     || y > (gMap->scrollY >> 4) + 9)
        return 0;

    return 1;
}
asm(".global sub_08029DBC\n.thumb_set sub_08029DBC, IsCellOnScreen\n");
