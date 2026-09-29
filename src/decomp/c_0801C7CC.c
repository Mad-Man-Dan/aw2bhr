#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C7CC.
 * sub_0801C7CC @ 0x0801C7CC
 */

u16 AP_GetSignal(u16 *p)
{
    return p[0x14];
}
asm(".global sub_0801C7CC\n.thumb_set sub_0801C7CC, AP_GetSignal\n");
