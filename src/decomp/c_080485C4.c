#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080485C4.
 * sub_080485C4 @ 0x080485C4
 */

int sub_080485C4(void)
{
    if (IsCampaignCompletionFlagSet(0x21) != 0)
        return 1;

    return 0;
}
