#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DE1C.
 * sub_0802DE1C @ 0x0802DE1C, sub_0802DEFC @ 0x0802DEFC
 */

#include "hardware.h"

void sub_0802DE1C(void)
{
    int off;
    int v;

    HandleMoveMapCursor();
    sub_080236E8();
    HandleMoveCameraWithMapCursor(4);

    off = gMap->rowOffset[gUnknown_030033E4.unk02] + gUnknown_030033E4.unk00;

    if (gMap->move[off] < 0)
        sub_08023274(1);
    else
        sub_08023274(1);

    sub_08039264();

    if (!sub_0802DBF8())
        return;

    v = gpKeySt->pressed & 1;

    if (v != 0)
    {
        sub_0802E60C(((union Unk802C57CBuf *)&gUnknown_030033E4)->spos.unk00,
            ((union Unk802C57CBuf *)&gUnknown_030033E4)->spos.unk02);
        return;
    }

    if (!(gpKeySt->pressed & 2))
        return;

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0x11, gUnknown_03003F38, 0, 0);

    ScrollCameraToKeepCellInView(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03);
    sub_08035810();
    RebuildMapUnitLayers();
    HideRangeOverlay();
    gUnknown_03003334 = v;
    sub_0803B4DC(0x66);
}

void sub_0802DEFC(void)
{
    int off;
    int id;
    struct Unit *e;

    HandleMoveMapCursor();
    HandleGameMapCursorInput();
    HandleMoveCameraWithMapCursor(4);
    sub_08023274(5);

    if (!sub_0802DBF8())
        return;

    if (gpKeySt->pressed & 2)
    {
        sub_0803B4DC(0x66);
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
