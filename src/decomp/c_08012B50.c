#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012B50.
 * sub_08012B50 @ 0x08012B50
 */

/* The halfword-fill twin of AddToHalfwords: same `size -= 2` byte count, same
 * duplicated entry guard, only the body differs. */
void FillHalfwords2(u16 *dst, u16 size, u16 value)
{
    while (size != 0)
    {
        *dst = value;
        dst++;
        size -= 2;
    }
}
asm(".global sub_08012B50\n.thumb_set sub_08012B50, FillHalfwords2\n");
