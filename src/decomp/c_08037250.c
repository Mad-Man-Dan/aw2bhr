#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037250.
 * sub_08037250 @ 0x08037250, sub_08037258 @ 0x08037258
 */

u8 *GetWindowFrameTileData(void)
{
    return gUnknown_080D3FE4;
}
asm(".global sub_08037250\n.thumb_set sub_08037250, GetWindowFrameTileData\n");

u8 *sub_08037258(void)
{
    return gUnknown_080913BC;
}
