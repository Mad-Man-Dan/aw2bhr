#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F5D4.
 * sub_0803F5D4 @ 0x0803F5D4
 */

u8 sub_0803F5D4(struct Unk02028360 *p)
{
    return p - gUnknown_02028360;
}
