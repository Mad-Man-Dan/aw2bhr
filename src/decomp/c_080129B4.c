#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080129B4.
 * sub_080129B4 @ 0x080129B4
 */

#include "hardware.h"

void sub_080129B4(void)
{
    gUnknown_03002B2C = 1 - gUnknown_03002B2C;

    REG_BLDCNT = (gUnknown_03002B2C << 6) | 0x8f;
}
