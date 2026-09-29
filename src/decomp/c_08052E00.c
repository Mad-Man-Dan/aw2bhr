#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052E00.
 * sub_08052E00 @ 0x08052E00
 */

void BombEffect_Loop(void)
{
}
asm(".global sub_08052E00\n.thumb_set sub_08052E00, BombEffect_Loop\n");
