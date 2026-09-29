#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080703B4.
 * sub_080703B4 @ 0x080703B4
 */

void DummyFunc(void)
{
}
asm(".global sub_080703B4\n.thumb_set sub_080703B4, DummyFunc\n");
