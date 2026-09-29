#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CA9C.
 * sub_0803CA9C @ 0x0803CA9C, sub_0803CAB8 @ 0x0803CAB8
 */

u8 IsMapCategoryUnlocked(u32 id)
{
    struct Unk02028030 *s = &gUnknown_02028030;
    u32 idx = id >> 3;
    u8 *b = s->unk10;
    u8 *p = b + idx;

    return (1 << (id & 7)) & *p;
}
asm(".global sub_0803CA9C\n.thumb_set sub_0803CA9C, IsMapCategoryUnlocked\n");

int IsCoUnlocked(u32 id)
{
    struct Unk02028030 *s = &gUnknown_02028030;
    u32 idx = id >> 3;
    u8 *b = s->unk2a;
    u8 *p = b + idx;

    return (1 << (id & 7)) & *p;
}
asm(".global sub_0803CAB8\n.thumb_set sub_0803CAB8, IsCoUnlocked\n");
