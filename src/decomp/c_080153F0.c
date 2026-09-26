#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080153F0.
 * sub_080153F0 @ 0x080153F0
 */

bool8 sub_080153F0(s16 a)
{
    if (gUnknown_03001470[a].unk00 != 0)
        return 1;

    return 0;
}
