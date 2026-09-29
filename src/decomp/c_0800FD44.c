#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800FD44.
 * sub_0800FD44 @ 0x0800FD44
 */

/*
 * GetPipeTile -- choose the tile for a cell that joins onto its neighbours,
 * such as a road or a river, at (x, y).
 *
 * Two shortcuts come first: with a3 == 0 and the cell already showing tile
 * 0x162 or 0x163, GetSeamType answers instead; and if sub_0800F8D4 accepts the
 * cell, the tile it already has is kept.
 *
 * Otherwise GetPipeConnectionAt is asked about each of the four directions -- 0 and 1
 * are the horizontal pair, 2 and 3 the vertical one -- and n counts how many
 * answered. The rest of the function is a ladder on n that names a tile:
 *
 *   0x142  a horizontal piece         0x161  a corner joining 0 and 2
 *   0x143  a vertical piece           0x141  a corner joining 0 and 3
 *   0x122  horizontal, nothing found  0x160  a corner joining 1 and 2
 *   0x123  vertical, nothing found    0x140  a corner joining 1 and 3
 *
 * Inside an arm, a direction that answered 2 outranks one that merely answered
 * non-zero, and where two are still equal sub_0800F77C is asked about each: an
 * answer of 1 or less means that side wins. The n == 4 arm asks twice, first
 * for 0 or less and then for 1 or less. Where nothing decides, the plain
 * horizontal or vertical piece is returned. n above 4 cannot happen and gives
 * -1.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The arms are written out longhand, with the same tests repeated arm after
 *     arm. The original shares those tails, but the sharing is the compiler
 *     merging identical code; a shared helper or a loop in the source gives
 *     something different.
 *   - sub_0800F77C really is called again to compare two values after being
 *     called to test them, so one path makes four calls. That is what the
 *     original does.
 *   - The ladder on n is an if/else-if chain, not a `switch`. A five-case switch
 *     is compiled as a balanced comparison tree and would test 2 first.
 *   - n == 4 is the exception: it sits in a one-case `switch` whose
 *     `default: return -1;` cannot be reached, with the two shared returns as
 *     labels inside it that the arm reaches by `goto` and by falling through.
 *     That is the only arrangement found that keeps the unreachable default and
 *     still stops the compiler merging those two returns with earlier copies;
 *     the `goto ret142` / `goto ret143` jumps in the n == 1 and n == 4 arms are
 *     load-bearing for the same reason.
 */

#define MAP gMap

