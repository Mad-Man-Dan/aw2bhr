#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013428.
 * sub_08013428 @ 0x08013428
 */

void DebugPrintf(int x, int y, const char *fmt, ...)
{
}
asm(".global sub_08013428\n.thumb_set sub_08013428, DebugPrintf\n");
