#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044530.
 * sub_08044530 @ 0x08044530
 */

void CopUnitEffectNone(void)
{
}
asm(".global sub_08044530\n.thumb_set sub_08044530, CopUnitEffectNone\n");
