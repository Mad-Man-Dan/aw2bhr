#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B350.
 * sub_0803B350 @ 0x0803B350
 */

void sub_0803B350(u16 a)
{
    gUnknown_030005CC = a;
}
