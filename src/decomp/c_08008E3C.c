#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08008E3C.
 * sub_08008E3C @ 0x08008E3C, MakeBridge @ 0x08008F6C
 */

/*
 * sub_08008E3C -- turn a pair of neighbouring bridge tiles the right way round.
 *
 * 0x13 and 0x16 are the two orientations of a bridge tile. A bridge drawn as
 * 0x13 with another 0x13 directly above or below it should be the other
 * orientation, and the other way round for 0x16 side by side, so both cells of
 * the pair are rewritten with MakeTileSimple. Nothing happens unless the tile
 * at (x, y) is one of the two, and sub_08008CB8 returning non-zero skips the
 * whole check.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The map is read through byte pointers taken from the struct once per
 *     block -- `rows`, `tiles`, and the `q...` copies in the later blocks --
 *     and the row index is multiplied by 2 in its own statement. One set of
 *     variables shared by the whole function, or indexing the arrays directly,
 *     makes the compiler compute the addresses in a different order.
 */

void sub_08008E3C(int x, int y)
{
    struct Map *p;
    u16 *rows;
    u8 *tiles;
    int row;
    int off;
    int v;
    int n;

    if (sub_08008CB8(x, y))
        return;

    p = gMap;
    row = p->rowOffset[y];
    rows = p->rowOffset;
    off = (row + x) * 2;
    tiles = (u8 *)p->tile;
    v = *(u16 *)(tiles + off);

    if (v == 0x13)
    {
        if (y > 0)
        {
            n = y - 1;
            off = (rows[n] + x) * 2;

            if (*(u16 *)(tiles + off) == 0x13)
            {
                MakeTileSimple(x, y, 0x16);
                MakeTileSimple(x, n, 0x16);
            }
        }

        if (y < gMap->height - 1)
        {
            struct Map *q;
            u8 *qrows;
            u8 *qtiles;
            int qt;
            int qoff;

            n = y + 1;
            q = gMap;
            qt = n * 2;
            qrows = (u8 *)q->rowOffset;
            qoff = (*(u16 *)(qrows + qt) + x) * 2;
            qtiles = (u8 *)q->tile;

            if (*(u16 *)(qtiles + qoff) == 0x13)
            {
                MakeTileSimple(x, y, 0x16);
                MakeTileSimple(x, n, 0x16);
            }
        }
    }
    else if (v == 0x16)
    {
        if (x > 0)
        {
            int m = row - 1;

            off = (m + x) * 2;

            if (*(u16 *)(tiles + off) == 0x16)
            {
                MakeTileSimple(x, y, 0x13);
                MakeTileSimple(x - 1, y, 0x13);
            }
        }

        if (x < gMap->width - 1)
        {
            struct Map *q;
            u8 *qrows;
            u8 *qtiles;
            int qoff;

            q = gMap;
            qrows = (u8 *)q->rowOffset;
            qoff = (*(u16 *)(qrows + y * 2) + (x + 1)) * 2;
            qtiles = (u8 *)q->tile;

            if (*(u16 *)(qtiles + qoff) == 0x16)
            {
                MakeTileSimple(x, y, 0x13);
                MakeTileSimple(x + 1, y, 0x13);
            }
        }
    }
}

/*
 * MakeBridge -- build a bridge at (x, y) and pick its graphic.
 *
 * GetLandNeighbourMask and sub_08008CB8 describe the cell's surroundings; each returns
 * a small number whose bits say which sides are joined. The terrain already at
 * (x, y) decides which half of the function runs:
 *
 *   terrain 7, 0xD or 0x13 -- a bridge is laid: RemovePropertyAt clears the cell,
 *     0xD is turned to sea first (MakeSeaSafest), the terrain becomes 0xC and
 *     the tile is 0x36 for a straight span or 0x14 for the other case;
 *     RepaintNeighbours then redraws the neighbours. Some surroundings (3, 5, 10 and
 *     12) build nothing, and the default arm asks sub_08008D70 for the tile
 *     instead and uses it when it is positive.
 *   any other terrain -- only if IsPlainRiverAt allows it, and then only the
 *     terrain and tile are set, to 0xC with tile 0x13 or 0x16, chosen from
 *     whether a bridge cell (terrain 0xC) already sits next to it in that
 *     direction.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The inner tests on sub_08008CB8 are written as three-case `switch`
 *     statements, not as `if` expressions with `||`. The compiler builds a
 *     comparison tree for a switch that is six bytes longer than the `if`, and
 *     those are exactly the bytes the original has.
 *   - Two case arms fall through on purpose: the switch after `case 1: case 8:`
 *     runs into `case 9:`, and the one after `case 2: case 4:` runs into the
 *     code below it. The `goto tile14` / `goto set13` / `goto set16` jumps and
 *     the repeated bodies are also deliberate; sharing them changes the output.
 *   - As in sub_08008E3C above, each block reads the map through its own byte
 *     pointers and multiplies the row index in a separate statement.
 */

