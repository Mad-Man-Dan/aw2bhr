#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080311D0.
 * sub_080311D0 @ 0x080311D0
 */

struct Unk080311D0 /* >= 0x5c */
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ u32 unk58;
};

void sub_080311D0(struct Unk080311D0 *p)
{
    p->unk58 = 0;
}
