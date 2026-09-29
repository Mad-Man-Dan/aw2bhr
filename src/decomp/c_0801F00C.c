#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F00C.
 * sub_0801F00C @ 0x0801F00C
 */

void EnableSpriteLayerMode(void)
{
    gUnknown_03001FE0 = 1;
}
asm(".global sub_0801F00C\n.thumb_set sub_0801F00C, EnableSpriteLayerMode\n");
