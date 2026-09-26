#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015820.
 * sub_08015820 @ 0x08015820
 */

u16 sub_08015820(s16 a)
{
    return gUnknown_0200E438[gUnknown_03001470[a].unk26].unk3e;
}
