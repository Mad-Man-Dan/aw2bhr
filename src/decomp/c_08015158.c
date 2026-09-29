#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015158.
 * sub_08015158 @ 0x08015158
 */

/* Counts the free slots -- the same 30-slot scan InitSlotScripts clears and
 * FindSlotScript searches, with a `u8` accumulator. A leaf: no `push`, and the
 * count comes back in r0 already zero-extended, so nothing here narrows the
 * return type.
 */
int CountFreeSlotScripts(void)
{
    u8 n;
    u8 i;

    n = 0;

    for (i = 0; i < 30; i++)
    {
        if (gUnknown_03001470[i].unk00 == 0)
            n++;
    }

    return n;
}
asm(".global sub_08015158\n.thumb_set sub_08015158, CountFreeSlotScripts\n");
