#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080570C4.
 * sub_080570C4 @ 0x080570C4
 */

struct Unk080570C4Rec
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
};

void DrawTileBlockA(void *a1)
{
    struct Unk080570C4Rec *p = a1;
    u16 chr;
    int x;
    int y;

    chr = p->unk00;

    for (y = 0; y < p->unk04; y++)
    {
        for (x = 0; x < p->unk02; x++)
        {
            PutTileEntryA(chr, p->unk06 + x + (y << 5), p->unk08, 0);
            chr++;
        }
    }
}
asm(".global sub_080570C4\n.thumb_set sub_080570C4, DrawTileBlockA\n");
