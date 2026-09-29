#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B6BC.
 * sub_0801B6BC @ 0x0801B6BC
 */

/* Copies an IWRAM CODE OVERLAY into place: 0x086173F0 in ROM is CpuSet over
 * 0x03005C88 .. 0x0300677C, and three wrappers a few dozen bytes below then
 * call into the copied region through odd (THUMB) addresses inside it. See the
 * gUnknown_03005C88 note in include/unknown-globals.h.
 *
 * The length is a BYTE difference divided by four, and the ROM says so: it
 * carries the `cmp #0; bge; adds #3` rounding correction of a signed divide.
 * Spelled with `u32 *` boundaries the compiler knows the difference is already
 * a word count and drops the correction, which is 4 bytes short.
 *
 * `lsls #9; lsrs #0xb` is that divide FUSED with a mask, counted the way
 * docs/agbcc-codegen.md says to count a shift pair -- `(u32)x << 9 >> 11` keeps
 * bits 2..22, i.e. `(x / 4) & 0x1FFFFF`, and 21 bits is exactly the width of
 * the BIOS length field. The same fusion is already recorded on VramCopy.
 * `movs #0x80; lsls #0x13` is 0x04000000, the 32-bit-transfer bit. */
void StoreRoutinesToIRAM(void)
{
    CpuSet(gUnknown_086173F0, gUnknown_03005C88,
           (((gUnknown_0300677C - gUnknown_03005C88) / 4) & 0x1FFFFF) | 0x04000000);
}
asm(".global sub_0801B6BC\n.thumb_set sub_0801B6BC, StoreRoutinesToIRAM\n");
