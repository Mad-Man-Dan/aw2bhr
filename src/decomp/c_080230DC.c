#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080230DC.
 * sub_080230DC @ 0x080230DC, sub_08023168 @ 0x08023168
 */

/* Camera ease-in: halve the distance to the target each call, and land exactly
 * on it once the gap is 1. Parked in waves 33 and 35 at 18.6%; the gap was the
 * control flow, not the arithmetic.
 *
 * THE GOTOS ARE LOAD-BEARING -- do not fold them back into nested ifs. The
 * `xset`/`xhalve` blocks are reached from BOTH arms of the sign dispatch, so
 * they have two predecessors and start a fresh extended basic block, which is
 * why the ROM re-derives `(s16)a1` there (`lsls r0, r5, #16; asrs r0, r0, #16`)
 * instead of reusing the guard's still-live sign extension. Writing the body
 * out in each arm -- which is what both parked drafts did -- lets CSE reuse the
 * difference in the >= 0 arm only, the two copies stop being identical, and
 * cross-jumping can no longer merge them. See the wave-42 chapter in
 * docs/agbcc-codegen.md.
 *
 * Also deliberate: the then-arm spells both jumps (`goto xtest; goto xdone;`)
 * to get `bgt _body; b _join`, and the two differences are separate variables
 * with textually different expressions so the second `cmp rN, #0` survives.
 *
 * The Y half compares against 1 twice on two registers while the X half
 * compares once; that is register allocation, not a second idiom -- X's two
 * difference pseudos coalesced and cross-jumping merged the compare too. Same
 * source for both halves. */

void EaseMapCursorDisplayToward(s16 a1, s16 a2, s16 a3, s16 *outX, s16 *outY)
{
    u16 x;
    u16 y;
    int dx;
    int ex;
    int dy;
    int ey;

    x = gUnknown_030033E0.unk00;
    y = gUnknown_030033E0.unk02;

    dx = a1 - x;
    if (dx >= 0)
    {
        if (dx > 0)
            goto xtest;
        goto xdone;
    }
    else
    {
        ex = x - a1;
        if (ex <= 0)
            goto xdone;
        if (ex != 1)
            goto xhalve;
        goto xset;
    }
xtest:
    if (dx != 1)
        goto xhalve;
xset:
    x = a1;
    goto xdone;
xhalve:
    x = x + ((a1 - x) >> 1);
xdone:

    dy = a2 - y;
    if (dy >= 0)
    {
        if (dy > 0)
            goto ytest;
        goto ydone;
    }
    else
    {
        ey = y - a2;
        if (ey <= 0)
            goto ydone;
        if (ey != 1)
            goto yhalve;
        goto yset;
    }
ytest:
    if (dy != 1)
        goto yhalve;
yset:
    y = a2;
    goto ydone;
yhalve:
    y = y + ((a2 - y) >> 1);
ydone:

    gUnknown_030033E0.unk00 = x;
    gUnknown_030033E0.unk02 = y;
    *outX = x;
    *outY = y;
}
asm(".global sub_080230DC\n.thumb_set sub_080230DC, EaseMapCursorDisplayToward\n");

/* Camera step: snap straight to the target if either axis is more than 0x10
 * away, otherwise creep 4 pixels per call. Sibling of EaseMapCursorDisplayToward, parked in
 * wave 35 at 38.7%. Three things here each cost a wave; all three are written
 * up in the wave-42 chapters of docs/agbcc-codegen.md.
 *
 * 1. THE GOTOS ARE LOAD-BEARING. The step bodies have two predecessors, which
 *    is why they re-derive `(s16)a1` from the saved shifted value rather than
 *    reusing the guard's. Duplicating them into each arm does not reproduce it.
 *
 * 2. `sx` AND `sy` EXIST TO SPLIT REGISTERS. Binding the sign-extended
 *    parameter to its own local before either difference is created is what
 *    puts the difference in r1 and the extension in r0. Without it both
 *    differences land in r0, the two arms of the snap guard become identical
 *    instruction sequences, cross-jumping collapses them, and the function
 *    comes out 8 bytes short.
 *
 * 3. `x`/`y` ARE int AND `vx`/`vy` ARE u16, and that pairing is the whole step
 *    body. The ROM's no-change arm truncates (`lsls r0, r4, #16`), which only
 *    happens for an int-typed expression assigned to a u16; and its tail is
 *    `lsrs r0, r0, #16; adds r4, r0, #0` -- two instructions, i.e. a narrow
 *    temporary assigned into a wider variable, not a direct narrow store. A
 *    three-way `?:` CANNOT produce this: `X ? A op B : A` hits expand_expr's
 *    copy-then-conditionally-apply optimisation and presets instead of
 *    branching. Widening x to int is why the snap arm needs `(u16)a1`. */

void StepMapCursorDisplayToward(s16 a1, s16 a2, s16 a3, s16 *outX, s16 *outY)
{
    int x;
    int y;
    int sx;
    int sy;
    int dx;
    int ex;
    int dy;
    int ey;
    int dx2;
    int ex2;
    int dy2;
    int ey2;
    int tx;
    int ty;
    u16 vx;
    u16 vy;

    x = gUnknown_030033E0.unk00;
    y = gUnknown_030033E0.unk02;

    sx = a1;
    dx = sx - x;
    if (dx >= 0)
    {
        if (dx > 0x10)
            goto snap;
    }
    else
    {
        ex = x - sx;
        if (ex > 0x10)
            goto snap;
    }

    sy = a2;
    dy = sy - y;
    if (dy >= 0)
    {
        if (dy > 0x10)
            goto snap;
        goto steps;
    }
    else
    {
        ey = y - sy;
        if (ey <= 0x10)
            goto steps;
    }

snap:
    x = (u16)a1;
    y = (u16)a2;
    goto store;

steps:
    dx2 = a1 - x;
    if (dx2 >= 0)
    {
        if (dx2 > 0)
            goto xstep;
        goto xdone;
    }
    else
    {
        ex2 = x - a1;
        if (ex2 <= 0)
            goto xdone;
    }
xstep:
    tx = a1 - x;
    if (tx <= 0)
    {
        if (tx < 0)
            vx = x - 4;
        else
            vx = x;
    }
    else
        vx = x + 4;
    x = vx;
xdone:

    dy2 = a2 - y;
    if (dy2 >= 0)
    {
        if (dy2 > 0)
            goto ystep;
        goto store;
    }
    else
    {
        ey2 = y - a2;
        if (ey2 <= 0)
            goto store;
    }
ystep:
    ty = a2 - y;
    if (ty <= 0)
    {
        if (ty < 0)
            vy = y - 4;
        else
            vy = y;
    }
    else
        vy = y + 4;
    y = vy;

store:
    gUnknown_030033E0.unk00 = x;
    gUnknown_030033E0.unk02 = y;
    *outX = x;
    *outY = y;
}
asm(".global sub_08023168\n.thumb_set sub_08023168, StepMapCursorDisplayToward\n");
