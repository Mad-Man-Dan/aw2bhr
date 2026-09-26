#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044518.
 * CopUnitResupply @ 0x08044518
 */

void CopUnitResupply(void *p)
{
    sub_08029978(p, 0);
    sub_08029A48(p, 0);
}

asm(".global sub_08044518\n.thumb_set sub_08044518, CopUnitResupply\n");
