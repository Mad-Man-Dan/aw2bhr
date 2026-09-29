#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CB74.
 * sub_0803CB74 @ 0x0803CB74, sub_0803CB8C @ 0x0803CB8C
 */

int IsCampaignFlagBank0Set(int id)
{
    u8 *b = gUnknown_030033F4;
    int idx = id >> 3;
    u8 *p = b + idx;

    return (1 << id) & *p;
}
asm(".global sub_0803CB74\n.thumb_set sub_0803CB74, IsCampaignFlagBank0Set\n");

void ClearCampaignFlagBank0(void)
{
    u8 *base;
    u8 *p;
    u8 v;

    base = gUnknown_030033F4;
    v = 0;
    p = base + 3;

    do
        *p-- = v;
    while ((int)p >= (int)base);
}
asm(".global sub_0803CB8C\n.thumb_set sub_0803CB8C, ClearCampaignFlagBank0\n");
