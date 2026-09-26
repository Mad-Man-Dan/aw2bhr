#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080156FC.
 * sub_080156FC @ 0x080156FC
 */

void sub_080156FC(s16 a, u16 b)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[gUnknown_03001470[a].unk26];
    u32 v;

    p->unk1c = 0;
    v = p->unk48[b + 1];
    p->unk04 = v;
    p->unk08 = v;
}