int GetPipeTile(int x, int y, int a3)
{
    u8 s[4];
    int i;
    int n;

    n = 0;

    if (a3 == 0
     && (u16)(MAP->tile[MAP->rowOffset[y] + x] - 0x162) <= 1)
        return GetSeamType(x, y);

    if (sub_0800F8D4(x, y))
        return MAP->tile[MAP->rowOffset[y] + x];

    for (i = 0; i < 4; i++)
    {
        s[i] = GetPipeConnectionAt(x, y, i);
        if (s[i])
            n++;
    }

    if (n == 0)
        return 0x142;

    if (n == 1)
    {
        if (s[0])
            goto ret142;
        if (s[1])
            goto ret142;
        goto ret143;
    }

    if (n == 2)
    {
        if (s[0] && s[1])
            return 0x142;
        if (s[2] && s[3])
            return 0x143;

        if (s[0] && s[2])
        {
            if (sub_0800F77C(x, y, 0) <= 1 && sub_0800F77C(x, y, 2) <= 1)
                return 0x161;
            if (sub_0800F77C(x, y, 0) <= 1)
                return 0x142;
            if (sub_0800F77C(x, y, 2) <= 1)
                return 0x143;
            return 0x142;
        }
        if (s[0] && s[3])
        {
            if (sub_0800F77C(x, y, 0) <= 1 && sub_0800F77C(x, y, 3) <= 1)
                return 0x141;
            if (sub_0800F77C(x, y, 0) <= 1)
                return 0x142;
            if (sub_0800F77C(x, y, 3) <= 1)
                return 0x143;
            return 0x142;
        }
        if (s[1] && s[2])
        {
            if (sub_0800F77C(x, y, 1) <= 1 && sub_0800F77C(x, y, 2) <= 1)
                return 0x160;
            if (sub_0800F77C(x, y, 1) <= 1)
                return 0x142;
            if (sub_0800F77C(x, y, 2) <= 1)
                return 0x143;
            return 0x142;
        }
        if (s[1] && s[3])
        {
            if (sub_0800F77C(x, y, 1) <= 1 && sub_0800F77C(x, y, 3) <= 1)
                return 0x140;
            if (sub_0800F77C(x, y, 1) <= 1)
                return 0x142;
            if (sub_0800F77C(x, y, 3) <= 1)
                return 0x143;
            return 0x142;
        }
        return 0x122;
    }

    if (n == 3)
    {
        if (s[0] == 2 && s[1] == 2)
            return 0x142;
        if (s[2] == 2 && s[3] == 2)
            return 0x143;
        if (s[0] == 2 && s[2] == 2)
            return 0x161;
        if (s[0] == 2 && s[3] == 2)
            return 0x141;
        if (s[1] == 2 && s[2] == 2)
            return 0x160;
        if (s[1] == 2 && s[3] == 2)
            return 0x140;

        if (s[0] == 2)
        {
            if (s[1])
            {
                if (sub_0800F77C(x, y, 1) <= 1)
                    return 0x142;
                if (s[2])
                {
                    if (sub_0800F77C(x, y, 2) <= 1)
                        return 0x161;
                    return 0x142;
                }
                if (s[3])
                {
                    if (sub_0800F77C(x, y, 3) <= 1)
                        return 0x141;
                    return 0x142;
                }
                return 0x122;
            }
            if (sub_0800F77C(x, y, 2) <= 1
             && sub_0800F77C(x, y, 3) <= 1)
            {
                if (sub_0800F77C(x, y, 2) < sub_0800F77C(x, y, 3))
                    return 0x161;
                return 0x141;
            }
            if (sub_0800F77C(x, y, 2) <= 1)
                return 0x161;
            if (sub_0800F77C(x, y, 3) <= 1)
                return 0x141;
            return 0x142;
        }

        if (s[1] == 2)
        {
            if (s[0])
            {
                if (sub_0800F77C(x, y, 0) <= 1)
                    return 0x142;
                if (s[2])
                {
                    if (sub_0800F77C(x, y, 2) <= 1)
                        return 0x160;
                    return 0x142;
                }
                if (s[3])
                {
                    if (sub_0800F77C(x, y, 3) <= 1)
                        return 0x140;
                    return 0x142;
                }
                return 0x122;
            }
            if (sub_0800F77C(x, y, 2) <= 1
             && sub_0800F77C(x, y, 3) <= 1)
            {
                if (sub_0800F77C(x, y, 2) < sub_0800F77C(x, y, 3))
                    return 0x160;
                return 0x140;
            }
            if (sub_0800F77C(x, y, 2) <= 1)
                return 0x160;
            if (sub_0800F77C(x, y, 3) <= 1)
                return 0x140;
            return 0x142;
        }

        if (s[2] == 2)
        {
            if (s[3])
            {
                if (sub_0800F77C(x, y, 3) <= 1)
                    return 0x143;
                if (s[0])
                {
                    if (sub_0800F77C(x, y, 0) <= 1)
                        return 0x161;
                    return 0x143;
                }
                if (s[1])
                {
                    if (sub_0800F77C(x, y, 1) <= 1)
                        return 0x160;
                    return 0x143;
                }
                return 0x123;
            }
            if (sub_0800F77C(x, y, 0) <= 1
             && sub_0800F77C(x, y, 1) <= 1)
            {
                if (sub_0800F77C(x, y, 0) < sub_0800F77C(x, y, 1))
                    return 0x161;
                return 0x160;
            }
            if (sub_0800F77C(x, y, 0) <= 1)
                return 0x161;
            if (sub_0800F77C(x, y, 1) <= 1)
                return 0x160;
            return 0x143;
        }

        if (s[3] == 2)
        {
            if (s[2])
            {
                if (sub_0800F77C(x, y, 2) <= 1)
                    return 0x143;
                if (s[0])
                {
                    if (sub_0800F77C(x, y, 0) <= 1)
                        return 0x141;
                    return 0x143;
                }
                if (s[1])
                {
                    if (sub_0800F77C(x, y, 1) <= 1)
                        return 0x140;
                    return 0x143;
                }
                return 0x123;
            }
            if (sub_0800F77C(x, y, 0) <= 1
             && sub_0800F77C(x, y, 1) <= 1)
            {
                if (sub_0800F77C(x, y, 0) < sub_0800F77C(x, y, 1))
                    return 0x141;
                return 0x140;
            }
            if (sub_0800F77C(x, y, 0) <= 1)
                return 0x141;
            if (sub_0800F77C(x, y, 1) <= 1)
                return 0x140;
            return 0x143;
        }

        if (sub_0800F77C(x, y, 0) <= 1
         && sub_0800F77C(x, y, 1) <= 1)
            return 0x142;
        if (sub_0800F77C(x, y, 2) <= 1
         && sub_0800F77C(x, y, 3) <= 1)
            return 0x143;
        if (sub_0800F77C(x, y, 0) <= 1
         && sub_0800F77C(x, y, 2) <= 1)
            return 0x161;
        if (sub_0800F77C(x, y, 0) <= 1
         && sub_0800F77C(x, y, 3) <= 1)
            return 0x141;
        if (sub_0800F77C(x, y, 1) <= 1
         && sub_0800F77C(x, y, 2) <= 1)
            return 0x160;
        if (sub_0800F77C(x, y, 1) <= 1
         && sub_0800F77C(x, y, 3) <= 1)
            return 0x140;
        if (sub_0800F77C(x, y, 0) <= 1)
            return 0x142;
        if (sub_0800F77C(x, y, 1) <= 1)
            return 0x142;
        if (sub_0800F77C(x, y, 2) <= 1)
            return 0x143;
        if (sub_0800F77C(x, y, 3) <= 1)
            return 0x143;
        return 0x142;
    }

    switch (n)
    {
    case 4:
    {
        if (s[0] == 2 && s[1] == 2)
            return 0x142;
        if (s[2] == 2 && s[3] == 2)
            return 0x143;
        if (s[0] == 2 && s[2] == 2)
            return 0x161;
        if (s[0] == 2 && s[3] == 2)
            return 0x141;
        if (s[1] == 2 && s[2] == 2)
            return 0x160;
        if (s[1] == 2 && s[3] == 2)
            return 0x140;

        if (s[0] == 2)
        {
            if (sub_0800F77C(x, y, 1) <= 0)
                return 0x142;
            if (sub_0800F77C(x, y, 2) <= 0)
                return 0x161;
            if (sub_0800F77C(x, y, 3) <= 0)
                return 0x141;
            if (sub_0800F77C(x, y, 1) <= 1)
                return 0x142;
            if (sub_0800F77C(x, y, 2) <= 1)
                return 0x161;
            if (sub_0800F77C(x, y, 3) <= 1)
                return 0x141;
            return 0x142;
        }

        if (s[1] == 2)
        {
            if (sub_0800F77C(x, y, 0) <= 0)
                return 0x142;
            if (sub_0800F77C(x, y, 2) <= 0)
                return 0x160;
            if (sub_0800F77C(x, y, 3) <= 0)
                return 0x140;
            if (sub_0800F77C(x, y, 0) <= 1)
                return 0x142;
            if (sub_0800F77C(x, y, 2) <= 1)
                return 0x160;
            if (sub_0800F77C(x, y, 3) <= 1)
                return 0x140;
            return 0x142;
        }

        if (s[2] == 2)
        {
            if (sub_0800F77C(x, y, 3) <= 0)
                return 0x143;
            if (sub_0800F77C(x, y, 0) <= 0)
                return 0x161;
            if (sub_0800F77C(x, y, 1) <= 0)
                return 0x160;
            if (sub_0800F77C(x, y, 3) <= 1)
                return 0x143;
            if (sub_0800F77C(x, y, 0) <= 1)
                return 0x161;
            if (sub_0800F77C(x, y, 1) <= 1)
                return 0x160;
            return 0x143;
        }

        if (s[3] == 2)
        {
            if (sub_0800F77C(x, y, 2) <= 0)
                return 0x143;
            if (sub_0800F77C(x, y, 0) <= 0)
                return 0x141;
            if (sub_0800F77C(x, y, 1) <= 0)
                return 0x140;
            if (sub_0800F77C(x, y, 2) <= 1)
                return 0x143;
            if (sub_0800F77C(x, y, 0) <= 1)
                return 0x141;
            if (sub_0800F77C(x, y, 1) <= 1)
                return 0x140;
            return 0x143;
        }

        if (sub_0800F77C(x, y, 0) <= 1
         && sub_0800F77C(x, y, 1) <= 1)
            goto ret142;
        if (sub_0800F77C(x, y, 2) <= 1
         && sub_0800F77C(x, y, 3) <= 1)
            goto ret143;
        if (sub_0800F77C(x, y, 0) <= 1
         && sub_0800F77C(x, y, 2) <= 1)
            return 0x161;
        if (sub_0800F77C(x, y, 0) <= 1
         && sub_0800F77C(x, y, 3) <= 1)
            return 0x141;
        if (sub_0800F77C(x, y, 1) <= 1
         && sub_0800F77C(x, y, 2) <= 1)
            return 0x160;
        if (sub_0800F77C(x, y, 1) <= 1
         && sub_0800F77C(x, y, 3) <= 1)
            return 0x140;
        if (sub_0800F77C(x, y, 0) <= 1)
            goto ret142;
        if (sub_0800F77C(x, y, 1) <= 1)
            goto ret142;
        if (sub_0800F77C(x, y, 2) <= 1)
            goto ret143;
        if (sub_0800F77C(x, y, 3) > 1)
            goto ret142;
    }
ret143:
        return 0x143;
ret142:
        return 0x142;
    default:
        return -1;
    }
}
asm(".global sub_0800FD44\n.thumb_set sub_0800FD44, GetPipeTile\n");
