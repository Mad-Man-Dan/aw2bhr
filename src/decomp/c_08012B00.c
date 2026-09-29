#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012B00.
 * sub_08012B00 @ 0x08012B00
 */

void AddToHalfwords(u16 *dst, u16 size, u16 delta)
{
    while (size != 0)
    {
        *dst += delta;
        dst++;
        size -= 2;
    }
}
asm(".global sub_08012B00\n.thumb_set sub_08012B00, AddToHalfwords\n");
