#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C950.
 * sub_0803C950 @ 0x0803C950, sub_0803C97C @ 0x0803C97C, sub_0803C9A8 @ 0x0803C9A8, sub_0803C9D4 @ 0x0803C9D4
 */

void SetMapCategoryUnlocked(u32 id, u8 value)
{
    struct Unk02028030 *s = &gUnknown_02028030;
    u32 idx = id >> 3;
    u8 *b = s->unk10;
    u8 *p = b + idx;
    u32 bit = id & 7;

    *p = (*p & ~(1 << bit)) | (value << bit);
}
asm(".global sub_0803C950\n.thumb_set sub_0803C950, SetMapCategoryUnlocked\n");

void SetCoUnlocked(u32 id, u8 value)
{
    struct Unk02028030 *s = &gUnknown_02028030;
    u32 idx = id >> 3;
    u8 *b = s->unk2a;
    u8 *p = b + idx;
    u32 bit = id & 7;

    *p = (*p & ~(1 << bit)) | (value << bit);
}
asm(".global sub_0803C97C\n.thumb_set sub_0803C97C, SetCoUnlocked\n");

void SetCoSelectable(u32 id, u8 value)
{
    struct Unk02028030 *s = &gUnknown_02028030;
    u32 idx = id >> 3;
    u8 *b = s->unk2d;
    u8 *p = b + idx;
    u32 bit = id & 7;

    *p = (*p & ~(1 << bit)) | (value << bit);
}
asm(".global sub_0803C9A8\n.thumb_set sub_0803C9A8, SetCoSelectable\n");

void SetCampaignFlagBank2(u32 id, u8 value)
{
    struct Unk02028030 *s = &gUnknown_02028030;
    u32 idx = id >> 3;
    u8 *b = s->unk08;
    u8 *p = b + idx;
    u32 bit = id & 7;

    *p = (*p & ~(1 << bit)) | (value << bit);
}
asm(".global sub_0803C9D4\n.thumb_set sub_0803C9D4, SetCampaignFlagBank2\n");
