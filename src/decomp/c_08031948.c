#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031948.
 * sub_08031948 @ 0x08031948
 */

/* The army loop clears gUnknown_0849B018->unk0a[i] only for armies
 * SioIsPlayerLinked reports as gone, but clears gUnknown_0300449C[i] for all four.
 * Both stores carry the dead-`ldrb`-before-`strb` volatile tell.
 *
 * Wave 51, W51-B -- CORRECTION. This comment used to read the ABSENCE of that
 * tell on the gUnknown_03003F1C store two lines above as fixing that global as
 * NOT volatile. It does not: the dead load appears only where the assignment's
 * VALUE is consumed (a chained assignment), and this store is in statement
 * position. gUnknown_03003F1C IS volatile, decided by its only reader,
 * LinkWaitMapListBlocks, where the qualifier is worth 3 bytes. This function is
 * unchanged by it and still matches.
 *
 * The `strb r2` reusing SioIsPlayerLinked's returned zero rather than a fresh
 * `movs` is cse propagating the known value inside the `== 0` arm; the
 * gUnknown_0300449C store sits at the merge, where it is not known, and
 * materialises its own 0. Both are plain `= 0` in the source. */
void LinkMapListExchange_Start(void)
{
    int i;

    gUnknown_03003F1C = 0;

    for (i = 0; i < 4; i++)
    {
        if (!SioIsPlayerLinked(i))
            gUnknown_0849B018->unk0a[i] = 0;

        gUnknown_0300449C[i] = 0;
    }

    sub_08031824();

    gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 3, 2);
}
asm(".global sub_08031948\n.thumb_set sub_08031948, LinkMapListExchange_Start\n");
