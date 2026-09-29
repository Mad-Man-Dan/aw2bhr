#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DCB4.
 * sub_0802DCB4 @ 0x0802DCB4
 */

#include "hardware.h"

void MapCursorIdle(void)
{
    struct Unit *unit;

    HandleMoveMapCursor();
    HandleGameMapCursorInput();
    HandleMoveCameraWithMapCursor(4);
    StepMapCursorAndDraw(0);

    if (IsMapCursorSettled())
    {
        if (gpKeySt->pressed & 8)
        {
            MapCursor_OnPressStart();
            return;
        }

        if (gpKeySt->pressed & 4)
        {
            InitTextTileCache(0);
            OpenMapMenu();
            return;
        }

        if (gpKeySt->pressed & R_BUTTON)
        {
            if (gMap->unit[
                    gMap->rowOffset[gUnknown_030033E4.unk02]
                    + gUnknown_030033E4.unk00] != 0)
            {
                InitTextTileCache(0);
                ShowUnitClassInfoWindow(&gUnits[
                    gMap->unit[
                        gMap->rowOffset[gUnknown_030033E4.unk02]
                        + gUnknown_030033E4.unk00]]);
                return;
            }

            InitTextTileCache(0);
            ShowTerrainInfoWindow(GetTerrainTypeAt(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02));
            return;
        }

        if (gpKeySt->pressed & L_BUTTON)
        {
            PeekNextReadyUnit();
            unit = GetNextReadyUnit();

            if (unit != NULL)
            {
                ScrollCameraToKeepCellInView(unit->x, unit->y);

                if (FindSlotScript((s32)gUnknown_0849A00C) != -1)
                {
                    ResetDisplayEffects();
                    return;
                }
            }
        }

        if (gpKeySt->pressed & 1)
        {
            MapCursor_OnPressA(((struct Unk802C57CS *)&gUnknown_030033E4)->unk00,
                         ((struct Unk802C57CS *)&gUnknown_030033E4)->unk02);
            return;
        }

        if (gpKeySt->pressed & 2)
        {
            if (MapCursor_OnPressB(((struct Unk802C57CS *)&gUnknown_030033E4)->unk00,
                             ((struct Unk802C57CS *)&gUnknown_030033E4)->unk02))
                return;
        }
    }

    RefreshMapCursorInfoPanel();
    SetInfoBoxMode(0);
}
asm(".global sub_0802DCB4\n.thumb_set sub_0802DCB4, MapCursorIdle\n");
