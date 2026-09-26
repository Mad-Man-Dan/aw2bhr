#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080198B8.
 * sub_080198B8 @ 0x080198B8
 */

void sub_080198B8(bool8 (*func)(void))
{
    gUnknown_03001FF0 = func;
}
