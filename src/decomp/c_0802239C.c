#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802239C.
 * sub_0802239C @ 0x0802239C
 */

/* A seven-argument wrapper over the eight-argument WriteUnitTileQuad: it folds the
 * (x, y) cell into the tilemap pointer and passes four zeroes for the trailing
 * options.
 *
 * The two coordinate scalings are the shorten_binary_op form -- `lsls #0x10`
 * then `lsrs #0xf` is `(u16)x * 2`, not a mask and a shift -- and they fuse the
 * u16 parameters' entry narrowing with the pointer scale, which is why the
 * halves of each pair sit apart in the listing.
 *
 * Every parameter width below is read off WriteUnitTileQuad's own prologue, not
 * guessed here: r1/r3 and the first and last stack arguments narrow to u8, r2
 * and the middle two stack arguments to u16, and r0 is untouched. */
void WriteUnitTileQuadAt(u16 *dst, u16 x, u16 y, u8 a4, u16 a5, u8 a6, u8 a7)
{
    WriteUnitTileQuad(dst + x + y * 32, a4, a5, a6, a7, 0, 0, 0);
}
asm(".global sub_0802239C\n.thumb_set sub_0802239C, WriteUnitTileQuadAt\n");
