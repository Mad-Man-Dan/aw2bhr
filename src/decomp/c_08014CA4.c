#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014CA4.
 * sub_08014CA4 @ 0x08014CA4, sub_08014CEC @ 0x08014CEC
 */

/* Advances the text cursor by `a2` sub-tile units. unk40 is a fractional
 * accumulator in eighths: every whole 8 it carries out steps unk34 (the tile
 * x in halfwords) by 2 and unk32 (the tile column) by 1. Returns 1 when a
 * fraction is left over.
 *
 * `r = 0` sits BETWEEN the `unk40 += a2` store and the loop guard in the ROM,
 * so it is a statement there and not a `return unk40 != 0;` -- a returned
 * comparison would go through do_store_flag and never place the constant
 * before the loop. */
int TextWriterAdvanceCursor(struct Unk08014074 *p, int a2)
{
    int r;

    p->unk40 += a2;
    r = 0;

    while (p->unk40 > 7)
    {
        p->unk40 -= 8;
        p->unk34 += 2;
        p->unk32++;
    }

    if (p->unk40 != 0)
        r = 1;

    return r;
}
asm(".global sub_08014CA4\n.thumb_set sub_08014CA4, TextWriterAdvanceCursor\n");

/* Measures a string in pixels with one pixel of inter-character spacing:
 * gUnknown_084C36E4[c] per character, plus 1 before every character after the
 * first. `w != 0` rather than a separate "seen" flag is what the ROM tests --
 * the same accumulator drives the spacing decision.
 *
 * The accumulator is u16: both `+= ` steps round-trip through
 * `lsls #0x10; lsrs #0x10`. It is returned as `int` without re-narrowing
 * because it is already zero-extended. */
int GetStringPixelWidthSimple(u8 *s)
{
    u16 w;

    w = 0;

    while (*s != 0)
    {
        if (w != 0)
            w++;

        w += gUnknown_084C36E4[*s];
        s++;
    }

    return w;
}
asm(".global sub_08014CEC\n.thumb_set sub_08014CEC, GetStringPixelWidthSimple\n");
