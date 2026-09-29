#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F480.
 * sub_0802F480 @ 0x0802F480
 */

bool8 SioIsPlayerSending(s8 index)
{
    if ((gUnknown_0849B018->unk08 >> index) & 1)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802F480\n.thumb_set sub_0802F480, SioIsPlayerSending\n");
