#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803EB7C.
 * sub_0803EB7C @ 0x0803EB7C
 */

struct Unk0803EB7C /* >= 0x4e */
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk4c;
};

void sub_0803EB7C(struct Unk0803EB7C *p)
{
    p->unk4c = 0;
}
