#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080223E0.
 * sub_080223E0 @ 0x080223E0
 */

/* Clears the 2x2 tile block that (x, y) falls in, on the gBG1TilemapBuffer
 * tilemap. gUnknown_08499590 +0x0c / +0x0e is the scroll origin; the `& 0xf`
 * wraps into the 32x32 map.
 *
 * Three things are load-bearing and each was a near-miss on its own:
 *   - cx/cy have to be locals, or the tilemap's pool word hoists to the top
 *     of the function (the whole address becomes one expression);
 *   - the address has to be the ADDITIVE form `p + cx*2 + cy*64`, not
 *     `p[cx*2 + cy*64]` -- the subscript combines the two scales into three
 *     shifts instead of two;
 *   - there must be NO pointer local. Written out as four full store
 *     expressions, CSE builds the shared address with both `lsls` ahead of
 *     both `adds` and the base folded into the cx term; assigning the address
 *     to a `u16 *p` first interleaves them and accumulates into the base's
 *     register instead. Same length either way, so only the bytes notice.
 */
void ClearUnitTileQuadAt(u16 x, u16 y)
{
    int cx;
    int cy;

    cx = (x - gMap->camX) & 0xf;
    cy = (y - gMap->camY) & 0xf;

    *(gBG1TilemapBuffer + cx * 2 + cy * 64) = 0;
    *(gBG1TilemapBuffer + cx * 2 + cy * 64 + 1) = 0;
    *(gBG1TilemapBuffer + cx * 2 + cy * 64 + 0x20) = 0;
    *(gBG1TilemapBuffer + cx * 2 + cy * 64 + 0x21) = 0;
}
asm(".global sub_080223E0\n.thumb_set sub_080223E0, ClearUnitTileQuadAt\n");
