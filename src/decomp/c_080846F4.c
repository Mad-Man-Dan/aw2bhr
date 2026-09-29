#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080846F4.
 * sub_080846F4 @ 0x080846F4
 */

u8 GetHardCampaignToggle(void)
{
    return gUnknown_03005968;
}
asm(".global sub_080846F4\n.thumb_set sub_080846F4, GetHardCampaignToggle\n");
