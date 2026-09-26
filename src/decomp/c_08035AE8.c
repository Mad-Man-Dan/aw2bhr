#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035AE8.
 * sub_08035AE8 @ 0x08035AE8
 */

s16 sub_08035AE8(s16 a)
{
    return gUnknown_08090EAC[a & 1];
}