void MakeBridge(int x, int y)
{
    struct Map *p;
    u8 *rows;
    u8 *cells;
    int t;
    int idx;
    int cell;

    p = gMap;
    t = y * 2;
    rows = (u8 *)p->rowOffset;
    idx = *(u16 *)(rows + t) + x;
    cells = p->terrain;
    cell = *(cells + idx);

    if (cell == 7 || cell == 0xD || cell == 0x13)
    {
        switch (GetLandNeighbourMask(x, y))
        {
        case 1:
        case 8:
        {
            switch (sub_08008CB8(x, y))
            {
            case 2:
            case 4:
            case 6:
                goto tile14;
            }
        }
        case 9:
            RemovePropertyAt(x, y);
            if (cell == 0xD)
                MakeSeaSafest(x, y);
            SetTerrainAt(x, y, 0xC);
            MakeTileSimple(x, y, 0x36);
            RepaintNeighbours(x, y);
            break;

        case 2:
        case 4:
        {
            switch (sub_08008CB8(x, y))
            {
            case 1:
            case 8:
            case 9:
                break;
            default:
                goto tile14;
            }
        }
            RemovePropertyAt(x, y);
            if (cell == 0xD)
                MakeSeaSafest(x, y);
            SetTerrainAt(x, y, 0xC);
            MakeTileSimple(x, y, 0x36);
            RepaintNeighbours(x, y);
            break;

        case 11:
        case 13:
            RemovePropertyAt(x, y);
            if (cell == 0xD)
                MakeSeaSafest(x, y);
            SetTerrainAt(x, y, 0xC);
            MakeTileSimple(x, y, 0x36);
            RepaintNeighbours(x, y);
            break;

        case 6:
        case 7:
        case 14:
        tile14:
            RemovePropertyAt(x, y);
            if (cell == 0xD)
                MakeSeaSafest(x, y);
            SetTerrainAt(x, y, 0xC);
            MakeTileSimple(x, y, 0x14);
            RepaintNeighbours(x, y);
            break;

        case 3:
        case 5:
        case 10:
        case 12:
            break;

        default:
        {
            int k;

            if (cell == 0xD)
            {
                RemovePropertyAt(x, y);
                MakeSeaSafest(x, y);
            }

            k = sub_08008D70(x, y);

            if (k > 0)
            {
                RemovePropertyAt(x, y);
                SetTerrainAt(x, y, 0xC);
                MakeTileSimple(x, y, k);
            }
            break;
        }
        }
    }
    else
    {
        int v;
        int k;

        if (!IsPlainRiverAt(x, y))
            return;

        v = 0;

        switch (sub_08008CB8(x, y))
        {
        case 0:
        case 8:
        case 9:
            k = GetLandNeighbourMask(x, y);
            if (k & 6)
                goto set13;

            if (x > 0)
            {
                struct Map *q;
                u8 *qrows;
                u8 *qcells;
                int qt;
                int qidx;

                q = gMap;
                qt = y * 2;
                qrows = (u8 *)q->rowOffset;
                qidx = *(u16 *)(qrows + qt) + (x - 1);
                qcells = q->terrain;

                if (*(qcells + qidx) == 0xC)
                    goto set13;
            }

            if (x < gMap->width - 1)
            {
                struct Map *q;
                u8 *qrows;
                u8 *qcells;
                int qt;
                int qidx;

                q = gMap;
                qt = y * 2;
                qrows = (u8 *)q->rowOffset;
                qidx = *(u16 *)(qrows + qt) + (x + 1);
                qcells = q->terrain;

                if (*(qcells + qidx) == 0xC)
                    goto set13;
            }
            break;

        set13:
            v = 0x13;
            break;

        case 2:
        case 4:
        case 6:
            k = GetLandNeighbourMask(x, y);
            if (k & 9)
            {
                v = 0x16;
                break;
            }

            if (y > 0)
            {
                struct Map *q;
                u8 *qrows;
                u8 *qcells;
                int n;
                int qt;
                int qidx;

                q = gMap;
                n = y - 1;
                qt = n * 2;
                qrows = (u8 *)q->rowOffset;
                qidx = *(u16 *)(qrows + qt) + x;
                qcells = q->terrain;

                if (*(qcells + qidx) == 0xC)
                    goto set16;
            }

            if (y < gMap->height - 1)
            {
                struct Map *q;
                u8 *qrows;
                u8 *qcells;
                int n;
                int qt;
                int qidx;

                q = gMap;
                n = y + 1;
                qt = n * 2;
                qrows = (u8 *)q->rowOffset;
                qidx = *(u16 *)(qrows + qt) + x;
                qcells = q->terrain;

                if (*(qcells + qidx) == 0xC)
                    goto set16;
            }
            break;

        set16:
            v = 0x16;
            break;
        }

        if (v > 0)
        {
            RemovePropertyAt(x, y);
            SetTerrainAt(x, y, 0xC);
            MakeTileSimple(x, y, v);
        }
    }
}

asm(".global sub_08008F6C\n.thumb_set sub_08008F6C, MakeBridge\n");
