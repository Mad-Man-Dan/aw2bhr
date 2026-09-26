#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08009DFC.
 * sub_08009DFC @ 0x08009DFC
 */

#define MAP08009DFC gMap

/*
 * sub_08009DFC -- count how many of the four cells next to (x, y) are water.
 *
 * Water here means terrain 7, 0xD or 2 -- the same ids MakeBridge will span.
 * If (x, y) is itself one of them the answer is -1. Otherwise the four
 * neighbours that are on the map are counted, and a cell surrounded on all
 * four sides gives -1 as well, unless sub_080094EC objects to the cell.
 *
 * MAP08009DFC is only another name for gMap.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The two left/right blocks keep their own `idx` and adjust it in separate
 *     statements (`idx--; idx += x;`). In one expression the compiler folds the
 *     arithmetic differently.
 */

int sub_08009DFC(int x, int y)
{
    int v;
    int n;

    v = MAP08009DFC->terrain[MAP08009DFC->rowOffset[y] + x];
    if (v == 7 || v == 0xD || v == 2)
        return -1;

    n = 0;
    if (y > 0)
    {
        v = MAP08009DFC->terrain[MAP08009DFC->rowOffset[y - 1] + x];
        if (v == 7 || v == 0xD || v == 2)
            n++;
    }

    if (y < MAP08009DFC->height - 1)
    {
        v = MAP08009DFC->terrain[MAP08009DFC->rowOffset[y + 1] + x];
        if (v == 7 || v == 0xD || v == 2)
            n++;
    }

    if (x > 0)
    {
        int idx;

        idx = MAP08009DFC->rowOffset[y];
        idx--;
        idx += x;
        v = MAP08009DFC->terrain[idx];
        if (v == 7 || v == 0xD || v == 2)
            n++;
    }

    if (x < MAP08009DFC->width - 1)
    {
        int idx;

        idx = MAP08009DFC->rowOffset[y];
        idx++;
        idx += x;
        v = MAP08009DFC->terrain[idx];
        if (v == 7 || v == 0xD || v == 2)
            n++;
    }

    if (n == 4 && sub_080094EC(x, y) == 0)
        n = -1;
    return n;
}
