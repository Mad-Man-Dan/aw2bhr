#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800056C.
 * sub_0800056C @ 0x0800056C
 */

void sub_0800056C(u16 a)
{
    gActiveMap->mode = a;
    gActiveMap->stateChanged = 1;
}
