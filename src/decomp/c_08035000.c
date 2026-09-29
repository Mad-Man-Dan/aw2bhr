#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035000.
 * sub_08035000 @ 0x08035000
 */

const struct Unk085C77A0 *GetMapListEntry(int index)
{
    return &gUnknown_085C77A0[index];
}
asm(".global sub_08035000\n.thumb_set sub_08035000, GetMapListEntry\n");
