#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F460.
 * sub_0802F460 @ 0x0802F460
 */

bool8 sub_0802F460(s8 index)
{
    if ((gUnknown_0849B018->unk09 >> index) & 1)
        return TRUE;

    return FALSE;
}
