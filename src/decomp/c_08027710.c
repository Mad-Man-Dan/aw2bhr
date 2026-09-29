#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027710.
 * sub_08027710 @ 0x08027710, sub_0802776C @ 0x0802776C, sub_080277BC @ 0x080277BC
 */

void PlaceInfoBoxAwayFromCursor(void)
{
    u16 x;
    u16 y;

    x = gUnknown_030033E4.unk00 * 16 - gMap->scrollX;
    y = gUnknown_030033E4.unk02 * 16 - gMap->scrollY;

    if ((s16)y <= 0x4f)
    {
        if ((s16)x <= 0x7f)
            SnapInfoBoxToSide1();
        else
            SnapInfoBoxToSide0();
    }
    else
    {
        if (gUnknown_03003130.unk00 == 0)
            SnapInfoBoxToSide0();
        else
            SnapInfoBoxToSide1();
    }
}
asm(".global sub_08027710\n.thumb_set sub_08027710, PlaceInfoBoxAwayFromCursor\n");

void SetInfoBoxMode(u8 a1)
{
    switch (a1)
    {
    case 0:
        RunOrQueueDrawCallback(DrawInfoBoxCombobox, 0);
        break;

    case 1:
        RunOrQueueDrawCallback(SnapInfoBoxToSide0, 0);
        break;

    case 2:
        RunOrQueueDrawCallback(SnapInfoBoxToSide1, 0);
        break;

    case 3:
        RunOrQueueDrawCallback(PlaceInfoBoxAwayFromCursor, 0);
        break;
    }
}
asm(".global sub_0802776C\n.thumb_set sub_0802776C, SetInfoBoxMode\n");

void sub_080277BC(void)
{
    gUnknown_03001470[gUnknown_03001FBC].unk24++;

    SetSlotSpriteScaleX(gUnknown_03001FBC, gUnknown_08090AA8[gUnknown_03001470[gUnknown_03001FBC].unk24]);
    SetSlotSpriteScaleY(gUnknown_03001FBC, gUnknown_08090AA8[gUnknown_03001470[gUnknown_03001FBC].unk24]);

    if (gUnknown_08090AA8[gUnknown_03001470[gUnknown_03001FBC].unk24] == 0x100)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
