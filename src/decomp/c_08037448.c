#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037448.
 * sub_08037448 @ 0x08037448
 */

/* Collects every gUnknown_085C77A0 row whose unk1a equals the requested id and
 * that IsMapSelectableForSlots accepts, into gUnknown_02027F78, then arms the
 * gUnknown_02027F74 cursor pair over the result. Returns whether anything was
 * found.
 *
 * The comparison is written `gUnknown_085C77A0[i].unk1a == a1` with a1 a plain
 * `u8` parameter -- NOT via a `u16` local. agbcc narrows the compare to the
 * member's HImode and re-extends the u8 side, and LICM hoists the invariant
 * half, which is the ROM's `lsls r6, r6, #0x10` in the preheader against
 * `lsrs r0, r6, #0x10` in the body. A `u16` local instead makes the conversion
 * free, costs a second pseudo and loses both shifts.
 *
 * The `count > 0x31` bail sits AFTER the body and BEFORE the `i` increment, so
 * it is a `break` at the end of the loop body rather than part of the `for`
 * condition. */
u8 BuildMapListForCategory(u8 a1)
{
    u8 count;
    u8 i;
    u8 v;

    v = sub_08026340();
    if (gPlaySt.gameMode != 2 && gPlaySt.gameMode != 4
        && IsMapCategoryUnlocked(a1) == 0)
        return 0;

    count = 0;
    for (i = 0; i < 0xc0; i++)
    {
        if (gUnknown_085C77A0[i].category == a1 && IsMapSelectableForSlots(i, v))
            gUnknown_02027F78[count++] = i;
        if (count > 0x31)
            break;
    }

    if (count == 0)
        return 0;
    gUnknown_02027F74.unk36 = 0;
    gUnknown_02027F74.unk37 = count - 1;
    return 1;
}
asm(".global sub_08037448\n.thumb_set sub_08037448, BuildMapListForCategory\n");
