#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030838.
 * sub_08030838 @ 0x08030838
 */

void SioQueuePendingRecvData(struct Unk08090CD8Entry *src)
{
    struct Unk08090CD8Entry *dst;
    int i;

    dst = &gUnknown_0849B018->unk12c[32 + gUnknown_0849B018->unk1aaf];
    dst->unk00 = src->unk00;
    dst->unk01 = src->unk01;
    dst->unk02 = src->unk02;
    dst->unk04 = src->unk04;
    for (i = 0; i < src->unk04; i++)
        dst->unk06[i] = src->unk06[i];
    gUnknown_0849B018->unk1aaf++;
    gUnknown_0849B018->unk1aaf &= 0xF;
}
asm(".global sub_08030838\n.thumb_set sub_08030838, SioQueuePendingRecvData\n");
