#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E3B4.
 * sub_0801E3B4 @ 0x0801E3B4
 */

/* Maps the top two bits of a flag word onto four sprite-priority values. A
 * LEAF -- no prologue at all, just `bx lr`.
 *
 * The shape is agbcc's balanced switch tree, not a chain of `if`s: it tests the
 * MIDDLE case (0x8000) first, then `bgt` to split the remaining two, which is
 * what `expand_end_case` emits once there are three or more cases over a dense
 * range. Written as if/else-if the compares come out in source order instead.
 * `bgt` rather than `bhi` makes the masked value signed, i.e. the parameter is
 * `int` -- and `adds r1, r0, #0` before the mask is the copy-then-use that says
 * the parameter is not narrower. */
int GetObjDimensionFromBits(int a)
{
    switch (a & 0xC000)
    {
    case 0x4000:
        return 0x10;
    case 0x8000:
        return 0x20;
    case 0xC000:
        return 0x40;
    default:
        return 8;
    }
}
asm(".global sub_0801E3B4\n.thumb_set sub_0801E3B4, GetObjDimensionFromBits\n");
