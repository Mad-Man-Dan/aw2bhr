#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A178.
 * sub_0801A178 @ 0x0801A178, sub_0801A1A0 @ 0x0801A1A0
 */

/* Scrolls a list window one row towards the top: counts unk00 down and pulls
 * the view origin unk03 with it once the cursor would leave the unk04-row
 * window. Returns 0 when already at the top and 1 otherwise.
 *
 * A LEAF -- no `push` at all and a bare `bx lr` -- so nothing here may need a
 * callee-saved register. ScrollList_CursorDown below is the mirror image and does need
 * one, for the (unk01 - unk02) it computes up front.
 *
 * `p->unk00 - p->unk03 < p->unk04` compares SIGNED (`bge` on the skip): the
 * subtraction of two u8 members promotes to int and can go negative. The
 * `p->unk03 != 0` guard is second, which is the order the two `cmp`s appear
 * in. */
struct Unk1A178
{
    /* 00 */ u8 unk00;
    /* 01 */ u8 unk01;
    /* 02 */ u8 unk02;
    /* 03 */ u8 unk03;
    /* 04 */ u8 unk04;
    /* 05 */ u8 unk05;
};
/* The downward twin of ScrollList_CursorUp: counts unk00 up towards the unk01 limit
 * and pushes the view origin unk03 after it, bounded by the unk05-row window
 * and by the (unk01 - unk02) computed on entry.
 *
 * That difference is a u8 LOCAL, not an int: `lsls #0x18; lsrs #0x18` truncates
 * it, and it is computed before the early-out test, which is where a
 * block-top declaration with an initialiser puts it.
 *
 * The two direct u8-vs-u8 comparisons come out UNSIGNED (`bhi`) while the one
 * on the subtraction is SIGNED (`ble`) -- agbcc knows the zero-extended
 * operands of the first two are non-negative, but the difference in the third
 * can wrap. */
struct Unk1A1A0
{
    /* 00 */ u8 unk00;
    /* 01 */ u8 unk01;
    /* 02 */ u8 unk02;
    /* 03 */ u8 unk03;
    /* 04 */ u8 unk04;
    /* 05 */ u8 unk05;
};

int ScrollList_CursorUp(struct Unk1A178 *p)
{
    if (p->unk00 == 0)
        return 0;

    p->unk00--;

    if (p->unk00 - p->unk03 < p->unk04 && p->unk03 != 0)
        p->unk03--;

    return 1;
}
asm(".global sub_0801A178\n.thumb_set sub_0801A178, ScrollList_CursorUp\n");

int ScrollList_CursorDown(struct Unk1A1A0 *p)
{
    u8 v = p->unk01 - p->unk02;

    if (p->unk01 <= p->unk00)
        return 0;

    p->unk00++;

    if (p->unk00 - p->unk03 > p->unk05 && p->unk03 <= v)
        p->unk03++;

    return 1;
}
asm(".global sub_0801A1A0\n.thumb_set sub_0801A1A0, ScrollList_CursorDown\n");
