#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080F0C.
 * sub_08080F0C @ 0x08080F0C
 */

void sub_08080F0C(void)
{
    if (IsBlackHoleCo(gUnknown_03005970))
        sub_0803B524(0x1C8);
    else
        sub_0803B524(0x1C5);
}
