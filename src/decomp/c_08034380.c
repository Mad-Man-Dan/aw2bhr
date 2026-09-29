#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034380.
 * sub_08034380 @ 0x08034380
 */

/* The `subs #1` happens in u8 width (`lsls #24; lsrs #24` after it), so the
 * temporary is a u8 and not an int; an int temp would compare against a
 * sign-extended value and lose the truncation pair. */
int IsLinkCommandIdValid(u8 *p)
{
    u8 v = *p - 1;

    if (v > 0x13)
        return 0;

    return 1;
}
asm(".global sub_08034380\n.thumb_set sub_08034380, IsLinkCommandIdValid\n");
