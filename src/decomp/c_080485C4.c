#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080485C4.
 * sub_080485C4 @ 0x080485C4
 */

int ShopMessage_NeedsFlag21(void)
{
    if (IsCampaignCompletionFlagSet(0x21) != 0)
        return 1;

    return 0;
}
asm(".global sub_080485C4\n.thumb_set sub_080485C4, ShopMessage_NeedsFlag21\n");
