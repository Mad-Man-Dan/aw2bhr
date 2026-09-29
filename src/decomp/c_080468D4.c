#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080468D4.
 * sub_080468D4 @ 0x080468D4
 */

/* Convert a pixel offset to a tile column, draw one 8x8 cell, then flush.
 *
 * `if (x < 0) x += 7` followed by `lsls #0xd; lsrs #0x10` is a SIGNED divide by
 * 8, not a mask: the bias makes the shift round toward zero, and the shift pair
 * reads as `(u32)v << 13 >> 16`, which is `v / 8` truncated to 16 bits. The
 * truncation is FillTilemapRect's u16 parameter fusing with the divide, so no cast
 * belongs in the source. */
void ClearTerrainInfoMoveCosts(int x)
{
    FillTilemapRect(gBG0TilemapBuffer, (x + gUnknown_084C211C[0] * 8) / 8,
                 gUnknown_084C211C[1], 8, 8, 0);
    BG_EnableSyncBG0();
}
asm(".global sub_080468D4\n.thumb_set sub_080468D4, ClearTerrainInfoMoveCosts\n");
