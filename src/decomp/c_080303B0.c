#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080303B0.
 * sub_080303B0 @ 0x080303B0
 */

bool8 IsLinkKeySyncMode(void)
{
    if (gUnknown_0849B018->unk01 != 2)
        return FALSE;

    return TRUE;
}
asm(".global sub_080303B0\n.thumb_set sub_080303B0, IsLinkKeySyncMode\n");
