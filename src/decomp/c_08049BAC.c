#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049BAC.
 * sub_08049BAC @ 0x08049BAC
 */

void ShopScreen_ScrollBackgroundHook(void)
{
    if (gGameClock & 1)
    {
        gUnknown_0300200C++;
        gUnknown_03002000--;
    }
}
asm(".global sub_08049BAC\n.thumb_set sub_08049BAC, ShopScreen_ScrollBackgroundHook\n");
