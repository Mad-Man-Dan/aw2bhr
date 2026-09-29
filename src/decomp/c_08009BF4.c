#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08009BF4.
 * sub_08009BF4 @ 0x08009BF4, sub_08009CF8 @ 0x08009CF8
 */

/*
 * CountRiverNeighbours -- count how many of the four cells next to (x, y) are sea.
 *
 * A neighbour counts when it is on the map, sub_080094EC returns 0 for it, and
 * its terrain is 2. The order is up, down, left, right, and the result is 0
 * to 4.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - Each of the four blocks declares its own p, rows, cells, t and idx, and
 *     computes `t = yy * 2;` in a statement of its own before setting `rows`.
 *     Shared across the whole function the compiler keeps them in different
 *     registers, and hoisting the multiply changes the order of the address
 *     load and the shift.
 */
int CountRiverNeighbours(int x, int y)
{
    int n;
    int yy;

    n = 0;
    if (y > 0)
    {
        yy = y - 1;
        if (sub_080094EC(x, yy) == 0)
        {
            struct Map *p;
            u8 *rows, *cells;
            int t, idx;

            p = gMap;
            t = yy * 2;
            rows = (u8 *)p->rowOffset;
            idx = *(u16 *)(rows + t) + x;
            cells = p->terrain;
            if (*(cells + idx) == 2)
                n++;
        }
    }
    if (y < gMap->height - 1)
    {
        yy = y + 1;
        if (sub_080094EC(x, yy) == 0)
        {
            struct Map *p;
            u8 *rows, *cells;
            int t, idx;

            p = gMap;
            t = yy * 2;
            rows = (u8 *)p->rowOffset;
            idx = *(u16 *)(rows + t) + x;
            cells = p->terrain;
            if (*(cells + idx) == 2)
                n++;
        }
    }
    if (x > 0)
    {
        if (sub_080094EC(x - 1, y) == 0)
        {
            struct Map *p;
            u8 *rows, *cells;
            int t, idx;

            p = gMap;
            t = y * 2;
            rows = (u8 *)p->rowOffset;
            idx = *(u16 *)(rows + t) + (x - 1);
            cells = p->terrain;
            if (*(cells + idx) == 2)
                n++;
        }
    }
    if (x < gMap->width - 1)
    {
        if (sub_080094EC(x + 1, y) == 0)
        {
            struct Map *p;
            u8 *rows, *cells;
            int t, idx;

            p = gMap;
            t = y * 2;
            rows = (u8 *)p->rowOffset;
            idx = *(u16 *)(rows + t) + (x + 1);
            cells = p->terrain;
            if (*(cells + idx) == 2)
                n++;
        }
    }
    return n;
}
asm(".global sub_08009BF4\n.thumb_set sub_08009BF4, CountRiverNeighbours\n");

/*
 * CountRiverOrBridgeNeighbours -- count how many of the four cells next to (x, y) are sea or
 * bridge.
 *
 * The same count as CountRiverNeighbours above, except that terrain 0xC (a bridge) is
 * accepted as well as terrain 2, and the terrain byte is read before
 * sub_080094EC is called rather than after it.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - Reading the map before the call is what makes the original keep the
 *     terrain byte in a register that survives it; the two are not
 *     interchangeable even though the result is the same.
 *   - The per-block p, rows, cells, t and idx, and the separate `t = yy * 2;`,
 *     matter here for the same reason as in CountRiverNeighbours.
 *   - The second and fourth blocks put the bound test inside the block, after
 *     `p = gMap;`, so that the map pointer is loaded before the comparison.
 */
int CountRiverOrBridgeNeighbours(int x, int y)
{
    int terrain;
    int n;

    n = 0;
    if (y > 0)
    {
        struct Map *p;
        u8 *rows, *cells;
        int t, idx, yy;

        p = gMap;
        yy = y - 1;
        t = yy * 2;
        rows = (u8 *)p->rowOffset;
        idx = *(u16 *)(rows + t) + x;
        cells = p->terrain;
        terrain = *(cells + idx);
        if (sub_080094EC(x, yy) == 0 && (terrain == 2 || terrain == 0xC))
            n++;
    }
    {
        struct Map *p;
        u8 *rows, *cells;
        int t, idx, yy;

        p = gMap;
        if (y < p->height - 1)
        {
            yy = y + 1;
            t = yy * 2;
            rows = (u8 *)p->rowOffset;
            idx = *(u16 *)(rows + t) + x;
            cells = p->terrain;
            terrain = *(cells + idx);
            if (sub_080094EC(x, yy) == 0 && (terrain == 2 || terrain == 0xC))
                n++;
        }
    }
    if (x > 0)
    {
        struct Map *p;
        u8 *rows, *cells;
        int t, idx;

        p = gMap;
        t = y * 2;
        rows = (u8 *)p->rowOffset;
        idx = *(u16 *)(rows + t) + (x - 1);
        cells = p->terrain;
        terrain = *(cells + idx);
        if (sub_080094EC(x - 1, y) == 0 && (terrain == 2 || terrain == 0xC))
            n++;
    }
    {
        struct Map *p;
        u8 *rows, *cells;
        int t, idx;

        p = gMap;
        if (x < p->width - 1)
        {
            t = y * 2;
            rows = (u8 *)p->rowOffset;
            idx = *(u16 *)(rows + t) + (x + 1);
            cells = p->terrain;
            terrain = *(cells + idx);
            if (sub_080094EC(x + 1, y) == 0 && (terrain == 2 || terrain == 0xC))
                n++;
        }
    }
    return n;
}
asm(".global sub_08009CF8\n.thumb_set sub_08009CF8, CountRiverOrBridgeNeighbours\n");
