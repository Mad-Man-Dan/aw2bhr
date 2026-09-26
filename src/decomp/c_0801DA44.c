#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DA44.
 * sub_0801DA44 @ 0x0801DA44
 */

u32 sub_0801DA44(int index)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[index];

    return p->unk2c;
}
