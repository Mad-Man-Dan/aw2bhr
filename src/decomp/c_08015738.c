#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015738.
 * sub_08015738 @ 0x08015738
 */

void SetSlotSpriteScriptTable(s16 a, u32 *b, u16 c)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[gUnknown_03001470[a].unk26];
    u32 v;

    p->unk1c = 0;
    v = b[c + 1];
    p->unk04 = v;
    p->unk08 = v;
    p->unk48 = b;
    p->unk20 = b[0];
}
asm(".global sub_08015738\n.thumb_set sub_08015738, SetSlotSpriteScriptTable\n");
