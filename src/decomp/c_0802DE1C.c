#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DE1C.
 * sub_0802DE1C @ 0x0802DE1C, sub_0802DEFC @ 0x0802DEFC
 */

#include "hardware.h"

void MapCursorState_ChooseDestination(void)
{
    int off;
    int v;

    HandleMoveMapCursor();
    HandleMoveMapCursorInMoveRange();
    HandleMoveCameraWithMapCursor(4);

    off = gMap->rowOffset[gUnknown_030033E4.unk02] + gUnknown_030033E4.unk00;

    if (gMap->move[off] < 0)
        StepMapCursorAndDraw(1);
    else
        StepMapCursorAndDraw(1);

    UpdateMovePathAndQueueDraw();

    if (!IsMapCursorSettled())
        return;

    v = gpKeySt->pressed & 1;

    if (v != 0)
    {
        ConfirmUnitDestination(((union Unk802C57CBuf *)&gUnknown_030033E4)->spos.unk00,
            ((union Unk802C57CBuf *)&gUnknown_030033E4)->spos.unk02);
        return;
    }

    if (!(gpKeySt->pressed & 2))
        return;

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0x11, gUnknown_03003F38, 0, 0);

    ScrollCameraToKeepCellInView(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03);
    EndActiveMoveSlide();
    RebuildMapUnitLayers();
    HideRangeOverlay();
    gUnknown_03003334 = v;
    PlayMusicOrSfx2(0x66);
}
asm(".global sub_0802DE1C\n.thumb_set sub_0802DE1C, MapCursorState_ChooseDestination\n");

void MapCursorState_DeleteUnit(void)
{
    int off;
    int id;
    struct Unit *e;

    HandleMoveMapCursor();
    HandleGameMapCursorInput();
    HandleMoveCameraWithMapCursor(4);
    StepMapCursorAndDraw(5);

    if (!IsMapCursorSettled())
        return;

    if (gpKeySt->pressed & 2)
    {
        PlayMusicOrSfx2(0x66);
        gUnknown_03003334 = 0;
        return;
    }

    off = gMap->rowOffset[gUnknown_030033E4.unk02] + gUnknown_030033E4.unk00;
    id = gMap->unit[off];

    if (id == 0)
        return;

    if (((u32)id >> 6) + 1 != gUnknown_030033EC)
        return;

    e = &gUnits[id];

    if (e->flags & 1)
        return;

    if (!(gpKeySt->pressed & 1))
        return;

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0x12, id, 0, 0);

    StartUnitDestroyed(e);
}
asm(".global sub_0802DEFC\n.thumb_set sub_0802DEFC, MapCursorState_DeleteUnit\n");
