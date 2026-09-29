#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C91C.
 * sub_0803C91C @ 0x0803C91C
 */

/* The setter half of the IsCampaignMapUnlockedByMapData pair, and the same body as the promoted
 * SetCampaignMapUnlocked with the bit id mapped through FindMapIdByMapData first.
 *
 * The `s = &gUnknown_02028030` assignment must come AFTER the call, and this is
 * measured: written before it, agbcc keeps the address live across the `bl` in
 * a callee-saved register and pushes r5 as well. It also has to go through the
 * struct pointer rather than `&gUnknown_02028030.unk12[i]` -- the latter folds
 * the +0x12 into the relocation addend and loses the `adds r1, #0x12`. */
void SetCampaignMapUnlockedByMapData(u32 id, u8 value)
{
    struct Unk02028030 *s;
    u32 k;
    u32 idx;
    u8 *b;
    u8 *p;
    u32 bit;

    k = FindMapIdByMapData(id);
    s = &gUnknown_02028030;
    idx = k >> 3;
    b = s->unk12;
    p = b + idx;
    bit = k & 7;

    *p = (*p & ~(1 << bit)) | (value << bit);
}
asm(".global sub_0803C91C\n.thumb_set sub_0803C91C, SetCampaignMapUnlockedByMapData\n");
