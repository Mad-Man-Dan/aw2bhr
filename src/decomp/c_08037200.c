#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037200.
 * sub_08037200 @ 0x08037200
 */

void PutRightAlignedNumberSpritesPlusSprite(u16 a, u16 b, u16 c, u16 d)
{
    PutRightAlignedNumberSprites(a, b, c, d);
    PutOamHi((a - 0x18) & 0x1ff, (b - 8) & 0xff, gUnknown_0848B698, d + 0x1014);
}
asm(".global sub_08037200\n.thumb_set sub_08037200, PutRightAlignedNumberSpritesPlusSprite\n");
