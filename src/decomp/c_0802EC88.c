#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802EC88.
 * sub_0802EC88 @ 0x0802EC88
 */

void sub_0802EC88(u16 a1, u16 a2, u16 a3, u16 a4)
{
    int n;

    n = 0;

    if (a1 == 0x7FFF || a1 == 0x5FFF)
        n++;

    if (a2 == 0x7FFF || a1 == 0x5FFF)
        n++;

    if (a3 == 0x7FFF || a1 == 0x5FFF)
        n++;

    if (a4 == 0x7FFF || a1 == 0x5FFF)
        n++;

    if (n > 0)
        SioStepSendDelay();
}
