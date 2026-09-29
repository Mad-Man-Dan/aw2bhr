#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805B980.
 * sub_0805B980 @ 0x0805B980, sub_0805BA34 @ 0x0805BA34, sub_0805BAFC @ 0x0805BAFC, sub_0805BB8C @ 0x0805BB8C, sub_0805BBF8 @ 0x0805BBF8
 */

/* AiListEnemyPropertyCells's shape without the predicate call and without the count: fills
 * the gUnknown_03003F20 scratch list with {x, y, terrain} for every passable
 * map cell whose byte in the 0x3C72 plane is non-zero, and terminates it with a
 * 0xFFFF value halfword. Nothing is returned -- `pop {r0}; bx r0` with no value
 * computed after the store.
 *
 * The 4-byte {u8 x; u8 y; s16 v;} record is c_0804151C.c's layout for this same
 * buffer; gUnknown_03003F20 stays declared `struct Unk03003338 *` and is cast
 * here, exactly as the promoted readers do. Do NOT reshape struct Unk03003338.
 *
 * `gMap->unk3C72[gMap->rowOffset[y] + x]` (include/map.h) reproduces
 * BuildBoardableTransportList's cell-addressing idiom without naming p/rows/off/cells by
 * hand -- the struct member keeps the same `(base + 0x3C72) + idx`
 * association those intermediates existed to force. `t = y * 2` and
 * `&gUnknown_03003340[y]` both end up in the OUTER loop's preheader (sl and r4);
 * that is LICM, not source, and neither is authored.
 */

struct Unk5B980Cell
{
    /* 00 */ u8 x;
    /* 01 */ u8 y;
    /* 02 */ s16 v;
};
/* The blob struct UnitType's unk14 points at; only the +0x1a table of
 * per-terrain-code permission bytes is proved here. */
struct UnitTypeUnk14
{
    /* 0x00 */ u8 filler_00[0x1a];
    /* 0x1a */ u8 terrainOk[0x20];
};

void AiListLandingCells(void)
{
    struct Unk5B980Cell *out;
    int x;
    int y;

    out = (struct Unk5B980Cell *)gUnknown_03003F20;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] >= 0)
            {
                if (gMap->unk3C72[gMap->rowOffset[y] + x] != 0)
                {
                    out->x = x;
                    out->y = y;
                    out->v = (s8)gUnknown_03003340[y][x];
                    out++;
                }
            }
        }
    }

    out->v = 0xFFFF;
}
asm(".global sub_0805B980\n.thumb_set sub_0805B980, AiListLandingCells\n");

u8 AiPickCargoDropCell(int x, int y, u16 *out)
{
    struct Unit *unit;
    struct UnitTypeUnk14 *tbl;
    int idx;
    int best;

    idx = gMap->rowOffset[y] + x;

    if (gMap->unit[idx] != 0)
        return 0;

    tbl = (struct UnitTypeUnk14 *)gUnknown_085D5ABC[23].transportTable;

    if (tbl->terrainOk[gMap->terrain[idx]
                       & 0x1f] == 0)
        return 0;

    unit = &gUnits[gUnknown_030040D8->unk07[0]];

    best = 9999;
    out[0] = best;

    AiConsiderCargoDropNeighbour(x - 1, y, unit->type, out);
    AiConsiderCargoDropNeighbour(x + 1, y, unit->type, out);
    AiConsiderCargoDropNeighbour(x, y - 1, unit->type, out);
    AiConsiderCargoDropNeighbour(x, y + 1, unit->type, out);

    if (out[0] == best)
        return 0;

    return 1;
}
asm(".global sub_0805BA34\n.thumb_set sub_0805BA34, AiPickCargoDropCell\n");

void AiConsiderCargoDropNeighbour(int x, int y, int t, u16 *out)
{
    s8 *costs;
    int idx;
    int c;

    if (x < 0)
        return;
    if (y < 0)
        return;

    if (x >= gMap->width)
        return;
    if (y >= gMap->height)
        return;

    idx = gMap->rowOffset[y] + x;

    if (gMap->unit[idx] != 0)
        return;

    costs = gUnknown_085D3DD0[1].power[0].movementChart[0];

    c = (gMap->terrain[idx] & 0x1f)
        + gUnknown_085D5ABC[t].movementType * 32;

    if (costs[c] == -1)
        return;

    out[0] = x;
    out[1] = y;
}
asm(".global sub_0805BAFC\n.thumb_set sub_0805BAFC, AiConsiderCargoDropNeighbour\n");

u8 AiHasCargoRoomBeside(int x, int y)
{
    int n;

    if (gMap
            ->unit[gMap->rowOffset[y] + x]
        != 0)
        return 0;

    n = AiIsCellEmptyAndFootPassable(x - 1, y) + AiIsCellEmptyAndFootPassable(x + 1, y)
        + AiIsCellEmptyAndFootPassable(x, y - 1) + AiIsCellEmptyAndFootPassable(x, y + 1);

    if (n > 0)
        return 1;

    return 0;
}
asm(".global sub_0805BB8C\n.thumb_set sub_0805BB8C, AiHasCargoRoomBeside\n");

int AiIsCellEmptyAndFootPassable(int x, int y)
{
    s8 *costs;
    int idx;
    int c;

    if (x < 0)
        return 0;
    if (y < 0)
        return 0;

    if (x >= gMap->width)
        return 0;
    if (y >= gMap->height)
        return 0;

    idx = gMap->rowOffset[y] + x;

    if (gMap->unit[idx] != 0)
        return 0;

    costs = gUnknown_085D3DD0[1].power[0].movementChart[0];

    c = (gMap->terrain[idx] & 0x1f)
        + gUnknown_085D5ABC[1].movementType * 32;

    if (costs[c] == -1)
        return 0;

    return 1;
}
asm(".global sub_0805BBF8\n.thumb_set sub_0805BBF8, AiIsCellEmptyAndFootPassable\n");
