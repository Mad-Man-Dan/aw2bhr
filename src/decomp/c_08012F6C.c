#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012F6C.
 * sub_08012F6C @ 0x08012F6C
 */

/* CpuCopyAuto's SIGNED sibling -- same CpuSet-or-CpuFastSet choice on a
 * multiple-of-32 byte count, but the count is a signed int and the result is
 * masked to CpuSet's 21-bit length field.
 *
 * Both halves of that are readable off the shifts. The bias sequences
 * `lsrs #0x1f; adds` and `cmp #0; bge; adds #3` are signed division by 2 and
 * by 4, which an unsigned count would not produce. The odd-looking
 * `lsls #0xa; lsrs #0xb` and `lsls #0x9; lsrs #0xb` are the `& 0x1FFFFF`
 * fused with the division's arithmetic shift -- count them as
 * `(u32)x << a >> b` and both come out as a 21-bit result, which is exactly
 * the width of the BIOS length field. */
void VramCopy(const void *src, void *dst, int size)
{
    if (size & 0x1F)
        CpuSet(src, dst, (size / 2) & 0x1FFFFF);
    else
        CpuFastSet(src, dst, (size / 4) & 0x1FFFFF);
}
asm(".global sub_08012F6C\n.thumb_set sub_08012F6C, VramCopy\n");
