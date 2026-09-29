#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B42C.
 * sub_0804B42C @ 0x0804B42C, sub_0804B4C4 @ 0x0804B4C4
 */

/* Scores the four cardinal neighbours of (x, y) on the gMap
 * and returns the terrain code of the best-scoring in-bounds one. Byte-
 * identical twin of FindBestNeighborTerrainRoad, which reads the adjacent table
 * gUnknown_08551CBD instead of gUnknown_08551CA0.
 *
 * Two things in here are not free choices:
 *   `best` is `u8`, NOT `int` or `u32`. All three produce the same unsigned
 * `cmp; bcs` -- the two operands are zero-extended QImode values, so gcc picks
 * the unsigned compare whatever the wider type is -- but only the QImode local
 * makes `best = tbl[t]` a second `ldrb` off the surviving address register.
 * Anything wider is a `mov` off the value CSE already has, which is the same
 * size and the wrong two bytes. Suspect the TYPE before the allocation.
 *   `d` walks gUnknown_08551CDC and is materialised BEFORE `rows`/`terrain`.
 * The .rodata address-constant order in the ROM (gMap, 08551CDC, rowOffset,
 * terrain, 08551CA0) is the readout: a `gUnknown_08551CDC[i]` subscript inside
 * the loop is strength-reduced into the same pointer but creates its address
 * LAST, after the two loop-invariant sums.
 */
int FindBestNeighborTerrainPlain(int x, int y)
{
    struct Map *p;
    const s32 *d;
    u16 *rows;
    u8 *terrain;
    u8 best;
    int result;
    int i;
    int nx;
    int ny;
    int t;

    best = 0;
    result = 0;
    p = gMap;
    d = gUnknown_08551CDC[0];
    rows = p->rowOffset;
    terrain = p->terrain;

    for (i = 0; i < 4; i++)
    {
        nx = x + d[0];
        ny = y + d[1];

        if (nx <= p->width && nx >= 0 && ny <= p->height && ny >= 0)
        {
            t = *(terrain + (rows[ny] + nx)) & 0x1f;

            if (best < gUnknown_08551CA0[t])
            {
                result = t;
                best = gUnknown_08551CA0[t];
            }
        }

        d += 2;
    }

    return result;
}
asm(".global sub_0804B42C\n.thumb_set sub_0804B42C, FindBestNeighborTerrainPlain\n");

/* Byte-identical twin of FindBestNeighborTerrainPlain -- SAME SOURCE apart from the scoring
 * table, gUnknown_08551CBD instead of gUnknown_08551CA0. That symbol's ODD
 * address is real: the two tables are adjacent 29- and 31-byte runs that tile
 * 0x08551CA0..0x08551CDC exactly, and they differ in ROM only at element 5.
 * See FindBestNeighborTerrainPlain for the `u8 best` and pointer-materialisation notes.
 */
int FindBestNeighborTerrainRoad(int x, int y)
{
    struct Map *p;
    const s32 *d;
    u16 *rows;
    u8 *terrain;
    u8 best;
    int result;
    int i;
    int nx;
    int ny;
    int t;

    best = 0;
    result = 0;
    p = gMap;
    d = gUnknown_08551CDC[0];
    rows = p->rowOffset;
    terrain = p->terrain;

    for (i = 0; i < 4; i++)
    {
        nx = x + d[0];
        ny = y + d[1];

        if (nx <= p->width && nx >= 0 && ny <= p->height && ny >= 0)
        {
            t = *(terrain + (rows[ny] + nx)) & 0x1f;

            if (best < gUnknown_08551CBD[t])
            {
                result = t;
                best = gUnknown_08551CBD[t];
            }
        }

        d += 2;
    }

    return result;
}
asm(".global sub_0804B4C4\n.thumb_set sub_0804B4C4, FindBestNeighborTerrainRoad\n");
