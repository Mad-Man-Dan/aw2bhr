#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087298.
 * sub_08087298 @ 0x08087298
 */

u16 GetMapPreviewScrollY(void)
{
    if (gPlaySt.gameMode == 2)
        return gUnknown_030058F4 * 2 - 0x54;

    return gUnknown_030058F4 * 2 - 0x5f;
}
asm(".global sub_08087298\n.thumb_set sub_08087298, GetMapPreviewScrollY\n");
