#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B644.
 * GetBackdropVariantForSecondSide @ 0x0804B644
 */

/* A terrain/sprite id remap, applied only when the two ids agree. Self
 * contained: no globals, no calls.
 *
 * The case ORDER is the source's, not the table's -- agbcc lays case bodies out
 * in source order and the ROM has 39/40 before 37/38, and 44 between 3 and 4.
 * The jump table itself runs 3..45 with 28 default entries in the middle, which
 * is what fixes the `subs r0, #3; cmp r0, #0x2a` bound.
 *
 * `a == b` on two u16 parameters compiles to a compare of the two `<< 16`
 * forms with `b` recovered by `lsrs #0x10` -- the shared-shift pattern, not a
 * sign of anything in the source. */
u16 GetBackdropVariantForSecondSide(u16 a, u16 b)
{
    if (a == b)
    {
        switch (b)
        {
        case 33: b = 0x22; break;
        case 34: b = 0x21; break;
        case 35: b = 0x24; break;
        case 36: b = 0x23; break;
        case 39: b = 0x28; break;
        case 40: b = 0x27; break;
        case 37: b = 0x26; break;
        case 38: b = 0x25; break;
        case 3:  b = 0x2C; break;
        case 44: b = 3;    break;
        case 4:  b = 0x2D; break;
        case 45: b = 4;    break;
        }
    }

    return b;
}

asm(".global sub_0804B644\n.thumb_set sub_0804B644, GetBackdropVariantForSecondSide\n");
