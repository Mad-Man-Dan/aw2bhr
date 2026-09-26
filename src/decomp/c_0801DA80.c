#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DA80.
 * sub_0801DA80 @ 0x0801DA80
 */

u8 sub_0801DA80(int index)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[index];

    return p->unk28;
}
