#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800C2D0.
 * sub_0800C2D0 @ 0x0800C2D0, MakeProperty @ 0x0800C454
 */

/* sub_0800C22C's callee -- see c_0800C22C.c, whose 3x3 walk supplies the third
 * argument this one takes.  `f` is that flag: nonzero for the orthogonal
 * neighbours, zero for the diagonals, and all it gates is the four-way
 * MakeRiver sweep in the middle.
 *
 * The struct is c_0800C22C.c's / c_0800AF24.c's, copied verbatim.
 *
 * gUnknown_08499590 is read FIVE times, so -fforce-addr parks its address in
 * this unit's own .rodata and the ROM's `ldr r4, =gUnknown_0808D874;
 * ldr r0, [r4]; ldr r1, [r0]` is a THREE-level chain: 0x0808D874 is the pool
 * word, it holds 0x08499590, and gUnknown_08499590 is the pointer variable.
 * Write the global's name -- the ROM word pointing AT it is not a symbol.
 * The later `ldr r4, [r4]` that swaps the pool address for &gUnknown_08499590
 * in sl is CSE's, not the source's.
 *
 * THE ONE NON-OBVIOUS SPELLING, and it cost the only rewrite here: the two
 * column neighbours must be `MAP->rowOffset[y] + (x - 1)` with the
 * parentheses.  All three spellings differ, measured side by side:
 *   `rowOffset[y] + (x - 1)`  ->  ldrh; subs r0,#1; adds r0,r0,x   <- the ROM
 *   `rowOffset[y] - 1 + x`    ->  subs x,#1 hoisted ABOVE the ldrh
 *   `rowOffset[y] + x - 1`    ->  the -1 folds into the 0x1432 pool word,
 *                                 which becomes 0x1431 and loses the reloc
 * Written up in docs/agbcc-codegen.md.
 *
 * `c` (the terrain byte) is live across every call, which is what puts it in
 * r8 and forces the high-register push; the trailing if/else-if chain tests it
 * in source order 1, 5, 3, 4 and is NOT a switch -- a switch over those four
 * would have been ordered. */

#define MAP gMap

void sub_0800C2D0(int x, int y, int f)
{
    int c;
    int v;

    c = MAP->terrain[MAP->rowOffset[y] + x];
    v = sub_08007DD0(x, y);

    if (v < 0 && c == 7)
        v = 0x2A;

    if (c != 2)
    {
        if (v == 0x2A)
            SetTerrainAt(x, y, 7);
        MakeTileSimple(x, y, v);
    }

    if (f)
    {
        if (MAP->terrain[MAP->rowOffset[y - 1] + x] == 2)
            MakeRiver(x, y - 1);
        if (MAP->terrain[MAP->rowOffset[y + 1] + x] == 2)
            MakeRiver(x, y + 1);
        if (MAP->terrain[MAP->rowOffset[y] + (x - 1)] == 2)
            MakeRiver(x - 1, y);
        if (MAP->terrain[MAP->rowOffset[y] + (x + 1)] == 2)
            MakeRiver(x + 1, y);
    }

    sub_0800ABD0(x, y);

    if (c == 1)
        MakeTile2(x, y, 1);
    else if (c == 5)
    {
        v = GetRoadTile(x, y);
        MakeTileSimple(x, y, v);
        v = sub_080016D0(x, y);
        MakeTileSimple(x, y, v);
    }
    else if (c == 3)
        MakeMountain(x, y);
    else if (c == 4)
    {
        v = sub_080016D0(x, y);
        MakeTileSimple(x, y, v);
    }
}

void MakeProperty(int x, int y, int t)
{
    int a;
    int b;
    int u;

    if ((s8)gActiveMap->propertyCount > 0x3B && GetPropertyKindAt(x, y) == 0)
        return;

    if (GetArmyHq(t, &a, &b))
    {
        ClearArmyHq(t);
        RemovePropertyAt(a, b);
        SetTerrainAt(a, b, 1);
        MakeTile2(a, b, 1);
    }

    if (GetPropertyKindAt(x, y) == 2)
        ClearArmyHq(MAP->terrain[MAP->rowOffset[y] + x]);

    u = (u16)GetDefaultTileForTerrain(t);

    if (GetPropertyKindAt(x, y))
    {
        if (MAP->terrain[MAP->rowOffset[y] + x] == t)
            return;
        RemovePropertyAt(x, y);
    }

    SetTerrainAt(x, y, t);
    MakeTile2(x, y, u);
    AddPropertyRecord(x, y, t);
    RecountArmyProperties();
    SetArmyHq(t, x, y);
    sub_0800A588(x, y);
    sub_0800ABD0(x, y);
    RepaintNeighbours(x, y);
    sub_0800BEE4(x, y);
    sub_0800EC20(x, y);
}

asm(".global sub_0800C454\n.thumb_set sub_0800C454, MakeProperty\n");
