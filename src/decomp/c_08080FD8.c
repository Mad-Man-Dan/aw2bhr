#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080FD8.
 * sub_08080FD8 @ 0x08080FD8
 */

void sub_08080FD8(u16 *p)
{
    p[0x32] = 0;
}
