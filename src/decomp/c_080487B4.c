#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080487B4.
 * sub_080487B4 @ 0x080487B4
 */

/* Draw one gUnknown_0849EDB0 row into the tilemap a3 at cell (a1, a2): an icon
 * at column a1, a name field at a1 + 2 and a second field at a1 + 14, all on
 * the same row and all in palette a5.
 *
 * `(u16)(a5 * 0x1000)` and NOT `(a5 & 0xf) << 12`: the ROM's `lsl #0x1c;
 * lsr #0x10` is a net left shift of twelve whose 4-bit mask is only the u16
 * truncation of the product, and shorten_binary_op folds the multiply into two
 * instructions where the shift spelling needs three (docs/agbcc-codegen.md).
 *
 * `&a3[a2 * 32 + a1]` keeps the single `lsl #1` the ROM emits -- a3 is `u16 *`
 * so the element scale is applied once, after the row/column add. */
void DrawShopItemRow(u8 a1, u8 a2, u16 *a3, u16 a4, u16 a5)
{
    PutTilePoolGraphicTilemap(gUnknown_0849EDB0[a4].unk00, &a3[a2 * 32 + a1]);
    PutTextScriptImmediate(a1 + 2, a2, a3, gTextTable[gUnknown_0849EDB0[a4].unk02],
        (u16)(a5 * 0x1000), 0);
    DrawTallNumberRightAligned(a1 + 14, a2, a3, gUnknown_0849EDB0[a4].unk04,
        (u16)(a5 * 0x1000), 0);
}
asm(".global sub_080487B4\n.thumb_set sub_080487B4, DrawShopItemRow\n");
