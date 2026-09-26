#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804450C.
 * sub_0804450C @ 0x0804450C
 */

struct Unk0804450C
{
    /* 0x00 */ u8 filler_00[0x01];
    /* 0x01 */ u8 unk01;
};

void sub_0804450C(struct Unk0804450C *p)
{
    p->unk01 &= 0xfe;
}
