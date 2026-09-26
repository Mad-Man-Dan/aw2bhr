#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080308B4.
 * sub_080308B4 @ 0x080308B4
 */

void sub_080308B4(u8 *src)
{
    struct Unk08090CD8 *const *pp;
    struct Unk08090CD8 *ctrl;
    struct Unk08090CD8Buf *buf;
    struct Unk08090CD8Entry *entry;
    u8 i;

    entry = &gUnknown_08090CD8->unk00->unk12c[gUnknown_08090CD8->unk00->unk1aad];
    pp = &gUnknown_08090CD8;
    entry->unk00 = 0xAF;
    entry->unk01 = gUnknown_08090CD8->unk00->unk06;
    buf = gUnknown_08090CD8->unk00;
    entry->unk02 = buf->unk20;
    entry->unk04 = 20;
    buf->unk20++;

    for (i = 0; i < 20; i++)
        entry->unk06[i] = src[i];

    ctrl = *pp;
    ctrl->unk00->unk1aad++;
    ctrl->unk00->unk1aad &= 0x1F;
}
