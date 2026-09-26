#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045B18.
 * sub_08045B18 @ 0x08045B18
 */

int sub_08045B18(void)
{
    if (sub_08044374(1) == 1)
        return 1;

    return 0;
}
