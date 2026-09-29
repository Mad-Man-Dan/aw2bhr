#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080616F0.
 * sub_080616F0 @ 0x080616F0
 */

void ClearBg3TilemapBuffer(void)
{
    u16 i;

    for (i = 0; i < 0x400; i++)
        gBG3TilemapBuffer[i] = 0;
}
asm(".global sub_080616F0\n.thumb_set sub_080616F0, ClearBg3TilemapBuffer\n");
