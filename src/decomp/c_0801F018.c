#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F018.
 * sub_0801F018 @ 0x0801F018
 */

void DisableSpriteLayerMode(void)
{
    gUnknown_03001FE0 = 0;
}
asm(".global sub_0801F018\n.thumb_set sub_0801F018, DisableSpriteLayerMode\n");
