#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015844.
 * sub_08015844 @ 0x08015844
 */

void SetSlotSpriteScale(s16 a, u16 b, u16 c)
{
    gUnknown_0200E438[gUnknown_03001470[a].unk26].unk3c = b;
    gUnknown_0200E438[gUnknown_03001470[a].unk26].unk3e = c;
}
asm(".global sub_08015844\n.thumb_set sub_08015844, SetSlotSpriteScale\n");
