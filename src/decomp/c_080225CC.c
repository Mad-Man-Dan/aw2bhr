#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080225CC.
 * sub_080225CC @ 0x080225CC
 */

/* The gBG2TilemapBuffer twin of ClearUnitTileQuadAt -- same 2x2 block, same address
 * form (see the comment there), but filled with 0x360, the same value
 * ClearBg2Tilemap clears that whole tilemap to. */
void ClearUnitIconTileQuadAt(u16 x, u16 y)
{
    int cx;
    int cy;

    cx = (x - gMap->camX) & 0xf;
    cy = (y - gMap->camY) & 0xf;

    *(gBG2TilemapBuffer + cx * 2 + cy * 64) = 0x360;
    *(gBG2TilemapBuffer + cx * 2 + cy * 64 + 1) = 0x360;
    *(gBG2TilemapBuffer + cx * 2 + cy * 64 + 0x20) = 0x360;
    *(gBG2TilemapBuffer + cx * 2 + cy * 64 + 0x21) = 0x360;
}
asm(".global sub_080225CC\n.thumb_set sub_080225CC, ClearUnitIconTileQuadAt\n");
