#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080718E4.
 * sub_080718E4 @ 0x080718E4
 */

void DummyFunc_rev(void)
{
}
asm(".global sub_080718E4\n.thumb_set sub_080718E4, DummyFunc_rev\n");
