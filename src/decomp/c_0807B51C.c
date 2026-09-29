#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807B51C.
 * sub_0807B51C @ 0x0807B51C
 */

void PutSpriteNumberRightAligned(int x, int y, int value, int idx)
{
    while (value != 0) {
        PutSprite(0, x, y, gUnknown_0848B690,
                  (gUnknown_0861617C[idx] + DivRem(value, 10) * 4) | 0x1000);
        value = Div(value, 10);
        x -= 0xc;
    }
}
asm(".global sub_0807B51C\n.thumb_set sub_0807B51C, PutSpriteNumberRightAligned\n");
