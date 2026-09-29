#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E4B4.
 * sub_0802E4B4 @ 0x0802E4B4
 */

/* The cell fetch is gMap->unit[gMap->rowOffset[sy] + sx], read twice.
 *
 * The whole address chain is recomputed after the CanBuildAtCell call because
 * the call clobbers memory; only the two s16 casts survive as common
 * subexpressions, which is why sx/sy read as locals.
 *
 * gUnknown_030040D8 is the same object as gUnits[i] -- see the note
 * on struct Unk030040D8 in unknown-globals.h for why the cast is here rather
 * than in the global's type. */
void MapCursor_OnPressA(s16 x, s16 y)
{
    u8 *sel;
    int idx;
    s16 sx;
    s16 sy;

    InitTextTileCache(0);
    gUnknown_030040DC = 0;
    gUnknown_030033E8[0] = 0;
    gUnknown_030033E8[1] = 0;
    gUnknown_03000558 = 0;

    sel = &gUnknown_03003F38;
    sx = x;
    sy = y;
    idx = gMap->rowOffset[sy] + sx;
    gUnknown_03003F38 = gMap->unit[idx];
    gUnknown_030040D8 = (struct Unk030040D8 *)&gUnits[*sel];

    if (CanBuildAtCell(sx, sy))
    {
        OpenDeploymentScreen(sx, sy);
        return;
    }

    idx = gMap->rowOffset[sy] + sx;
    if (gMap->unit[idx] == 0 || (gUnknown_030040D8->unk01 & 1))
    {
        OpenMapMenu();
        return;
    }

    RunMapEventsOnUnitSelected(gUnknown_030040D8);
    gUnknown_03003110[0] = 4;
    CreateMoveSlideForActiveUnit(gUnknown_030040D8);
    SetMapLayersRangeBlend();
    RebuildMapUnitLayers();
    gUnknown_03004480 = (*sel >> 6) + 1;
    SetWorkingMapPlane(gMap->move);
    GenerateUnitMovementMap(gUnknown_030040D8);
    gUnknown_03004480 = gUnknown_030033EC;
    ShowRangeOverlay((u16)sx, (u16)sy, 0);
    gUnknown_03003334 = 1;
    InitMovePathForActiveUnit();
    PlayMusicOrSfx2(0x69);
}
asm(".global sub_0802E4B4\n.thumb_set sub_0802E4B4, MapCursor_OnPressA\n");
