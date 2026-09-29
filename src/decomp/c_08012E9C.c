#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012E9C.
 * sub_08012E9C @ 0x08012E9C
 */

void PutBg0AsciiChar(u16 x, u16 y, u8 c)
{
    if (c == 0x2e || c == 0x2c)
        PutBg0Tile(x, y, 0x3c);
    else if (c == 0x2d || c == 0x3d || c == 0x5f)
        PutBg0Tile(x, y, 0x3d);
    else if (c == 0x28)
        PutBg0Tile(x, y, 0x3a);
    else if (c == 0x29)
        PutBg0Tile(x, y, 0x3b);
    else if (c == 0x3a)
        PutBg0Tile(x, y, 0x3d);
    else if (c == 0x20)
        PutBg0Tile(x, y, 0x3f);
    else if (c == 0x2f)
        PutBg0Tile(x, y, 0x3d);
    else if (c == 0x25)
        PutBg0Tile(x, y, 0x3e);
    else if (c > 0x60)
        PutBg0Tile(x, y, c + 0xffbf);
    else if (c > 0x40)
        PutBg0Tile(x, y, c + 0xffdf);
    else
        PutBg0Tile(x, y, c + 0xffe0);
}
asm(".global sub_08012E9C\n.thumb_set sub_08012E9C, PutBg0AsciiChar\n");
