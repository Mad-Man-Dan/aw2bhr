#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08007F9C.
 * sub_08007F9C @ 0x08007F9C
 */

void RepaintNeighbours(int x, int y)
{
    if (y > 0)
    {
        int n = y - 1;

        if (x > 0)
        {
            int m = x - 1;
            register s16 ret asm("r0");
            register int v asm("r2");

            ret = sub_08007DD0(m, n);
            asm("lsl %0, %0, #16\n\tasr %1, %0, #16" : "+r"(ret), "=r"(v));
            MakeTileSimple(m, n, v);
            RepaintTile(m, n);
        }

        {
            register s16 ret asm("r0");
            register int v asm("r2");

            ret = sub_08007DD0(x, n);
            asm("lsl %0, %0, #16\n\tasr %1, %0, #16" : "+r"(ret), "=r"(v));
            MakeTileSimple(x, n, v);
            RepaintTile(x, n);
        }

        if (x < gMap->width - 1)
        {
            int m = x + 1;
            register s16 ret asm("r0");
            register int v asm("r2");

            ret = sub_08007DD0(m, n);
            asm("lsl %0, %0, #16\n\tasr %1, %0, #16" : "+r"(ret), "=r"(v));
            MakeTileSimple(m, n, v);
            RepaintTile(m, n);
        }
    }

    if (x > 0)
    {
        int m = x - 1;
        register s16 ret asm("r0");
        register int v asm("r2");

        ret = sub_08007DD0(m, y);
        asm("lsl %0, %0, #16\n\tasr %1, %0, #16" : "+r"(ret), "=r"(v));
        MakeTileSimple(m, y, v);
        RepaintTile(m, y);
    }

    if (x < gMap->width - 1)
    {
        int m = x + 1;
        register s16 ret asm("r0");
        register int v asm("r2");

        ret = sub_08007DD0(m, y);
        asm("lsl %0, %0, #16\n\tasr %1, %0, #16" : "+r"(ret), "=r"(v));
        MakeTileSimple(m, y, v);
        RepaintTile(m, y);
    }

    if (y < gMap->height - 1)
    {
        int n = y + 1;

        if (x > 0)
        {
            int m = x - 1;
            register s16 ret asm("r0");
            register int v asm("r2");

            ret = sub_08007DD0(m, n);
            asm("lsl %0, %0, #16\n\tasr %1, %0, #16" : "+r"(ret), "=r"(v));
            MakeTileSimple(m, n, v);
            RepaintTile(m, n);
        }

        {
            register s16 ret asm("r0");
            register int v asm("r2");

            ret = sub_08007DD0(x, n);
            asm("lsl %0, %0, #16\n\tasr %1, %0, #16" : "+r"(ret), "=r"(v));
            MakeTileSimple(x, n, v);
            RepaintTile(x, n);
        }

        if (x < gMap->width - 1)
        {
            int m = x + 1;
            register s16 ret asm("r0");
            register int v asm("r2");

            ret = sub_08007DD0(m, n);
            asm("lsl %0, %0, #16\n\tasr %1, %0, #16" : "+r"(ret), "=r"(v));
            MakeTileSimple(m, n, v);
            RepaintTile(m, n);
        }
    }

    RepaintPipesAround(x, y);
}
asm(".global sub_08007F9C\n.thumb_set sub_08007F9C, RepaintNeighbours\n");

/*
 * RepaintNeighbours (the function above) -- redraw the eight cells around (x, y)
 * after (x, y) itself changed.
 *
 * For each of the eight neighbours that is on the map it asks sub_08007DD0 for
 * that cell's tile shape, stores it with MakeTileSimple and repaints the cell.
 * The centre cell is left to RepaintPipesAround at the end.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - Every site pins sub_08007DD0's result to r0 and the outgoing argument to
 *     r2, and sign-extends with two inline assembly instructions. In plain C
 *     -- an s16 local, a cast, or an int local -- the compiler copies the
 *     result into another register and narrows it there, which is two bytes
 *     longer at each of the eight sites. No plain-C spelling has been found
 *     that gives the original's pair of shifts.
 *   - The neighbours are written out as eight separate blocks with their own
 *     `m` and `n` locals rather than as a loop.
 */
