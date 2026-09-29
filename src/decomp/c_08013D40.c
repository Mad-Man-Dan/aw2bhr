#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013D40.
 * sub_08013D40 @ 0x08013D40
 */

void ClearTextSkipFlag(void)
{
    gUnknown_03002514 = 0;
}
asm(".global sub_08013D40\n.thumb_set sub_08013D40, ClearTextSkipFlag\n");
