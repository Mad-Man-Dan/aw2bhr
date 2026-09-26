#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010EE8.
 * sub_08010EE8 @ 0x08010EE8
 */

void sub_08010EE8(u16 index, void *value)
{
    gUnknown_03003050[index] = value;
}
