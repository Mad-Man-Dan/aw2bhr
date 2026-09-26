#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015BD0.
 * sub_08015BD0 @ 0x08015BD0
 */

s8 sub_08015BD0(s32 a)
{
    u8 i;

    for (i = 0; i < 30; i++)
    {
        if (gUnknown_03001470[i].unk00 == a)
            return i;
    }

    return -1;
}
