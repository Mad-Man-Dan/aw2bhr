#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804BA4C.
 * sub_0804BA4C @ 0x0804BA4C
 */

void sub_0804BA4C(void)
{
    u16 i;

    for (i = 0; i < 0x28; i++)
        ;

    gUnknown_03002F18 = gUnknown_03002F18;
}
