#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804129C.
 * sub_0804129C @ 0x0804129C
 */

struct Unk804129C
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk4c;
};

void sub_0804129C(struct Unk804129C *s)
{
    s->unk4c = 0;
}
