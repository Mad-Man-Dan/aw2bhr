#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080620C0.
 * sub_080620C0 @ 0x080620C0
 */

void AiClearInterestLists(void)
{
    int i;
    int j;

    for (i = 0; i <= 2; i++)
        for (j = 0; j <= 0x1f; j++)
            gUnknown_02029ED8[j + i * 0x20 + gUnknown_030033EC * 0xc00] = 0;
}
asm(".global sub_080620C0\n.thumb_set sub_080620C0, AiClearInterestLists\n");
