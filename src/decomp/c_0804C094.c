#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C094.
 * sub_0804C094 @ 0x0804C094
 */

void ResetFigurePose_None(u16 a, u16 b, int c)
{
}
asm(".global sub_0804C094\n.thumb_set sub_0804C094, ResetFigurePose_None\n");
