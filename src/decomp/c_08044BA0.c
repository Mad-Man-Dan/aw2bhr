#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044BA0.
 * sub_08044BA0 @ 0x08044BA0
 */

bool8 IsBlackHoleCo(int a1)
{
    int lo;

    lo = 0xa;
    if (a1 > 0xe)
        return 0;
    if (a1 < lo)
        return 0;
    return 1;
}
asm(".global sub_08044BA0\n.thumb_set sub_08044BA0, IsBlackHoleCo\n");
