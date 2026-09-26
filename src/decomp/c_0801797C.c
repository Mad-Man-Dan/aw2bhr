#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801797C.
 * sub_0801797C @ 0x0801797C, sub_08017988 @ 0x08017988
 */

void sub_0801797C(void)
{
    gUnknown_03002B38 = 0;
}

s16 sub_08017988(void)
{
    return gUnknown_03002B38;
}
