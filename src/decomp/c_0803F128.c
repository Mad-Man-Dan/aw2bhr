#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F128.
 * sub_0803F128 @ 0x0803F128
 */

/* Picks one of two ROM blobs off a mode id; GetCannonFireTileGraphic immediately above is
 * the same shape over a different pair. Two `beq`s with the default falling
 * into case 3's own block is a switch whose `case 3:` and `default:` share one
 * arm -- writing them as two separate `return gUnknown_0810A3E8;` statements
 * emits the block twice (with its pool word twice) rather than cross-jumping,
 * and is eight bytes longer.
 */
const u16 *GetCannonFireSpriteData(int a)
{
    switch (a)
    {
    case 3:
    default:
        return gUnknown_0810A3E8;

    case 4:
        return gUnknown_0810AFC8;
    }
}
asm(".global sub_0803F128\n.thumb_set sub_0803F128, GetCannonFireSpriteData\n");
