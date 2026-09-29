#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045EAC.
 * sub_08045EAC @ 0x08045EAC
 */

/* Three sequential calls, all results discarded. */
void sub_08045EAC(void)
{
    StartScreenShake(2, 0x8C, 0);
    StartWhiteFlash(2, 0, 0xB4, 0);
    sub_0803B4DC(0x1D5);
}
