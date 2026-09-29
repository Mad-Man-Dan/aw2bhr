#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C8A0.
 * sub_0806C8A0 @ 0x0806C8A0
 */

void ClearCreditsPageArea(void)
{
    int i;
    int j;

    for (i = 0; i <= 0x13; i++)
        for (j = 8; j <= 0x1d; j++)
            gBG0TilemapBuffer[i * 32 + j] = 0;

    BG_EnableSyncBG0();
}
asm(".global sub_0806C8A0\n.thumb_set sub_0806C8A0, ClearCreditsPageArea\n");
