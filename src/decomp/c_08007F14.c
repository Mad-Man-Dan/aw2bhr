#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08007F14.
 * MakeTile2 @ 0x08007F14
 */

void MakeTile2(int x, int y, int v)
{
    MakeTileSimple(x, y, v);

    if (v == 0x2A)
        return;

    if (v == 1)
    {
        int n = y - 1;

        if (IsTerrainAtCoordsType(x, n, 1))
            MakeTileSimple(x, n, 1);

        sub_0800ABD0(x, y);
    }

    MakeForestSimple(x, y);
    RepaintNeighbours(x, y);
    sub_0800A588(x, y);
}

asm(".global sub_08007F14\n.thumb_set sub_08007F14, MakeTile2\n");
