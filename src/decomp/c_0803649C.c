#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803649C.
 * sub_0803649C @ 0x0803649C
 */

/* Scan the three-slot proc table gUnknown_03003124 for a free entry.
 * Returns the index, or -1 when all three are taken. The u16 counter is
 * read off the `lsls #0x10; lsrs #0x10` on the increment; the returned
 * value's `lsls #0x10; asrs #0x10` is the s16 return narrowing.
 */
s16 FindFreeMoveSlideSlot(void)
{
    u16 i;

    for (i = 0; i < 3; i++)
    {
        if (gUnknown_03003124[i] == 0)
            return i;
    }

    return -1;
}
asm(".global sub_0803649C\n.thumb_set sub_0803649C, FindFreeMoveSlideSlot\n");
