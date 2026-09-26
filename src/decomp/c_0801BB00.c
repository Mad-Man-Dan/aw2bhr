#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BB00.
 * sub_0801BB00 @ 0x0801BB00
 */

void sub_0801BB00(int index, void *handler)
{
    gUnknown_03002FE0[index] = handler;
}
