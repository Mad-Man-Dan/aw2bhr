#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C06C.
 * sub_0806C06C @ 0x0806C06C
 */

struct Unk0806C06C /* >= 0x38 */
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ u32 unk30;
    /* 0x34 */ u32 unk34;
};

void sub_0806C06C(struct Unk0806C06C *p)
{
    p->unk34 = 0;
    p->unk30 = 0;
}
