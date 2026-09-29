#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E4B0.
 * sub_0801E4B0 @ 0x0801E4B0
 */

int QueueSpriteRequestSimple(int a1, int a2, int a3, int a4, int a5)
{
    gUnknown_0200ED20[gUnknown_03002510].unk00 = a2;
    gUnknown_0200ED20[gUnknown_03002510].unk02 = a3;
    gUnknown_0200ED20[gUnknown_03002510].unk04 = 0x40000000 | a4;
    gUnknown_0200ED20[gUnknown_03002510].unk08 = a5;
    if (sub_0801A718(&gUnknown_0200ED20[gUnknown_03002510], a1) == -1)
        return 1;
    gUnknown_03002510++;
    return 0;
}
asm(".global sub_0801E4B0\n.thumb_set sub_0801E4B0, QueueSpriteRequestSimple\n");
