#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010B34.
 * sub_08010B34 @ 0x08010B34, MakePipe @ 0x08010D28, MakeSeam @ 0x08010D80, sub_08010DD4 @ 0x08010DD4
 */

/*
 * sub_08010B34 -- the end-piece tile for a cell that joins in one direction
 * only; -1 for anything else.
 *
 * Nothing happens unless the tile at (x, y) is one of the fourteen joining ids.
 * A cell showing 0x162 or 0x163 is rebuilt first: if sub_0800F8D4 accepts it the
 * function gives up with -1, otherwise RemovePropertyAt clears the cell, the terrain
 * becomes 0xF and GetPipeTile supplies a fresh tile.
 *
 * Each direction is then asked whether GetPipeConnectionAt reports 2 for it. The first
 * one that does wins, but only if none of the other three does as well:
 * direction 0 gives tile 0x121, 1 gives 0x120, 2 gives 0x103 and 3 gives 0x102.
 * A cell that joins in no direction, or in more than one, gives -1.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The fourteen-way `||` chain must keep this order. The compiler folds only
 *     the first adjacent pair, 0x142 and 0x143, into a single range test; from
 *     the third term on the left-hand side is already a compound test and no
 *     further pair is folded, so it is the order and not the set of values that
 *     matters.
 *   - The cell is read twice, the second time as a fresh expression rather than
 *     through `t`. The original reloads it, because the fourteen tests join
 *     control flow and the compiler carries no value across that join.
 */
int sub_08010B34(int x, int y)
{
    u16 t;
    u16 u;

    t = gMap->tile[
            gMap->rowOffset[y] + x];
    if (t == 0x142 || t == 0x143 || t == 0x140 || t == 0x141 || t == 0x160
        || t == 0x161 || t == 0x162 || t == 0x163 || t == 0x122 || t == 0x123
        || t == 0x121 || t == 0x120 || t == 0x103 || t == 0x102)
    {
        u = gMap->tile[
                gMap->rowOffset[y] + x];
        if (u == 0x162 || u == 0x163)
        {
            if (sub_0800F8D4(x, y))
                return -1;
            RemovePropertyAt(x, y);
            SetTerrainAt(x, y, 0xf);
            MakeTileSimple(x, y, GetPipeTile(x, y, 1));
        }
        if (GetPipeConnectionAt(x, y, 0) == 2)
        {
            if (GetPipeConnectionAt(x, y, 1) == 2)
                return -1;
            if (GetPipeConnectionAt(x, y, 2) == 2)
                return -1;
            if (GetPipeConnectionAt(x, y, 3) == 2)
                return -1;
            return 0x121;
        }
        if (GetPipeConnectionAt(x, y, 1) == 2)
        {
            if (GetPipeConnectionAt(x, y, 0) == 2)
                return -1;
            if (GetPipeConnectionAt(x, y, 2) == 2)
                return -1;
            if (GetPipeConnectionAt(x, y, 3) == 2)
                return -1;
            return 0x120;
        }
        if (GetPipeConnectionAt(x, y, 2) == 2)
        {
            if (GetPipeConnectionAt(x, y, 0) == 2)
                return -1;
            if (GetPipeConnectionAt(x, y, 1) == 2)
                return -1;
            if (GetPipeConnectionAt(x, y, 3) == 2)
                return -1;
            return 0x103;
        }
        if (GetPipeConnectionAt(x, y, 3) == 2)
        {
            if (GetPipeConnectionAt(x, y, 0) == 2)
                return -1;
            if (GetPipeConnectionAt(x, y, 1) == 2)
                return -1;
            if (GetPipeConnectionAt(x, y, 2) == 2)
                return -1;
            return 0x102;
        }
    }
    return -1;
}

