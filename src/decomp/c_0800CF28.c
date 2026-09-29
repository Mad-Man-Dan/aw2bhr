#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800CF28.
 * MakeForest @ 0x0800CF28
 */

/* The full "a cell changed" refresh, and the widest of the family: it is
 * sub_0800AF74's list (sub_0800A588 / sub_0800ABD0 / RepaintNeighbours /
 * sub_0800BEE4) with MakeForestBlocks in front of it, wrapped in two guards.
 *
 * The struct is c_0800C22C.c's / c_0800AF24.c's, copied verbatim.
 *
 * `(u16)(cells[off] - 0x86) <= 1` in the ROM is fold_range_test's output for
 * the source's `cells[off] == 0x86 || cells[off] == 0x87` -- do not author the
 * subtract.  The whole guard is therefore
 * `86 || 87 || terrain != 4`, and the `beq` on terrain == 4 is the only path
 * that skips the re-tile block.
 *
 * `off` is one binding local and stays one: the ROM keeps rowOffset[y] + x in
 * r3 across the u16 cells load and reuses it for the u8 terrain load, so the
 * two accesses share the index rather than recomputing it. */

#define MAP gMap

void MakeForest(int x, int y)
{
    int off;

    if (GetPropertyKindAt(x, y))
    {
        RemovePropertyAt(x, y);
        SetTerrainAt(x, y, 1);
    }

    off = MAP->rowOffset[y] + x;

    if (MAP->tile[off] == 0x86 || MAP->tile[off] == 0x87 || MAP->terrain[off] != 4)
    {
        RepaintTileRight(x, y);
        SetTerrainAt(x, y, 4);
        MakeTile2(x, y, 0x87);
        MakeForestSimple(x, y);
    }

    MakeForestBlocks(x, y);
    sub_0800A588(x, y);
    sub_0800ABD0(x, y);
    RepaintNeighbours(x, y);
    sub_0800BEE4(x, y);
}

asm(".global sub_0800CF28\n.thumb_set sub_0800CF28, MakeForest\n");
