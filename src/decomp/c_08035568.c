#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035568.
 * sub_08035568 @ 0x08035568
 */

void ClearMoveSlideSlots(void)
{
    u16 i;

    for (i = 0; i < 3; i++)
        gUnknown_03003124[i] = 0;
}
asm(".global sub_08035568\n.thumb_set sub_08035568, ClearMoveSlideSlots\n");
