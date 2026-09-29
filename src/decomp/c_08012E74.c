#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012E74.
 * sub_08012E74 @ 0x08012E74
 */

void PutBg0Tile(u16 x, u16 y, u16 c)
{
    gBG0TilemapBuffer[y * 32 + x] = c | 0xC000;
}
asm(".global sub_08012E74\n.thumb_set sub_08012E74, PutBg0Tile\n");
