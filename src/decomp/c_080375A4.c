#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080375A4.
 * sub_080375A4 @ 0x080375A4
 */

void sub_080375A4(u8 a)
{
    BuildMapListForCategory(gUnknown_08090EF0[a]);
}
