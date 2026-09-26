#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804415C.
 * sub_0804415C @ 0x0804415C
 */

u8 sub_0804415C(int a1)
{
    return gPlayers[a1].coMode != 0;
}
