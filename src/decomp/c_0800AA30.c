#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800AA30.
 * CountLandOnSide @ 0x0800AA30
 */

/* Sums IsTerrainLand over the five in-bounds cells of one half-plane around
 * (x, y): the whole neighbouring column (dir 0 = west, 1 = east) or row
 * (2 = north, 4 = south), plus the two cells beside (x, y) on the other axis.
 * The map descriptor is `**pp` == gUnknown_08499590, whose header is
 * `u16 width` at +0 and `u16 height` at +2, so every guard is a bounds test.
 *
 * `pp`, `q` and `lim` are ALL the -fforce-addr workaround and none of them is
 * the original source, which simply named gUnknown_08499590 everywhere.  That
 * spelling gives 416 of 416 bytes with every register right, but routes the
 * address through this unit's own .rodata, which the .text-only split cannot
 * place.  Reaching the ROM's relocation needs the pool word named, and then
 * three things have to be reproduced by hand that CSE did for the original:
 *
 *  - `pp` is FUNCTION scope, assigned separately in each arm.  Per-arm locals
 *    give cases 0/2 r8 and cases 1/4 r9, and cases 2 and 4 then fail to
 *    cross-jump (the ROM shares _0800AB84 and _0800AB96 between them, which
 *    needs both arms holding pp in the same register).  One pseudo, four
 *    assignments, four pool words -- as the ROM has.
 *  - `q` is the MIDDLE level, bound explicitly, in cases 1 and 4 only.  Those
 *    are the two arms whose first read is in the arm's entry block, so the
 *    original's CSE kept &gUnknown_08499590 live across the calls (r8) and
 *    reloaded only the inner pointer.  Cases 0 and 2 read first from inside a
 *    nested `if` and re-derive both levels, which is plain `**pp`.  Without
 *    `q` the draft is 44 bytes short; `**pp` after the merge is not optional
 *    either, since the ROM re-derives there.
 *  - `lim` exists purely to place the `mov sb, r1` copy.  agbcc emits setup in
 *    source-statement order, and the ROM has that copy AFTER the bound is
 *    computed; with `pp = &g` as the statement before the `if`, it lands four
 *    instructions early.  Worth 4 bytes in each of the two arms.
 *
 * `t` is declared inside the guard on independent evidence: at function scope
 * its live range spans all four arms and it outranks the parameters, giving
 * t=r4/x=r5/y=r6 where the ROM has x=r4/y=r5/r=r6/t=r7. */
int CountLandOnSide(int x, int y, int dir)
{
    u8 **const *pp;
    int r = 0;

    switch (dir)
    {
    case 0:
        pp = &gUnknown_0808D854;

        if (x > 0)
        {
            int t = x - 1;
            if (y > 0)
                r += IsTerrainLand(t, y - 1);
            r += IsTerrainLand(t, y);
            if (y < *(u16 *)(**pp + 2) - 1)
                r += IsTerrainLand(t, y + 1);
        }
        if (y > 0)
            r += IsTerrainLand(x, y - 1);
        if (y < *(u16 *)(**pp + 2) - 1)
            r += IsTerrainLand(x, y + 1);
        break;

    case 1:
    {
        u8 **q;
        int lim;

        q = gUnknown_0808D854;
        lim = *(u16 *)*q - 1;
        pp = &gUnknown_0808D854;

        if (x < lim)
        {
            int t = x + 1;
            if (y > 0)
                r += IsTerrainLand(t, y - 1);
            r += IsTerrainLand(t, y);
            if (y < *(u16 *)(*q + 2) - 1)
                r += IsTerrainLand(t, y + 1);
        }
        if (y > 0)
            r += IsTerrainLand(x, y - 1);
        if (y < *(u16 *)(**pp + 2) - 1)
            r += IsTerrainLand(x, y + 1);
        break;
    }

    case 2:
        pp = &gUnknown_0808D854;

        if (y > 0)
        {
            int t = y - 1;
            if (x > 0)
                r += IsTerrainLand(x - 1, t);
            r += IsTerrainLand(x, t);
            if (x < *(u16 *)**pp - 1)
                r += IsTerrainLand(x + 1, t);
        }
        if (x > 0)
            r += IsTerrainLand(x - 1, y);
        if (x < *(u16 *)**pp - 1)
            r += IsTerrainLand(x + 1, y);
        break;

    case 4:
    {
        u8 **q;
        int lim;

        q = gUnknown_0808D854;
        lim = *(u16 *)(*q + 2) - 1;
        pp = &gUnknown_0808D854;

        if (y < lim)
        {
            int t = y + 1;
            if (x > 0)
                r += IsTerrainLand(x - 1, t);
            r += IsTerrainLand(x, t);
            if (x < *(u16 *)*q - 1)
                r += IsTerrainLand(x + 1, t);
        }
        if (x > 0)
            r += IsTerrainLand(x - 1, y);
        if (x < *(u16 *)**pp - 1)
            r += IsTerrainLand(x + 1, y);
        break;
    }
    }

    return r;
}

asm(".global sub_0800AA30\n.thumb_set sub_0800AA30, CountLandOnSide\n");
