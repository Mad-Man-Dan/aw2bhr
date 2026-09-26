#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801295C.
 * sub_0801295C @ 0x0801295C
 */

#include "hardware.h"

void sub_0801295C(void)
{
    gUnknown_030020B4.bits.vblank_int_enable = 1;
}
