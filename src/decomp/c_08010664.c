#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010664.
 * sub_08010664 @ 0x08010664
 */

/*
 * sub_08010664 -- redraw the joining tiles of (x, y) and of its four
 * neighbours after the cell changed.
 *
 * gUnknown_0848896C and gUnknown_08488974 are the x and y steps for the four
 * directions. A neighbour is redrawn when it is on the map and either
 * GetPipeConnectionAt reports 2 or 3 for that direction or the neighbour's own tile is
 * 0x162 or 0x163. Its new tile comes from GetPipeTile in the first two sweeps
 * and from sub_08010B34 in the third.
 *
 * Between the sweeps the centre cell is checked twice against the same list of
 * fourteen joining-tile ids and redrawn when it is one of them, first with
 * GetPipeTile and then with sub_08010B34. The whole order is: neighbours,
 * centre, neighbours again, centre again, neighbours a third time.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The new tile goes into `t` and MakeTileSimple is a separate statement.
 *     Nested as `MakeTileSimple(A, B, GetPipeTile(A, B, 0))` the compiler works
 *     A and B out once and keeps them alive across the call, four instructions
 *     shorter at each of the six sites; the original works them out again.
 *   - The first two sweeps are the same code written out twice, which is what
 *     the original has: two separate loops, neither merged into the other.
 *   - GetPipeConnectionAt is called up to three times with the same arguments in one
 *     condition. A call's result cannot be shared, so the original's source
 *     really does ask three times.
 *   - `(v = y + gUnknown_08488974[i] >= 0)` in the second sweep stores into `v`
 *     for no reason: nothing reads it before it is overwritten. It is there
 *     because it makes the compiler keep one more value in a register across
 *     that sweep, which is what the original does. It reproduces the output and
 *     says nothing about what the original source looked like.
 */

#define MAP gMap

void sub_08010664(int x, int y)
{
    int i;
    int t;
    u16 v;

    for (i = 0; i < 4; i++)
    {
        if (x + gUnknown_0848896C[i] >= 0
         && x + gUnknown_0848896C[i] < MAP->width
         && y + gUnknown_08488974[i] >= 0
         && y + gUnknown_08488974[i] < MAP->height)
        {
            if (GetPipeConnectionAt(x, y, i) != 0
             && (GetPipeConnectionAt(x, y, i) == 2 || GetPipeConnectionAt(x, y, i) == 3))
            {
                t = GetPipeTile(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], 0);
                MakeTileSimple(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], t);
            }
            else if (MAP->tile[MAP->rowOffset[y + gUnknown_08488974[i]] + (x + gUnknown_0848896C[i])] == 0x162
                  || MAP->tile[MAP->rowOffset[y + gUnknown_08488974[i]] + (x + gUnknown_0848896C[i])] == 0x163)
            {
                t = GetPipeTile(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], 0);
                MakeTileSimple(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], t);
            }
        }
    }

    v = MAP->tile[MAP->rowOffset[y] + x];
    if (v == 0x142 || v == 0x143 || v == 0x140 || v == 0x141
     || v == 0x160 || v == 0x161 || v == 0x162 || v == 0x163
     || v == 0x122 || v == 0x123 || v == 0x121 || v == 0x120
     || v == 0x103 || v == 0x102)
    {
        t = GetPipeTile(x, y, 0);
        MakeTileSimple(x, y, t);
    }

    for (i = 0; i < 4; i++)
    {
        if (x + gUnknown_0848896C[i] >= 0
         && x + gUnknown_0848896C[i] < MAP->width
         && (v = y + gUnknown_08488974[i] >= 0)
         && y + gUnknown_08488974[i] < MAP->height)
        {
            if (GetPipeConnectionAt(x, y, i) != 0
             && (GetPipeConnectionAt(x, y, i) == 2 || GetPipeConnectionAt(x, y, i) == 3))
            {
                t = GetPipeTile(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], 0);
                MakeTileSimple(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], t);
            }
            else if (MAP->tile[MAP->rowOffset[y + gUnknown_08488974[i]] + (x + gUnknown_0848896C[i])] == 0x162
                  || MAP->tile[MAP->rowOffset[y + gUnknown_08488974[i]] + (x + gUnknown_0848896C[i])] == 0x163)
            {
                t = GetPipeTile(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], 0);
                MakeTileSimple(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], t);
            }
        }
    }

    v = MAP->tile[MAP->rowOffset[y] + x];
    if (v == 0x142 || v == 0x143 || v == 0x140 || v == 0x141
     || v == 0x160 || v == 0x161 || v == 0x162 || v == 0x163
     || v == 0x122 || v == 0x123 || v == 0x121 || v == 0x120
     || v == 0x103 || v == 0x102)
    {
        t = sub_08010B34(x, y);
        MakeTileSimple(x, y, t);
    }

    for (i = 0; i < 4; i++)
    {
        if (x + gUnknown_0848896C[i] >= 0
         && x + gUnknown_0848896C[i] < MAP->width
         && y + gUnknown_08488974[i] >= 0
         && y + gUnknown_08488974[i] < MAP->height)
        {
            if (GetPipeConnectionAt(x, y, i) != 0
             && (GetPipeConnectionAt(x, y, i) == 2 || GetPipeConnectionAt(x, y, i) == 3))
            {
                t = sub_08010B34(x + gUnknown_0848896C[i], y + gUnknown_08488974[i]);
                MakeTileSimple(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], t);
            }
            else if (MAP->tile[MAP->rowOffset[y + gUnknown_08488974[i]] + (x + gUnknown_0848896C[i])] == 0x162
                  || MAP->tile[MAP->rowOffset[y + gUnknown_08488974[i]] + (x + gUnknown_0848896C[i])] == 0x163)
            {
                t = sub_08010B34(x + gUnknown_0848896C[i], y + gUnknown_08488974[i]);
                MakeTileSimple(x + gUnknown_0848896C[i], y + gUnknown_08488974[i], t);
            }
        }
    }

}
