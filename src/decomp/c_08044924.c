#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044924.
 * sub_08044924 @ 0x08044924, sub_08044940 @ 0x08044940
 */

/* The `(void *)` casts are the price of sub_08011B34/sub_08011B5C taking the
 * list entry as `void *` -- C89 needs one to hand it a function. */
void sub_08044924(void)
{
    sub_08011B5C((void *)sub_080246B4);
    sub_08011B5C((void *)sub_08024720);
}

void sub_08044940(void)
{
    sub_08011B34((void *)sub_080246B4);
    sub_08011B34((void *)sub_08024720);
}
