#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038C08.
 * sub_08038C08 @ 0x08038C08
 */

/* Returns 0 when the move stack visits any cell twice, 1 when every step is
 * distinct -- the O(n^2) self-intersection test over the unk20/unk2c pair. */
int IsMovePathSelfAvoiding(void)
{
    s8 i;
    s8 j;

    for (i = gUnknown_0849D5F8->unk45; i > 0; i--)
    {
        for (j = i - 1; j >= 0; j--)
        {
            if (gUnknown_0849D5F8->unk20[i] == gUnknown_0849D5F8->unk20[j]
             && gUnknown_0849D5F8->unk2c[i] == gUnknown_0849D5F8->unk2c[j])
                return 0;
        }
    }

    return 1;
}
asm(".global sub_08038C08\n.thumb_set sub_08038C08, IsMovePathSelfAvoiding\n");
