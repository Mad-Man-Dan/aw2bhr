#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014ED4.
 * sub_08014ED4 @ 0x08014ED4
 */

/* The free half of the HeapMalloc pair; no early-out block because there is
 * nothing to return. `pop {r0}; bx r0` makes it void. */
void HeapFree(void *p)
{
    if (gUnknown_03000050 != -1)
        HeapFreeBlock(gUnknown_03000050, p);
}
asm(".global sub_08014ED4\n.thumb_set sub_08014ED4, HeapFree\n");
