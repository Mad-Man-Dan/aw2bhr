#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015928.
 * sub_08015928 @ 0x08015928
 */

void sub_08015928(s16 a, u32 b)
{
    gUnknown_0200E438[gUnknown_03001470[a].unk26].unk44 = b;
}
