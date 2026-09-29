#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017B60.
 * sub_08017B60 @ 0x08017B60
 */

s16 EventOp_Halt(s16 a)
{
    return 0;
}
asm(".global sub_08017B60\n.thumb_set sub_08017B60, EventOp_Halt\n");
