#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057EC0.
 * sub_08057EC0 @ 0x08057EC0
 */

/* The 4-byte record: (column, row) plus an s16 priority. */
struct Unk08057EC0Rec
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s16 unk02;
};

void *AiPopFirstNearestCandidate(void)
{
    struct Unk08057EC0Rec *e;
    struct Unk08057EC0Rec *best;
    s16 bestv;

    e = (struct Unk08057EC0Rec *)gUnknown_03003F20;
    best = 0;
    bestv = 0x7FFF;

    while (e->unk02 >= 0)
    {
        if (e->unk02 < bestv)
        {
            best = e;
            bestv = e->unk02;
        }
        e++;
    }

    if (best != 0)
        best->unk02 = 0x7FFF;

    return best;
}
asm(".global sub_08057EC0\n.thumb_set sub_08057EC0, AiPopFirstNearestCandidate\n");
