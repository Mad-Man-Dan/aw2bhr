#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DCB4.
 * sub_0802DCB4 @ 0x0802DCB4
 */

#include "hardware.h"

void sub_0802DCB4(void)
{
    struct Unit *unit;

    HandleMoveMapCursor();
    HandleGameMapCursorInput();
    HandleMoveCameraWithMapCursor(4);
    sub_08023274(0);

    if (sub_0802DBF8())
    {
        if (gpKeySt->pressed & 8)
        {
            sub_0802E250();
            return;
        }

        if (gpKeySt->pressed & 4)
        {
            sub_0801B780(0);
            sub_0802D458();
            return;
        }

        if (gpKeySt->pressed & R_BUTTON)
        {
            if (gMap->unit[
                    gMap->rowOffset[gUnknown_030033E4.unk02]
                    + gUnknown_030033E4.unk00] != 0)
            {
                sub_0801B780(0);
                sub_0803A8F0(&gUnits[
                    gMap->unit[
                        gMap->rowOffset[gUnknown_030033E4.unk02]
                        + gUnknown_030033E4.unk00]]);
                return;
            }

            sub_0801B780(0);
            sub_080470F8(sub_0803EED4(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02));
            return;
        }

        if (gpKeySt->pressed & L_BUTTON)
        {
            PeekNextReadyUnit();
            unit = GetNextReadyUnit();

            if (unit != NULL)
            {
                ScrollCameraToKeepCellInView(unit->x, unit->y);

                if (sub_08015BD0((s32)gUnknown_0849A00C) != -1)
                {
                    sub_0802DCA4();
                    return;
                }
            }
        }

        if (gpKeySt->pressed & 1)
        {
            sub_0802E4B4(((struct Unk802C57CS *)&gUnknown_030033E4)->unk00,
                         ((struct Unk802C57CS *)&gUnknown_030033E4)->unk02);
            return;
        }

        if (gpKeySt->pressed & 2)
        {
            if (sub_0802E2D0(((struct Unk802C57CS *)&gUnknown_030033E4)->unk00,
                             ((struct Unk802C57CS *)&gUnknown_030033E4)->unk02))
                return;
        }
    }

    RefreshMapCursorInfoPanel();
    sub_0802776C(0);
}