/*
 * MakePipe -- lay a pipe at (x, y) and refresh everything around it.
 *
 * The terrain becomes 0xF and the tile comes from GetPipeTile. The six calls
 * that follow all take the same cell and redraw its surroundings; sub_0800A588
 * and RepaintNeighbours are the neighbour sweeps in c_0800A588.c and c_08007F9C.c.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - GetPipeTile's call stays nested inside MakeTileSimple. The original
 *     moves the result into the argument register and reloads the other two, so
 *     the nesting costs exactly what a temporary would.
 */
void MakePipe(int x, int y)
{
    SetTerrainAt(x, y, 0xf);
    MakeTileSimple(x, y, GetPipeTile(x, y, 1));
    RepaintPipesAround(x, y);
    sub_0800A588(x, y);
    sub_0800ABD0(x, y);
    RepaintNeighbours(x, y);
    sub_0800BEE4(x, y);
    sub_0800EC20(x, y);
}

asm(".global sub_08010D28\n.thumb_set sub_08010D28, MakePipe\n");

/*
 * MakeSeam -- lay a seam at (x, y), if the map may have another property.
 *
 * Nothing happens unless gActiveMap->propertyCount is 0x3B or less, or
 * GetPropertyKindAt already reports something for the cell. The terrain becomes
 * 0x10, GetSeamType gives the tile, which goes both to MakeTileSimple and to
 * AddPropertyRecord, and the per-army property totals are counted again.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - propertyCount is a u8 read through an `(s8)` cast, so the comparison is
 *     signed and a count of 0x80 or more also passes. Keep the cast and leave
 *     the field's type alone.
 */
void MakeSeam(int x, int y)
{
    int t;

    if ((s8)gActiveMap->propertyCount <= 0x3b || GetPropertyKindAt(x, y) != 0)
    {
        SetTerrainAt(x, y, 0x10);
        t = GetSeamType(x, y);
        MakeTileSimple(x, y, t);
        AddPropertyRecord(x, y, t);
        RecountArmyProperties();
    }
}

asm(".global sub_08010D80\n.thumb_set sub_08010D80, MakeSeam\n");

/*
 * sub_08010DD4 -- does the bridge at (x, y) meet a seam on either side?
 * 1 means yes.
 *
 * A horizontal bridge (tile 0x142) is tested against the cells left and right of
 * it, a vertical one (0x143) against the cells above and below, and any other
 * tile is accepted at once. A neighbour counts when its tile is 0x162 or 0x163.
 * Running off the edge of the map on the second side answers 0.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `return 0;` is the function's last statement and the accepting case is an
 *     explicit `else return 1;`. Written the other way round the compiler emits
 *     the same instructions with the two tail blocks swapped. What decides it is
 *     which return falls through at the end of the function, not the polarity of
 *     the last `if`.
 *   - The neighbour indexes stay written inline. Binding the adjusted coordinate
 *     to a local of its own stops the compiler regrouping the index and costs a
 *     multiply.
 */
int sub_08010DD4(int x, int y)
{
    u16 t;
    u16 u;

    t = gMap->tile[
            gMap->rowOffset[y] + x];
    if (t == 0x142)
    {
        if (x - 1 >= 0)
        {
            u = gMap->tile[
                    gMap->rowOffset[y] + (x - 1)];
            if (u == 0x162 || u == 0x163)
                return 1;
        }
        if (x + 1 >= gMap->width)
            return 0;
        u = gMap->tile[
                gMap->rowOffset[y] + (x + 1)];
        if (u == 0x162 || u == 0x163)
            return 1;
    }
    else if (t == 0x143)
    {
        if (y - 1 >= 0)
        {
            u = gMap->tile[
                    gMap->rowOffset[y - 1] + x];
            if (u == 0x162 || u == 0x163)
                return 1;
        }
        if (y + 1 >= gMap->height)
            return 0;
        u = gMap->tile[
                gMap->rowOffset[y + 1] + x];
        if (u == 0x162 || u == 0x163)
            return 1;
    }
    else
    {
        return 1;
    }
    return 0;
}
