#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080439A8.
 * sub_080439A8 @ 0x080439A8
 */

/* MATCHED (wave 37, W37-Q4), one attempt.
 *
 * The whole signature came off the already-promoted caller DrawCoPowerStarBar:
 *   DrawCoPowerStarBarSegment(&va, &vy, &vu, m, lo, 0, vu >= lo, &vt);
 * so a6 is a small selector, a7 a flag and a1/a2/a3/a8 are in-out `int *`.
 * Draws a bar: one PutSprite per pass, *a1 advancing 6 pixels each time,
 * while the remaining amount a5 is positive and stepping down by a4.
 *
 * Three things that are not free choices:
 *
 * - The switch is `case 0: default:` sharing one label, exactly like
 *   PlayArmyCoMusic next door.  The ROM tests `cmp #0; beq default`, then
 *   `cmp #1; beq case1`, and FALLS THROUGH into the default block -- which
 *   only happens when case 0's body IS the default body and is written FIRST.
 *
 * - The `*a3` update is a two-armed if/else with one shared store, not a
 *   ternary: `bge` is the inverted test, so the `<` arm is the fall-through
 *   (movs r0,#0) and the subtract is the else.
 *
 * - `(*a8)--` is a POST-decrement: r0 keeps the old value and the store puts
 *   value-1 back before DivRem is called on the old one.
 *
 * The loop is a plain `while (a5 > 0)`; `bl Div` sits ahead of the `cmp r6,#0;
 * ble` guard, so `half` is a source statement before the loop and not a LICM
 * hoist.  The switch and the a7 test are loop-invariant but stay INSIDE the
 * loop in the ROM, so they are source statements there too.
 */
void DrawCoPowerStarBarSegment(int *a1, int *a2, int *a3, int a4, int a5, int a6, int a7, int *a8)
{
    int half;
    int n;
    u16 *gfx;
    int k;
    int base;
    int d;

    half = Div(a4, 2);
    while (a5 > 0)
    {
        n = Div(*a3, half);
        if (n > 1)
            n = 2;
        if (*a3 < half)
            *a3 = 0;
        else
            *a3 = *a3 - n * half;
        switch (a6)
        {
        case 0:
        default:
            gfx = gUnknown_084A0042;
            k = 1;
            base = 0x20;
            break;
        case 1:
            gfx = gUnknown_084A004A;
            k = 4;
            base = 0x23;
            break;
        }
        if (a7)
            d = gUnknown_084A006E[DivRem((*a8)--, 0x20)];
        else
            d = 0;
        PutSprite(0, *a1, *a2 - d, gfx, (base + n * k) | 0x7000);
        *a1 += 6;
        a5 -= a4;
    }
}
asm(".global sub_080439A8\n.thumb_set sub_080439A8, DrawCoPowerStarBarSegment\n");
