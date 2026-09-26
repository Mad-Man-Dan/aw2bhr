#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CA00.
 * sub_0803CA00 @ 0x0803CA00
 */

void sub_0803CA00(u32 id, u8 value)
{
    struct Unk02028030 *s = &gUnknown_02028030;
    u32 idx = id >> 3;
    u8 *b = s->unk00;
    u8 *p = b + idx;
    u32 bit = id & 7;

    *p = (*p & ~(1 << bit)) | (value << bit);
}
