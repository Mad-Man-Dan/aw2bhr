#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080362D0.
 * sub_080362D0 @ 0x080362D0
 */

struct Unk080362D0
{
    /* 0x00 */ u8 filler_00[0x3e];
    /* 0x3e */ u16 unk3e;
};

void sub_080362D0(struct Unk080362D0 *p)
{
    p->unk3e--;
}
