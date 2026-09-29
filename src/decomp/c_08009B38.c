#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08009B38.
 * sub_08009B38 @ 0x08009B38, sub_08009B84 @ 0x08009B84
 */

int IsPlainRiverAt(int x, int y)
{
    int idx;
    int terrain;

    idx = gMap->rowOffset[y] + x;
    terrain = gMap->terrain[idx];

    if (terrain == 2)
        return sub_080094EC(x, y) == 0;
    return 0;
}
asm(".global sub_08009B38\n.thumb_set sub_08009B38, IsPlainRiverAt\n");

int sub_08009B84(int x, int y)
{
    int v;
    int w;
    int m;

    v = sub_0800A6AC(x, y);
    if (v == 0)
    {
        v = sub_0800A884(x, y);
        if (v != 0)
        {
            w = sub_0800A95C(x, y);
            if (w != 0)
                v = w;
        }
    }
    else
    {
        w = sub_0800A95C(x, y);
        if (w != 0)
        {
            m = v & 0xFE00;
            if (m == 0x4000)
                v = w;
            if (m == 0x2000 && (v & 0xFE00) == m)
                v = w;
            if (m == 0)
                v = w;
        }
    }
    return v;
}
