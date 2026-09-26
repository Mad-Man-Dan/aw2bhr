#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063A30.
 * sub_08063A30 @ 0x08063A30
 */

void sub_08063A30(struct Unk03001470 *entry, const void *data)
{
    entry->unk04 = data;
    entry->unk08 = 0;
    entry->unk10 = 0;
}
