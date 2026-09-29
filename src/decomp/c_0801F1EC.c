#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F1EC.
 * sub_0801F1EC @ 0x0801F1EC
 */

/* Finds the gUnknown_0200F920 entry whose unk02 is the tile id a1 and
 * re-uploads its graphics to a2. The count unk05 is re-read from memory on
 * every iteration, so it is a MEMBER read in the loop condition and not a
 * hoisted local. The `b` out of the loop body after the call is a plain
 * `return`: the loop is counted and its exit test is NOT strength-reduced,
 * which is the wave-38 discriminator between `break`/`return` and `continue`. */
void ReloadTilePoolGraphic(int a1, int a2)
{
    struct Unk0200F920 *e;
    int i;

    e = &gUnknown_0200F920[GetTilePoolForGraphic(a1)];
    for (i = 0; i < e->unk05; i++) {
        if (e->unk08[i].unk02 == a1) {
            CopyGraphicToPoolTiles(a2, e->unk00, e->unk08[i].unk00);
            return;
        }
    }
}
asm(".global sub_0801F1EC\n.thumb_set sub_0801F1EC, ReloadTilePoolGraphic\n");
