#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800A3D4.
 * sub_0800A3D4 @ 0x0800A3D4
 */

/* For each of the four cardinal neighbours of (x, y) that is on the map, run
 * the same two-branch cell update: if IsPlainRiverAt accepts the cell, redraw it
 * with sub_08009B84's tile; otherwise, if it is a sub_080094EC cell that
 * sub_0800A798 does not veto, stamp the fixed pair (7, 0x2a).
 *
 * The first block reaches `bl IsPlainRiverAt` with r0 never written -- x is still
 * sitting in it from the prologue -- which is a pass-through, not a one-argument
 * call; the other three blocks copy it back out of r6.
 *
 * `pp` and `lim` are the -fforce-addr workaround, not the original source; the
 * ROM's pool word is agbcc's own address constant for &gUnknown_08499590 and
 * the honest spelling relocates against this unit's .rodata, which the split
 * cannot place.  `lim` is what places the `mov r8, r1` copy: agbcc emits setup
 * in source-statement order and the ROM copies the pool register into r8 AFTER
 * the bound is computed, so the bound needs a statement of its own ahead of
 * `pp = &g`.  Only one pool word here and both reads re-derive both levels, so
 * unlike CountLandOnSide no middle-level local is wanted.
 *
 * `n` is block-scoped and `v` is not, and that is the whole of the last 48
 * bytes: at function scope `n` outranks `v` and the two callee-saved registers
 * come out r4/r5 the wrong way round (the ROM has v=r4, n=r5).  Splitting it
 * into four allocnos drops its priority below `v`.  Ruled out on the same
 * function, all worse: swapping the declaration order (inert, as the register
 * allocation section predicts), block-scoping `v` instead (80.5%), splitting
 * `n` into a separate nx/ny pair (89.0%, no movement at all), and reusing `v`
 * for either predicate's result (+12 and +8 bytes). */

void sub_0800A3D4(int x, int y)
{
    u8 **const *pp;
    int v;
    int lim;

    if (y > 0)
    {
        int n = y - 1;
        if (IsPlainRiverAt(x, n))
        {
            v = sub_08009B84(x, n);
            if (v > 0)
            {
                SetTerrainAt(x, n, 2);
                MakeTileSimple(x, n, v);
            }
        }
        else if (sub_080094EC(x, n))
        {
            if (sub_0800A798(x, n) == 0)
            {
                SetTerrainAt(x, n, 7);
                MakeTileSimple(x, n, 0x2a);
            }
        }
    }

    lim = *(u16 *)(*gUnknown_0808D83C + 2) - 1;
    pp = &gUnknown_0808D83C;

    if (y < lim)
    {
        int n = y + 1;
        if (IsPlainRiverAt(x, n))
        {
            v = sub_08009B84(x, n);
            if (v > 0)
            {
                SetTerrainAt(x, n, 2);
                MakeTileSimple(x, n, v);
            }
        }
        else if (sub_080094EC(x, n))
        {
            if (sub_0800A798(x, n) == 0)
            {
                SetTerrainAt(x, n, 7);
                MakeTileSimple(x, n, 0x2a);
            }
        }
    }

    if (x > 0)
    {
        int n = x - 1;
        if (IsPlainRiverAt(n, y))
        {
            v = sub_08009B84(n, y);
            if (v > 0)
            {
                SetTerrainAt(n, y, 2);
                MakeTileSimple(n, y, v);
            }
        }
        else if (sub_080094EC(n, y))
        {
            if (sub_0800A798(n, y) == 0)
            {
                SetTerrainAt(n, y, 7);
                MakeTileSimple(n, y, 0x2a);
            }
        }
    }

    if (x < *(u16 *)**pp - 1)
    {
        int n = x + 1;
        if (IsPlainRiverAt(n, y))
        {
            v = sub_08009B84(n, y);
            if (v > 0)
            {
                SetTerrainAt(n, y, 2);
                MakeTileSimple(n, y, v);
            }
        }
        else if (sub_080094EC(n, y))
        {
            if (sub_0800A798(n, y) == 0)
            {
                SetTerrainAt(n, y, 7);
                MakeTileSimple(n, y, 0x2a);
            }
        }
    }
}
