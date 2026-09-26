#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080129D4.
 * sub_080129D4 @ 0x080129D4
 */

void sub_080129D4(u32 seed)
{
    gUnknown_03001FD4 = seed;
}
