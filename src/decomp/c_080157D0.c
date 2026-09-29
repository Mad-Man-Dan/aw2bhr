#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080157D0.
 * sub_080157D0 @ 0x080157D0
 */

u16 GetSlotSpriteScaleX(s16 a)
{
    return gUnknown_0200E438[gUnknown_03001470[a].unk26].unk3c;
}
asm(".global sub_080157D0\n.thumb_set sub_080157D0, GetSlotSpriteScaleX\n");
