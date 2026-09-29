#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801566C.
 * sub_0801566C @ 0x0801566C
 */

void CopySlotSpriteAttrs(s16 a, struct UnkVec *dst)
{
    *dst = gUnknown_0200E438[gUnknown_03001470[a].unk26].unk30;
}
asm(".global sub_0801566C\n.thumb_set sub_0801566C, CopySlotSpriteAttrs\n");
