#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804151C.
 * sub_0804151C @ 0x0804151C, sub_080415E4 @ 0x080415E4, sub_080416A4 @ 0x080416A4, sub_08041758 @ 0x08041758
 */

/* Scans the whole map for cells the IsCellCapturableByCurrentArmy predicate accepts, and writes
 * them into the gUnknown_03003F20 list as {x, y, value} triples terminated by a
 * 0xFFFF value word. Returns how many were written.
 *
 * Byte-identical twin of BuildSiloCellList apart from the predicate: the two differ
 * in exactly one instruction, the `bl` target (IsCellCapturableByCurrentArmy vs IsTerrainSilo).
 *
 * gUnknown_03003F20 is declared `struct Unk03003338 *` -- a type inherited from
 * its only writer, InitRecordListPointersAndTerrainTable. This function reads it as a 4-byte
 * {u8 x; u8 y; s16 v;} record (`strb`, `strb #1`, `strh #2`, `adds r5,#4`), so
 * the cast is deliberate; see include/unknown-globals.h. The declaration is
 * left alone rather than retyped because the writer's value genuinely is the
 * Unk03003338 record pointer -- one scratch buffer, two record layouts.
 *
 * The outer guard compares against sb (the count) rather than y because both
 * are 0 at that point and CSE shares the register -- nothing to author. */

struct Unk4151CCell
{
    /* 00 */ u8 x;
    /* 01 */ u8 y;
    /* 02 */ s16 v;
};
/* Byte-identical twin of BuildCapturableCellList -- 200 bytes, 87 instructions, the same
 * five pool words in the same order, differing in exactly ONE instruction: the
 * predicate called is IsTerrainSilo here and IsCellCapturableByCurrentArmy there. See
 * src/decomp/c_0804151C.c for the read-out of the shape.
 *
 * That one-instruction equality is also what proves IsTerrainSilo returns bool8
 * and not the `int` it was promoted as: the `lsls #0x18; lsrs #0x18` after the
 * `bl` is agbcc's re-narrowing of a narrow-returning callee, and an `int`
 * callee would not produce it. See include/unknown-functions.h. */

struct Unk41758Cell
{
    /* 00 */ u8 x;
    /* 01 */ u8 y;
    /* 02 */ s16 v;
};

int BuildCapturableCellList(void)
{
    struct Unk4151CCell *out;
    int count;
    int x;
    int y;

    count = 0;
    out = (struct Unk4151CCell *)gUnknown_03003F20;
    if ((u8)(gUnknown_030040D8->unk00 - 1) > 1)
        return 0;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] >= 0 && IsCellCapturableByCurrentArmy(x, y) == 1)
            {
                count++;
                out->x = x;
                out->y = y;
                out->v = (s8)gUnknown_03003340[y][x];
                out++;
            }
        }
    }
    out->v = 0xFFFF;
    return count;
}
asm(".global sub_0804151C\n.thumb_set sub_0804151C, BuildCapturableCellList\n");

/* Builds the gUnknown_03003338 list from every map cell whose unit id passes
 * CanTransportCarryUnitId against gUnknown_03003F38, terminates it with a zero unk00, and
 * returns the element count as a pointer difference.
 *
 * The splitter's `gUnknown_08091310` is NOT an object: the ROM word at
 * 0x08091310 holds 0x03003338, so it is agbcc's own -fforce-addr word for
 * gUnknown_03003338 (its neighbour 0x08091314 holds the same address again --
 * the documented one-word-per-(function, symbol) run). Naming the global
 * honestly reproduces the ROM's three-level `ldr rN,=<word>; ldr rM,[rN];
 * ldr rK,[rM]` chain, including the reload at the end for the pointer
 * subtraction.
 *
 * Cell addressing now uses the canonical gMap unit and rowOffset fields
 * directly.
 *
 * NOT a twin of BuildResupplyTargetList despite the adjacency and the similar size. */
int BuildBoardableTransportList(void)
{
    struct Unk03003338 *out;
    int off;
    u8 cell;
    int x;
    int y;

    out = gUnknown_03003338;
    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] >= 0)
            {
                off = gMap->rowOffset[y] + x;
                cell = gMap->unit[off];
                if (cell != 0 && CanTransportCarryUnitId(cell, gUnknown_03003F38))
                {
                    out->unk00 = cell;
                    out++;
                }
            }
        }
    }
    out->unk00 = 0;
    return out - gUnknown_03003338;
}
asm(".global sub_080415E4\n.thumb_set sub_080415E4, BuildBoardableTransportList\n");

/* The BuildBoardableTransportList shape with a different predicate and a different order: here
 * IsResupplyableAllyAt is asked BEFORE the map cell is addressed, and the cell is read
 * only to be stored, so there is no `cell` local and no `!= 0` test.
 * Not a twin of BuildBoardableTransportList -- 81 instructions against 84, four pool words
 * against five.
 *
 * `gUnknown_08091314` is not an object either: the ROM word there holds
 * 0x03003338, the second of the two consecutive -fforce-addr words for
 * gUnknown_03003338 (0x08091310 is BuildBoardableTransportList's). One word per
 * (function, symbol), as documented in include/unknown-globals.h. */
int BuildResupplyTargetList(void)
{
    struct Unk03003338 *out;
    int off;
    int x;
    int y;

    out = gUnknown_03003338;
    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] >= 0 && IsResupplyableAllyAt(x, y))
            {
                off = gMap->rowOffset[y] + x;
                out->unk00 = gMap->unit[off];
                out++;
            }
        }
    }
    out->unk00 = 0;
    return out - gUnknown_03003338;
}
asm(".global sub_080416A4\n.thumb_set sub_080416A4, BuildResupplyTargetList\n");

int BuildSiloCellList(void)
{
    struct Unk41758Cell *out;
    int count;
    int x;
    int y;

    count = 0;
    out = (struct Unk41758Cell *)gUnknown_03003F20;
    if ((u8)(gUnknown_030040D8->unk00 - 1) > 1)
        return 0;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] >= 0 && IsTerrainSilo(x, y) == 1)
            {
                count++;
                out->x = x;
                out->y = y;
                out->v = (s8)gUnknown_03003340[y][x];
                out++;
            }
        }
    }
    out->v = 0xFFFF;
    return count;
}
asm(".global sub_08041758\n.thumb_set sub_08041758, BuildSiloCellList\n");
