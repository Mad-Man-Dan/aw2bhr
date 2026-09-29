#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015778.
 * sub_08015778 @ 0x08015778
 */

/* One more member of the gUnknown_0200E438 accessor family; identical to
 * SetSlotSpriteHook (unk44) and SetSlotSpriteScaleX (unk3c) with the offset swapped.
 */
void SetSlotSpriteFrameTable(s16 a, u32 b)
{
    gUnknown_0200E438[gUnknown_03001470[a].unk26].unk20 = b;
}
asm(".global sub_08015778\n.thumb_set sub_08015778, SetSlotSpriteFrameTable\n");
