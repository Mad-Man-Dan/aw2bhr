#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08000654.
 * sub_08000654 @ 0x08000654
 */

void sub_08000654(void)
{
    gActiveMap->state = 1;
}
