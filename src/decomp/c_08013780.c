#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013780.
 * sub_08013780 @ 0x08013780
 */

/* Fills a run of the 32-byte palette-bias array.  The `lsl #24; lsr #24` on
 * the value is the fill-loop rule, not a cast: the store sits in the loop so
 * combine cannot fold the truncation into it, which is what makes the third
 * parameter recoverably `u8`.  Both of the first two are `u16` for the same
 * reason (`lsl #16; lsr #16`), and the loop counter is `int` because
 * `start + count` promotes and the compares are signed. */
void ColFadeSet(u16 start, u16 count, u8 value)
{
    int i;

    for (i = start; i < start + count; i++)
        gUnknown_0200B5F4[i] = value;
}
asm(".global sub_08013780\n.thumb_set sub_08013780, ColFadeSet\n");
