#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087040.
 * sub_08087040 @ 0x08087040
 */

/*
 * MapSelectList_DrawPropertyIcons -- draw a row of four two-part sprites.
 *
 * Four sprites are drawn side by side, 0x18 pixels apart, the first at x 0x97.
 * Each is drawn in two halves: the top half at y 0 from the sprite data
 * gUnknown_0848B690, and the bottom half at y 0x10 from gUnknown_0848B6A8.
 *
 * PutSprite's last argument is the tile number with its attribute bits. The
 * tile numbers step by 8 from sprite to sprite, so the top halves use 0x50,
 * 0x58, 0x60 and 0x68 and the bottom halves 0x54, 0x5c, 0x64 and 0x6c. Bit
 * 0x2000 is set on all of them.
 */
void MapSelectList_DrawPropertyIcons(void)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        PutSprite(1, 0x97 + i * 0x18, 0, gUnknown_0848B690, (0x50 + i * 8) | 0x2000);
        PutSprite(1, 0x97 + i * 0x18, 0x10, gUnknown_0848B6A8, (0x54 + i * 8) | 0x2000);
    }
}
asm(".global sub_08087040\n.thumb_set sub_08087040, MapSelectList_DrawPropertyIcons\n");
