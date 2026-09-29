#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010F94.
 * sub_08010F94 @ 0x08010F94
 */

void InitScreenFadeLatch(void)
{
    gUnknown_03002B5C = 1;
}
asm(".global sub_08010F94\n.thumb_set sub_08010F94, InitScreenFadeLatch\n");
