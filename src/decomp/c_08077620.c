#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077620.
 * sub_08077620 @ 0x08077620
 */

/* Draws the two halves of a banner plus its icon, each behind its own bounds
 * check: the pair at x 0x80 / 0x78 only while the first argument is on screen,
 * and the PutCoMinimugSprite sprite plus its list only while the second is.
 *
 * The second check is UNSIGNED (`cmp #0xae; bhi`) on `b + 0xf`, i.e. the single
 * comparison that covers both ends of the range -- the first check is a plain
 * signed `bgt`, so the two are deliberately different tests and not a pair. */
void WorldMapMissionInfo_PutSprites(int a, int b)
{
    int y;

    y = b * 2 - 0x60;

    if (a <= 0xef)
    {
        PutSprite(1, y, 0x80, gUnknown_081CC5D0, 0);
        PutSprite(1, y, 0x78, gUnknown_081CC5DE, 0);
    }

    if ((u32)(b + 0xf) <= 0xae)
    {
        PutCoMinimugSprite(a + 0x18, b + 0x10, 0x2098, 0);
        PutSprite(0, 0x50, b - 0x10, gUnknown_081CC5B0, 0);
    }
}
asm(".global sub_08077620\n.thumb_set sub_08077620, WorldMapMissionInfo_PutSprites\n");
