#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08004D90.
 * sub_08004D90 @ 0x08004D90
 */

/* Passes PutTilePoolGraphicTilemap(9, ...) the BG0 tilemap cell at column 3,
 * tile row row*2 + 5 (side 0) or row*2 + 7 (side 1), the same rows
 * sub_08004DD4 computes, then calls sub_08004DD4 with the same
 * arguments and 2.
 *
 * Both arms computing the full pointer is what the ROM shows: the
 * `ldr =gBG0TilemapBuffer` literal is duplicated per arm and the
 * common tail (scale, deref, add) is cross-jumped after the merge.
 *
 * The (u32) on the tile index is load-bearing: with a signed index
 * agbcc distributes the u16 scale over it and emits `lsls #6;
 * adds #6` instead of the ROM's `lsls #5; adds #3; lsls #1`. */
void sub_08004D90(int side, int row, u8 *text)
{
    u16 *tm;
    int y = row * 2;

    if (side == 0)
        tm = gBG0TilemapBuffer + (u32)((y + 5) * 32 + 3);
    else
        tm = gBG0TilemapBuffer + (u32)((y + 7) * 32 + 3);

    PutTilePoolGraphicTilemap(9, tm);
    sub_08004DD4(side, row, text, 2);
}
