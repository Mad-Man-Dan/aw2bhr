#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B744.
 * sub_0804B744 @ 0x0804B744
 */

/* The two gUnknown_085D6A48 rows must be bound to pointer locals AND bound
 * INTERLEAVED with their first uses: the ROM computes row 0's address, tests
 * it, then computes row 1's address. Hoisting both binds to the top (the
 * obvious spelling) emits the two address chains back to back, and inlining
 * the subscript instead hoists the `+8` onto the gUnknown_085D6A48 base and
 * loses the `ldrh [rN,#8]` displacement. */
void SetUpBattleBackdrops(u16 a, u16 b)
{
    u16 *e0;
    u16 *e1;

    if (gUnknown_03004504.bit0 == 0)
    {
        a = GetBattleBackdropId(a, gUnknown_03004528[0], 0);
        b = GetBattleBackdropId(b, gUnknown_03004528[1], 0);
    }

    b = GetBackdropVariantForSecondSide(a, b);

    e0 = gUnknown_085D6A48[gUnknown_03004580[0][1]];
    if (e0[4] == 3)
        a = 0x33;
    e1 = gUnknown_085D6A48[gUnknown_03004580[1][1]];
    if (e1[4] == 3)
        b = 0x33;
    if (e0[1] == 2)
        a = 7;
    if (e1[1] == 2)
        b = 7;

    InitPanelScrollDrivers(a, b);
    LoadBattleBackdropArt(0, a, (void *)0x06008000, (void *)0x06003000,
        (void *)0x05000020, (void *)0x06017C00, (void *)0x05000380);
    LoadBattleBackdropArt(1, b, (void *)0x0600C000, (void *)0x06003800,
        (void *)0x05000080, (void *)0x06017E00, (void *)0x050003A0);
}
asm(".global sub_0804B744\n.thumb_set sub_0804B744, SetUpBattleBackdrops\n");
