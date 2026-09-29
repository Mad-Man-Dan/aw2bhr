#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C154.
 * sub_0802C154 @ 0x0802C154
 */

void MarkPlayerYielded(int a1)
{
    gPlayers[a1].unk31 = 1;
}
asm(".global sub_0802C154\n.thumb_set sub_0802C154, MarkPlayerYielded\n");
