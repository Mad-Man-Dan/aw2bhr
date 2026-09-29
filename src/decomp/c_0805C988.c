#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805C988.
 * sub_0805C988 @ 0x0805C988, sub_0805C9CC @ 0x0805C9CC
 */

/* One of a near-twin pair with sub_0805C9CC: both call GetInventionRecordByIndex(0) for the
 * head of the 8-byte record list at 0x02028360 and walk it until unk02_6 is 0.
 *
 * The 0x3C0 MASK-AND-COMPARE is the bitfield read, not a plain u16 access:
 * comparing struct Unk02028360's `u16 unk02_6 : 4` against a CONSTANT folds to
 * `(container & 0x3C0) == k << 6` in the container's HImode, which is why the
 * mask is hoisted into r3 and compared against 0x40 rather than extracted with
 * a shift pair. sub_0805C9CC's `p->unk02_0` is compared against a VARIABLE, so
 * it cannot fold and comes out as extract_bit_field's `ldrb; lsl #29; lsr #29`
 * instead -- the two spellings in one pair of functions confirm the boundary
 * the header records for this member.
 *
 * The loop test sits at the BOTTOM, entered by `b`, where sub_0805C9CC's is
 * peeled into a guard plus a do/while. That falls out of the compiler, not the
 * source: both are the same `for`, and 0x140 needs a register that 0x40 (an
 * 8-bit `cmp` immediate) does not, so only the twin gets a preheader and with
 * it the rotation. */

int AiIsOnLaserLine(int x, int y)
{
    struct Unk02028360 *p;

    for (p = GetInventionRecordByIndex(0); p->unk02_6 != 0; p++)
    {
        if (p->unk02_6 == 1)
        {
            if (x == p->unk00 || y == p->unk01)
                return 1;
        }
    }

    return 0;
}
asm(".global sub_0805C988\n.thumb_set sub_0805C988, AiIsOnLaserLine\n");

/* AiIsOnLaserLine's near-twin -- same list walk, different selector and hit test.
 * See that function for why the 0x3C0 compare is a bitfield read and why this
 * one alone gets the guard-plus-do/while shape.
 *
 * `p->unk02_0` is compared against a variable rather than a constant, so it
 * cannot fold into a mask-and-compare and comes out as extract_bit_field's
 * `ldrb r0,[r2,#2]; lsl #29; lsr #29` -- a QImode extract, because bits 0..2
 * sit wholly inside the first byte of the u16 container. Both compares are
 * SIGNED (`bgt`, `blt`): `p->unk00 - x` is int arithmetic and the 3-bit field
 * promotes to int. */

int sub_0805C9CC(int x, int y)
{
    struct Unk02028360 *p;

    for (p = GetInventionRecordByIndex(0); p->unk02_6 != 0; p++)
    {
        if (p->unk02_6 == 5)
        {
            if (p->unk00 - x <= p->unk02_0 && y >= p->unk01 + 3)
                return 1;
        }
    }

    return 0;
}
