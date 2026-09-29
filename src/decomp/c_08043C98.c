#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043C98.
 * sub_08043C98 @ 0x08043C98
 */

u8 *GetUnlockedCoList(void)
{
    return gUnknown_020288A0;
}
asm(".global sub_08043C98\n.thumb_set sub_08043C98, GetUnlockedCoList\n");
