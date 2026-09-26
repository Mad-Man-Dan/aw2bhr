#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045EE0.
 * sub_08045EE0 @ 0x08045EE0, sub_08045EF4 @ 0x08045EF4
 */

void sub_08045EE0(void)
{
    sub_080130DC(0x1e, 0, 0, 0);
}

void sub_08045EF4(void)
{
    sub_080130DC(0, 0, 0x3c, 0);
}
