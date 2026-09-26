#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057D44.
 * sub_08057D44 @ 0x08057D44
 */

u32 sub_08057D44(int a1, int a2)
{
    return gUnknown_08555450[a2][a1];
}
