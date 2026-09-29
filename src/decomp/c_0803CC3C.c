#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CC3C.
 * sub_0803CC3C @ 0x0803CC3C, sub_0803CC64 @ 0x0803CC64
 */

/* Written as a negated guard rather than `if (in range) return Div(...)`:
 * both spellings mean the same thing, but the positive form makes agbcc emit
 * `bls` to the Div arm and place the `return 1` FIRST, which is 40 bytes of
 * the same instructions in the wrong order. The ROM branches `bhi` past the
 * Div arm, so the Div arm is the fall-through and the guard is the negation.
 * Measured against the range-test and ternary spellings side by side.
 *
 * Div gets the UNNARROWED `a - 0xb4` (r1) while the comparison uses the u16
 * copy (r0) -- agbcc computes both and keeps them apart, which is why the
 * subtraction is not written `(u16)(a - 0xb4)` in the Div argument. */
u8 sub_0803CC3C(u16 a)
{
    if ((u16)(a - 0xb4) > 0xb)
        return 1;
    return Div(a - 0xb4, 3) + 1;
}

/* Two compares that share the `return 0` arm, which is what `||` gives. The
 * `lsls #0x18; lsrs #0x18` after the `bl` is agbcc re-narrowing a u8-returning
 * callee, not a cast here -- 0xff is GetMapArmyCount's empty-slot sentinel and
 * the same one gUnknown_020280C0.unk13 uses. */
int IsMapArmyCountValid(u16 a)
{
    u8 v = GetMapArmyCount(a);

    if (v == 0xff || v == 0)
        return 0;
    return 1;
}
asm(".global sub_0803CC64\n.thumb_set sub_0803CC64, IsMapArmyCountValid\n");
