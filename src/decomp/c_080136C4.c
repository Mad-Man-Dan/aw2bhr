#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080136C4.
 * sub_080136C4 @ 0x080136C4
 */

void sub_080136C4(void)
{
    int i;

    for (i = 0x1f; i >= 0; i--)
        gUnknown_0200B5F4[i] = 0;
}
