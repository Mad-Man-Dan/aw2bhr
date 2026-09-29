#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044404.
 * sub_08044404 @ 0x08044404, CopUnitCondAlways @ 0x08044408
 */

bool8 CopUnitCondAlways2(void *unit)
{
    return 1;
}
asm(".global sub_08044404\n.thumb_set sub_08044404, CopUnitCondAlways2\n");

bool8 CopUnitCondAlways(void *unit)
{
    return 1;
}

asm(".global sub_08044408\n.thumb_set sub_08044408, CopUnitCondAlways\n");
