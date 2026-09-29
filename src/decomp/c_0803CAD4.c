#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CAD4.
 * sub_0803CAD4 @ 0x0803CAD4
 */

u8 IsCoSelectable(u32 id)
{
    struct Unk02028030 *s = &gUnknown_02028030;
    u32 idx = id >> 3;
    u8 *b = s->unk2d;
    u8 *p = b + idx;

    return (1 << (id & 7)) & *p;
}
asm(".global sub_0803CAD4\n.thumb_set sub_0803CAD4, IsCoSelectable\n");
