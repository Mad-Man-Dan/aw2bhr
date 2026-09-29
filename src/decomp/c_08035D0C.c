#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035D0C.
 * sub_08035D0C @ 0x08035D0C
 */

#include "proc.h"
/* Picks the help/message id for whatever the proc's cursor is standing on:
 * two overriding predicates first, then a table on the low five bits of the
 * gUnknown_08499590 terrain byte at the same `rowOffset[y] + x` key
 * IsMoveSlideOnRiver uses (same `(v + 8) / 16` pixel-to-cell divide).
 *
 * The 0x2d arm of the switch and the IsMoveSlideOnRiver early return are ONE block in
 * the ROM -- cross-jumping merged them, which is why the jump table's entries
 * for 2, 7 and 19 point back above the switch.
 *
 * There is NO `struct ... *proc = procArg;` local here, and that is 2 bytes:
 * the ROM keeps the incoming pointer in r4 alone (`push {r4, lr}`), while a
 * binding local makes a second pseudo that gets copied into r5 for the second
 * member read and pushes r5 as well.  The cast at each use is free. */
struct Unk35D0CProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x42);
    /* 0x42 */ s16 unk42;
    /* 0x44 */ s16 unk44;
};

u16 GetMoveSlideFootstepSfx(ProcPtr proc)
{
    struct Map *map;

    if (IsMoveSlideOnRiver(proc))
        return 0x2d;

    if (IsWeatherSnowing(proc))
        return 0x43;

    map = gMap;

    switch (map->terrain[map->rowOffset[
                (((struct Unk35D0CProc *)proc)->unk44 + 8) / 16]
                       + (((struct Unk35D0CProc *)proc)->unk42 + 8) / 16] & 0x1f)
    {
    case TERRAIN_RIVER:
    case TERRAIN_SEA:
    case TERRAIN_REEF:
        return 0x2d;
    case TERRAIN_MOUNTAIN:
        return 0x4b;
    case TERRAIN_WOOD:
    case TERRAIN_SHOAL:
        return 0x47;
    case TERRAIN_PLAIN:
    case TERRAIN_ROAD:
    case TERRAIN_CITY:
    case TERRAIN_HQ:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BRIDGE:
    case TERRAIN_BASE:
        return 0x29;
    }

    return 0;
}
asm(".global sub_08035D0C\n.thumb_set sub_08035D0C, GetMoveSlideFootstepSfx\n");
