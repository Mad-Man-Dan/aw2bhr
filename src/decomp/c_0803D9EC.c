#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D9EC.
 * sub_0803D9EC @ 0x0803D9EC
 */

struct Unk803D9EC
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u16 unk1e;
    /* 0x20 */ u16 unk20;
};

void sub_0803D9EC(struct Unk803D9EC *p)
{
    p->unk1e = gUnknown_030033EC;
    p->unk20 = 1;
}
