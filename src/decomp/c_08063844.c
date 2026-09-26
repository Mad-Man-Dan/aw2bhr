#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063844.
 * sub_08063844 @ 0x08063844
 */

u32 sub_08063844(struct Unk03001470 *entry)
{
    return entry->unk00 >> 8;
}
