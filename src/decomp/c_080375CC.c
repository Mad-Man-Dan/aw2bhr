#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080375CC.
 * sub_080375CC @ 0x080375CC
 */

struct Unk080375CC
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u16 unk1e;
};

void sub_080375CC(struct Unk080375CC *p)
{
    p->unk1e = 0;
}
