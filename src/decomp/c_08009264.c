#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08009264.
 * sub_08009264 @ 0x08009264
 */

void sub_08009264(int x, int y)
{
    int v;
    int d;

    if (IsPlainRiverAt(x, y))
    {
        v = 0;
        switch (sub_08008CB8(x, y))
        {
        case 0:
        case 8:
        case 9:
            d = GetLandNeighbourMask(x, y);
            if (d & 6)
                v = 0x13;
            break;
        case 2:
        case 4:
        case 6:
            d = GetLandNeighbourMask(x, y);
            if (d & 9)
                v = 0x16;
            break;
        }
        if (v > 0)
        {
            RemovePropertyAt(x, y);
            SetTerrainAt(x, y, 0xC);
            MakeTileSimple(x, y, v);
            RepaintNeighbours(x, y);
        }
    }
}
