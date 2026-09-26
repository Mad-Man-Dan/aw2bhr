#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C62C.
 * sub_0802C62C @ 0x0802C62C
 */

bool8 sub_0802C62C(void)
{
    if (gPlaySt.savingEnabled == 0)
        return FALSE;

    return TRUE;
}
