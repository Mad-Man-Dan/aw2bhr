#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013564.
 * sub_08013564 @ 0x08013564
 */

struct Unk08013564
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
};

void sub_08013564(struct Unk08013564 *s)
{
    int mask = ~0xE;

    s->unk04 &= mask;
    s->unk02 &= mask;
    s->unk00 &= mask;
}
