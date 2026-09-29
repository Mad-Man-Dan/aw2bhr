#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800A3A8.
 * sub_0800A3A8 @ 0x0800A3A8
 */

/* One quarter of src/decomp/c_0800A3D4.c, lifted out on its own and without
 * that function's IsPlainRiverAt guard: fetch the cell's tile and, if it is
 * positive, stamp kind 2 and draw it.
 *
 * `v` has to be a local rather than the call written inline twice -- the ROM
 * holds sub_08009B84's result in r6 across both calls, and it is the same
 * `v > 0` test c_0800A3D4.c uses. */
void sub_0800A3A8(int x, int y)
{
    int v;

    v = sub_08009B84(x, y);

    if (v > 0)
    {
        SetTerrainAt(x, y, 2);
        MakeTileSimple(x, y, v);
    }
}
