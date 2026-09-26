#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801224C.
 * sub_0801224C @ 0x0801224C
 */

void sub_0801224C(u16 a, u16 b)
{
    gUnknown_03000044 = a;
    gUnknown_03000046 = b;
}
