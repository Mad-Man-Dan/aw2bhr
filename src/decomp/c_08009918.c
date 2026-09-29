#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08009918.
 * sub_08009918 @ 0x08009918
 */

#include "map.h"

/* Twin of sub_0800977C (src/decomp/c_0800977C.c), whose staged pointer
 * spelling this reuses. It differs in calling CountRiverOrBridgeNeighbours instead of
 * CountRiverNeighbours, and each neighbour counted must also have the matching
 * tile: 0x16/0x36 horizontally, or tile 0x13 above / 0x14 below the cell
 * vertically. (The vertical tests read the same (x, y-1) and (x, y+1)
 * tiles for both neighbours; that is what the ROM does.)
 *
 * Matching details that differ from the twin:
 *  - `cnt` is declared once for both arms; per-arm declarations duplicate
 *    the shared `adds r6,#1; cmp r6,#1; ble` tail into the x-arm (+8).
 *  - Vertical neighbours read `idx = row + x` in one expression;
 *    horizontal ones step a separate `row` local (`row--; idx = row + x`)
 *    so the row and the index land in different registers. */
int sub_08009918(int x, int y)
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
        int cnt;

        ok = CountRiverOrBridgeNeighbours(x, y) != 0;

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
                cnt = 0;

                if (x > 0)
                {
                    struct Map *p;
                    u8 *rows;
                    u8 *cells;
                    int t2;
                    int idx;
                    int row;

                    p = gMap;
                    t2 = y * 2;
                    rows = (u8 *)p->rowOffset;
                    row = *(u16 *)(rows + t2);
                    row--;
                    idx = row + x;
                    cells = p->terrain;
                    if (*(cells + idx) == 0xC)
                    {
                        int tile2 = p->tile[idx];
                        if (tile2 == 0x16 || tile2 == 0x36)
                            cnt = 1;
                    }
                }

                pe = gMap;

                if (x < pe->width - 1)
                {
                    u8 *rows;
                    u8 *cells;
                    int t2;
                    int idx;
                    int row;

                    t2 = y * 2;
                    rows = (u8 *)pe->rowOffset;
                    row = *(u16 *)(rows + t2);
                    row++;
                    idx = row + x;
                    cells = pe->terrain;
                    if (*(cells + idx) == 0xC)
                    {
                        int tile2 = pe->tile[idx];
                        if (tile2 == 0x16 || tile2 == 0x36)
                            cnt++;
                    }
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
                cnt = 0;

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
                    idx = *(u16 *)(rows + t2) + x;
                    cells = p->terrain;
                    if (*(cells + idx) == 0xC
                        && (p->tile[idx] == 0x13
                            || p->tile[p->rowOffset[y + 1] + x] == 0x14))
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
                    idx = *(u16 *)(rows + t2) + x;
                    cells = pe->terrain;
                    if (*(cells + idx) == 0xC
                        && (pe->tile[pe->rowOffset[y - 1] + x] == 0x13
                            || pe->tile[idx] == 0x14))
                        cnt++;
                }
                if (cnt > 1)
                    ok = 0;
            }
        }
    }

    return terrain == 2 || ok;
}
