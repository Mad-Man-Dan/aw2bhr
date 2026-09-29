#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034400.
 * sub_08034400 @ 0x08034400
 */

void PackPathNibbles(u8 *src, u8 *dst)
{
    int i;

    for (i = 0; i < 6; i++)
        dst[i] = ((src[i * 2] & 0xf) << 4) | (src[i * 2 + 1] & 0xf);
}
asm(".global sub_08034400\n.thumb_set sub_08034400, PackPathNibbles\n");
