#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800977C.
 * sub_0800977C @ 0x0800977C
 */

/* Returns whether the cell at (x, y) is a join candidate. The typed gMap
 * spelling is byte-exact here as long as the rowOffset/terrain/tile
 * byte-pointer locals stay scoped per use.
 */
int sub_0800977C(int x, int y)
{
    int t;
    int terrain;
    u8 ok = 0;

    {
        struct Map *p;
        u8 *rows;
        u8 *cells;
        int idx;

        p = gMap;
        t = y * 2;
        rows = (u8 *)p->rowOffset;
        idx = *(u16 *)(rows + t) + x;
        cells = p->terrain;
        terrain = *(cells + idx);
    }

    if (terrain == 0xC)
    {
        int tile;

        ok = CountRiverNeighbours(x, y) != 0;

        {
            struct Map *p;
            u8 *rows;
            u8 *tiles;
            int off;

            p = gMap;
            rows = (u8 *)p->rowOffset;
            off = (*(u16 *)(rows + t) + x) * 2;
            tiles = (u8 *)p->tile;
            tile = *(u16 *)(tiles + off);
        }

        if (tile == 0x16 || tile == 0x36)
        {
            if (x > 0 && sub_08009720(x - 1, y))
                ok = 0;
            if (x < gMap->width - 1 && sub_08009720(x + 1, y))
                ok = 0;

            if (ok)
            {
                struct Map *pe;
                int cnt = 0;

                if (x > 0)
                {
                    struct Map *p;
                    u8 *rows;
                    u8 *cells;
                    int t2;
                    int idx;

                    p = gMap;
                    t2 = y * 2;
                    rows = (u8 *)p->rowOffset;
                    idx = *(u16 *)(rows + t2);
                    idx--;
                    idx += x;
                    cells = p->terrain;
                    if (*(cells + idx) == 0xC)
                        cnt = 1;
                }

                pe = gMap;

                if (x < pe->width - 1)
                {
                    u8 *rows;
                    u8 *cells;
                    int t2;
                    int idx;

                    t2 = y * 2;
                    rows = (u8 *)pe->rowOffset;
                    idx = *(u16 *)(rows + t2);
                    idx++;
                    idx += x;
                    cells = pe->terrain;
                    if (*(cells + idx) == 0xC)
                        cnt++;
                }
                if (cnt > 1)
                    ok = 0;
            }
        }
        else
        {
            if (y > 0 && sub_08009720(x, y - 1))
                ok = 0;
            if (y < gMap->height - 1 && sub_08009720(x, y + 1))
                ok = 0;

            if (ok)
            {
                struct Map *pe;
                int cnt = 0;

                if (y > 0)
                {
                    struct Map *p;
                    u8 *rows;
                    u8 *cells;
                    int t2;
                    int idx;

                    p = gMap;
                    t2 = (y - 1) * 2;
                    rows = (u8 *)p->rowOffset;
                    idx = *(u16 *)(rows + t2);
                    idx += x;
                    cells = p->terrain;
                    if (*(cells + idx) == 0xC)
                        cnt = 1;
                }

                pe = gMap;

                if (y < pe->height - 1)
                {
                    u8 *rows;
                    u8 *cells;
                    int t2;
                    int idx;

                    t2 = (y + 1) * 2;
                    rows = (u8 *)pe->rowOffset;
                    idx = *(u16 *)(rows + t2);
                    idx += x;
                    cells = pe->terrain;
                    if (*(cells + idx) == 0xC)
                        cnt++;
                }
                if (cnt > 1)
                    ok = 0;
            }
        }
    }

    return terrain == 2 || ok;
}
