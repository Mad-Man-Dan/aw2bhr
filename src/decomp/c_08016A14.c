#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016A14.
 * sub_08016A14 @ 0x08016A14
 */

void sub_08016A14(void)
{
    if ((gUnknown_0200C420.unk08 & 1) == 0)
        gUnknown_0200C420.unk08++;
}
