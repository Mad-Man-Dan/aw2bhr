#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C7CC.
 * sub_0801C7CC @ 0x0801C7CC
 */

u16 sub_0801C7CC(u16 *p)
{
    return p[0x14];
}
