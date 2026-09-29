#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F4A0.
 * sub_0802F4A0 @ 0x0802F4A0
 */

/* True when the number of unk0a slots holding 5 matches the population of the
 * unk09 mask -- 3/2, 7/3, 0xf/4. Three tests, one `return TRUE`.
 *
 * unk09 is re-read at every test because it is volatile, but the RECORD
 * POINTER is loaded once and held in r1 across all three. Naming
 * gUnknown_0849B018 three times reloads the whole chain each time (a volatile
 * access kills agbcc's CSE of every other MEM), so the pointer is bound to a
 * local -- and bound AFTER the loop, because the loop's own volatile reads are
 * what stop the two loads being merged into one. */
bool8 SioAreAllLinkedPlayersStatus5(void)
{
    struct Unk0849B018 *p;
    int count;
    int i;

    count = 0;
    for (i = 0; i <= 3; i++)
    {
        if (gUnknown_0849B018->unk0a[i] == 5)
            count++;
    }

    p = gUnknown_0849B018;

    if (p->unk09 == 3 && count == 2)
        return TRUE;
    if (p->unk09 == 7 && count == 3)
        return TRUE;
    if (p->unk09 == 0xf && count == 4)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802F4A0\n.thumb_set sub_0802F4A0, SioAreAllLinkedPlayersStatus5\n");
