#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080452E4.
 * sub_080452E4 @ 0x080452E4
 */

struct Unk452E4
{
    /* 0x00 */ u8 filler_00[0x3c];
    /* 0x3c */ s16 unk3c;
    /* 0x3e */ u8 filler_3e[0x02];
    /* 0x40 */ int unk40;
};

void sub_080452E4(struct Unk452E4 *p)
{
    sub_08029088(p->unk3c, p->unk40);
}
