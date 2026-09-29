#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CC84.
 * CopyString @ 0x0803CC84
 */

/* Not a member of the 0x0803Cxxx bit-accessor family despite the address --
 * it touches no global at all and takes both buffers as parameters.
 *
 * A string copy that writes one byte past the cursor on every iteration: the
 * `dst[i + 1]` store is redundant for all but the last pass, and the loop only
 * advances i by one. The `u8` counter is proved by the `lsl #24; lsr #24` on
 * the increment. dst is r0 and src is r1 -- LoadDesignRoomName passes its own second
 * parameter as dst and &gUnknown_020280C0[i].unk02 as src. */
void CopyString(u8 *dst, const u8 *src)
{
    u8 i = 0;

    while (src[i] != 0)
    {
        dst[i] = src[i];
        dst[i + 1] = src[i + 1];
        i++;
    }

    dst[i] = 0;
}

asm(".global sub_0803CC84\n.thumb_set sub_0803CC84, CopyString\n");
