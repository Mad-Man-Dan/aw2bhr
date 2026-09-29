#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803ABD8.
 * sub_0803ABD8 @ 0x0803ABD8
 */

void DebugScreenNoOp(void)
{
}
asm(".global sub_0803ABD8\n.thumb_set sub_0803ABD8, DebugScreenNoOp\n");
