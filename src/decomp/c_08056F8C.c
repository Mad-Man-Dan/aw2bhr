#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08056F8C.
 * sub_08056F8C @ 0x08056F8C
 */

struct Unk08056F8CRec
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
};

void DrawTileBlockFlippableB(void *a1)
{
    struct Unk08056F8CRec *p = a1;
    u16 chr;
    int x;
    int y;

    chr = p->unk00;

    if (p->unk0a != 0)
    {
        for (y = 0; y < p->unk04; y++)
        {
            for (x = p->unk02; x > 0; x--)
            {
                PutTileEntryB(chr, p->unk06 + x + (y << 5), p->unk08, 1);
                chr++;
            }
        }
    }
    else
    {
        for (y = 0; y < p->unk04; y++)
        {
            for (x = 0; x < p->unk02; x++)
            {
                PutTileEntryB(chr, p->unk06 + x + (y << 5), p->unk08, 0);
                chr++;
            }
        }
    }
}
asm(".global sub_08056F8C\n.thumb_set sub_08056F8C, DrawTileBlockFlippableB\n");
