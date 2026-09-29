#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072C28.
 * sub_08072C28 @ 0x08072C28
 */

void FillHalfwordsUnsigned(u16 *dst, u32 count, u16 value)
{
    u32 i;

    for (i = 0; i < count; i++)
        *dst++ = value;
}
asm(".global sub_08072C28\n.thumb_set sub_08072C28, FillHalfwordsUnsigned\n");
