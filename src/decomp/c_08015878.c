#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015878.
 * sub_08015878 @ 0x08015878
 */


/*
 * sub_08015878 -- set a sprite's tile number and palette.
 *
 * `a` is a gUnknown_03001470 slot. sub_0801566C copies the slot's stored OAM
 * attributes into a local, the two bitfields are poked, and sub_08015608 hands
 * the eight bytes back by value. SetSlotSpriteHidden and SetSlotSpriteFlicker are the same
 * shape, each setting a single bit.
 *
 * Why the C looks odd: all three parameters are declared narrow, and `b` is
 * signed where `c` is unsigned. Both arrive zero-extended, so the difference
 * shows up only at the bitfield store: storing a signed value into an unsigned
 * bitfield makes the compiler copy the mask into a register of its own first.
 * Widening the parameters, or making `b` unsigned, loses that copy.
 */
void sub_08015878(s16 a, s16 b, u16 c)
{
    struct OamData o;

    sub_0801566C(a, (struct UnkVec *)&o);
    o.tileNum = b;
    o.paletteNum = c;
    sub_08015608(a, *(struct UnkVec *)&o);
}
