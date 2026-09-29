#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074230.
 * sub_08074230 @ 0x08074230
 */

void EmptyStubMenuA(void)
{
}
asm(".global sub_08074230\n.thumb_set sub_08074230, EmptyStubMenuA\n");
