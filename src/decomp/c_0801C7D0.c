#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C7D0.
 * sub_0801C7D0 @ 0x0801C7D0
 */

u16 sub_0801C7D0(u16 *p)
{
    u16 r;

    r = p[0x14];
    p[0x14] = 0;

    return r;
}
