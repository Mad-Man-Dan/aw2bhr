#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017B64.
 * sub_08017B64 @ 0x08017B64
 */

bool8 EventOp_StoreByte(s16 a)
{
    *(u8 *)gUnknown_0200C528[a].unk04->unk04 = gUnknown_0200C528[a].unk04->unk0c;
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08017B64\n.thumb_set sub_08017B64, EventOp_StoreByte\n");
