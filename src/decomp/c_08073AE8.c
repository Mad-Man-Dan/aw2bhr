#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073AE8.
 * sub_08073AE8 @ 0x08073AE8
 */

void SwapWipeEdgeBuffers(void)
{
    void * temp = gUnknown_0202FDDC;

    gUnknown_0202FDDC = gUnknown_0202FDE0;
    gUnknown_0202FDE0 = temp;
}
asm(".global sub_08073AE8\n.thumb_set sub_08073AE8, SwapWipeEdgeBuffers\n");
