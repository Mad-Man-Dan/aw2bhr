#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019C24.
 * sub_08019C24 @ 0x08019C24
 */

void ClearBg0TilemapBuffer(void)
{
    u16 *p;
    int i;

    p = gBG0TilemapBuffer;

    for (i = 0; i < 0x400; i++)
        p[i] = 0;
}
asm(".global sub_08019C24\n.thumb_set sub_08019C24, ClearBg0TilemapBuffer\n");
