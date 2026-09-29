#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080870B8.
 * sub_080870B8 @ 0x080870B8
 */

void MapSelectList_DrawPropertyCounts(int a, int b, int c, int d)
{
    if (a != -1 || b != a || c != b || d != c)
    {
        DrawSpriteNumberFont2(0x9f, 0x10, a);
        DrawSpriteNumberFont2(0xb7, 0x10, b);
        DrawSpriteNumberFont2(0xcf, 0x10, c);
        DrawSpriteNumberFont2(0xe7, 0x10, d);
    }
}
asm(".global sub_080870B8\n.thumb_set sub_080870B8, MapSelectList_DrawPropertyCounts\n");
