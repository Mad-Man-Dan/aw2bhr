#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800BCD0.
 * sub_0800BCD0 @ 0x0800BCD0
 */

/*
 * sub_0800BCD0 -- is (x, y) and all eight cells around it terrain 0x13 or 7?
 *
 * The same test is written out nine times, once per cell of the 3 x 3 block,
 * each setting its own bit of `m` (bit 8 the top left, bit 4 the centre, bit 0
 * the bottom right). The centre is done first and a miss returns 0 at once. A
 * cell off the edge of the map leaves its bit clear, so any cell on the border
 * answers 0. The result is `m == 0x1FF`.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `y - 1` and `y + 1` are written out in all six row lookups instead of
 *     being bound to a local. With a local the compiler works the row's scaled
 *     index out once and carries it into the next block, where the original
 *     works it out again.
 *   - `t` and `c` are declared once for the whole function, not per block. This
 *     function makes no calls, so the original keeps both in low registers
 *     throughout; nine separate pairs come out with some of them swapped.
 *   - The side and diagonal indexes are written `(i = <row> -+ 1, i + x)`, so
 *     the 1 is added to the row and x afterwards. In one expression the
 *     compiler folds the 1 into the terrain array's own offset instead.
 */
#define MAP gMap

int sub_0800BCD0(int x, int y)
{
    int m = 0;
    int t;
    int c;

    t = MAP->terrain[MAP->rowOffset[y] + x];
    c = 0;
    if (t == 0x13 || t == 7)
        c = 1;
    m |= c << 4;

    if (m == 0)
        return 0;

    if (y > 0)
    {
        if (x > 0)
        {
            int i;
            t = MAP->terrain[(i = MAP->rowOffset[y - 1] - 1, i + x)];
            c = 0;
            if (t == 0x13 || t == 7)
                c = 1;
            m |= c << 8;
        }
        t = MAP->terrain[MAP->rowOffset[y - 1] + x];
        c = 0;
        if (t == 0x13 || t == 7)
            c = 1;
        m |= c << 7;

        if (x < MAP->width - 1)
        {
            int i;
            t = MAP->terrain[(i = MAP->rowOffset[y - 1] + 1, i + x)];
            c = 0;
            if (t == 0x13 || t == 7)
                c = 1;
            m |= c << 6;
        }
    }

    if (x > 0)
    {
        int i;
        t = MAP->terrain[(i = MAP->rowOffset[y] - 1, i + x)];
        c = 0;
        if (t == 0x13 || t == 7)
            c = 1;
        m |= c << 5;
    }

    if (x < MAP->width - 1)
    {
        int i;
        t = MAP->terrain[(i = MAP->rowOffset[y] + 1, i + x)];
        c = 0;
        if (t == 0x13 || t == 7)
            c = 1;
        m |= c << 3;
    }

    if (y < MAP->height - 1)
    {
        if (x > 0)
        {
            int i;
            t = MAP->terrain[(i = MAP->rowOffset[y + 1] - 1, i + x)];
            c = 0;
            if (t == 0x13 || t == 7)
                c = 1;
            m |= c << 2;
        }
        t = MAP->terrain[MAP->rowOffset[y + 1] + x];
        c = 0;
        if (t == 0x13 || t == 7)
            c = 1;
        m |= c << 1;

        if (x < MAP->width - 1)
        {
            int i;
            t = MAP->terrain[(i = MAP->rowOffset[y + 1] + 1, i + x)];
            c = 0;
            if (t == 0x13 || t == 7)
                c = 1;
            m |= c;
        }
    }

    return m == 0x1ff;
}
