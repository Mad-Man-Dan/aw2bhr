#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803DE94.
 * sub_0803DE94 @ 0x0803DE94, sub_0803DEEC @ 0x0803DEEC
 */

/* Scans gUnknown_02028360 for the first record whose rectangle contains
 * (a, b), stopping at the first record whose unk02_6 kind field is zero --
 * the same 0x3C0 mask ResetInventionRecords clears and struct Unk02028360's comment
 * already records. The loop test is `ldrh` + `and 0x3C0` with no shift, which
 * is how agbcc tests a bitfield against zero; the rectangle's own fields are
 * read as bitfields and get the canonical extract pairs instead
 * (`lsl #0x1d / lsr #0x1d` for unk02_0 at bits 0..2, `lsl #0x1a / lsr #0x1d`
 * for unk02_3 at bits 3..5), both off a narrowed `ldrb` at +2.
 *
 * The two edge sums are LOCALS, not subexpressions of the `&&` chain: the ROM
 * computes both before any of the four branches, which short-circuit
 * evaluation of `a < p->unk00 + p->unk02_0` inside the chain would not do. */
struct Unk02028360 *FindInventionAt(int a, int b)
{
    struct Unk02028360 *p;

    for (p = gUnknown_02028360; p->unk02_6 != 0; p++) {
        int w = p->unk00 + p->unk02_0;
        int h = p->unk01 + p->unk02_3;

        if (a >= p->unk00 && b >= p->unk01 && a < w && b < h)
            return p;
    }
    return NULL;
}
asm(".global sub_0803DE94\n.thumb_set sub_0803DE94, FindInventionAt\n");

/* FindInventionAt's shape with one extra test. The kind field is compared
 * against the third argument out of the HALFWORD the loop test already
 * loaded -- `lsl #0x16 / lsr #0x1c` on r5, i.e. bits 6..9 of the u16 at +2,
 * which is unk02_6 -- where the rectangle's own fields are re-read as a
 * narrowed `ldrb`. CSE keeping the loop test's `ldrh` alive across the body
 * is what puts the 0x3C0 mask in r8 rather than r7 and costs this function
 * the extra high-register save its twin does not have. */
struct Unk02028360 *FindInventionOfKindAt(int a, int b, int c)
{
    struct Unk02028360 *p;

    for (p = gUnknown_02028360; p->unk02_6 != 0; p++) {
        int w = p->unk00 + p->unk02_0;
        int h = p->unk01 + p->unk02_3;

        if (a >= p->unk00 && b >= p->unk01 && a < w && b < h
         && p->unk02_6 == c)
            return p;
    }
    return NULL;
}
asm(".global sub_0803DEEC\n.thumb_set sub_0803DEEC, FindInventionOfKindAt\n");
