#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800EB5C.
 * sub_0800EB5C @ 0x0800EB5C
 */

/*
 * MakeForestBlock3x3 -- draw the 3 x 3 block of tiles for a terrain-4 cell at (x, y).
 *
 * Nothing happens unless the terrain at (x, y) reads 4. The left column goes
 * through sub_0800EBFC with tiles 0x25, 0x45 and 0x65; the other six are
 * written straight with MakeTileSimple, so the block ends up as 0x25 0x26 0x27
 * over 0x45 0x46 0x47 over 0x65 0x66 0x67. What sub_0800EBFC does besides
 * setting its tile is not visible here.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - Both map reads go through the one `map` local, which is what loads the
 *     map pointer once and reads the row table before the terrain plane.
 *     Reaching the bytes by arithmetic on the raw pointer makes the compiler
 *     add the offsets in a different order.
 */

void MakeForestBlock3x3(int x, int y)
{
    struct Map *map = gMap;

    if (map->terrain[map->rowOffset[y] + x] == 4) {
        sub_0800EBFC(x, y, 0x25);
        sub_0800EBFC(x, y + 1, 0x45);
        sub_0800EBFC(x, y + 2, 0x65);
        MakeTileSimple(x + 1, y, 0x26);
        MakeTileSimple(x + 2, y, 0x27);
        MakeTileSimple(x + 1, y + 1, 0x46);
        MakeTileSimple(x + 2, y + 1, 0x47);
        MakeTileSimple(x + 1, y + 2, 0x66);
        MakeTileSimple(x + 2, y + 2, 0x67);
    }
}
asm(".global sub_0800EB5C\n.thumb_set sub_0800EB5C, MakeForestBlock3x3\n");
