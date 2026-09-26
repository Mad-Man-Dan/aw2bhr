#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801D98C.
 * sub_0801D98C @ 0x0801D98C
 */

void sub_0801D98C(int index, s16 x, s16 y)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[index];

    p->unk0c = x << 8;
    p->unk10 = y << 8;
}
