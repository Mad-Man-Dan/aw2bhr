#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063454.
 * sub_08063454 @ 0x08063454
 */

/* Arms a transfer on the link record, or resets it if the record is busy or
 * the request is out of range.
 *
 * Two things here are not what the listing looks like.
 *
 * The switch is `switch (a5)` over cases -4..4, NOT `switch ((s8)a5 + 4)`.
 * expand_case builds the tree `index - minval` in the INDEX's own type, so the
 * `+ 4` happens in QImode and comes out as `(a5 << 24) + 0x04000000` shifted
 * back down -- which is why `a5 << 24` and not `(s8)a5` is what stays live in
 * r2, and why each arm re-`asrs` it.
 *
 * The failure block sits BETWEEN the range check and the success block, which
 * an `if (...) { ...; return; }` tail cannot produce -- gcc places the outer
 * if's false-label after the body, so the success block would come first. The
 * forward `goto` is what puts the false-label in the middle.
 *
 * `~0x7E` really is ~0x7E (bits 0 and 7 of the packed byte): `movs #0x7f;
 * rsbs #0` materialises -0x7F. Spelling it 0x81 would have been one `movs`. */
void MultiBootStartMaster(struct Unk08062FB8 *p, int a2, int a3, u8 a4, s8 a5)
{
    int q;

    if (p->unk18 == 0 && p->unk1e != 0 && p->unk4a == 0)
    {
        p->unk20 = a2;
        a3 = (a3 + 15) & ~15;
        if (a3 >= 0x100 && a3 <= 0x40000)
            goto ok;
    }

    sub_08062FB8(p);
    return;

ok:
    p->unk24 = a2 + a3;
    switch (a5)
    {
    case -4:
    case -3:
    case -2:
    case -1:
        q = (a4 << 3) | (3 - a5);
        break;
    case 0:
        q = a4 | 0x38;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        q = (a4 << 3) | (a5 - 1);
        break;
    }
    p->unk1c = ((q & 0x3F) << 1) | ~0x7E;
    p->unk18 = 0xD0;
}
asm(".global sub_08063454\n.thumb_set sub_08063454, MultiBootStartMaster\n");
