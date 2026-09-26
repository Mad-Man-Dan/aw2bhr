#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C660.
 * sub_0802C660 @ 0x0802C660
 */

bool8 sub_0802C660(void)
{
    if (gUnknown_085C77A0[gPlaySt.mapID].category == 0)
        return TRUE;

    return FALSE;
}
