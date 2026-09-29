#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804931C.
 * sub_0804931C @ 0x0804931C
 */

void RedrawSelectedShopRow(void)
{
    DrawShopItemRow(0,
                 (gUnknown_084C30F8->unk01e - gUnknown_084C30F8->unk020) * 2 + 7,
                 gBG0TilemapBuffer,
                 gUnknown_02028E1C[gUnknown_084C30F8->unk01e],
                 3);
    BG_EnableSyncBG0();
}
asm(".global sub_0804931C\n.thumb_set sub_0804931C, RedrawSelectedShopRow\n");
