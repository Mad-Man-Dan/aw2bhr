#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A8F0.
 * sub_0803A8F0 @ 0x0803A8F0
 */

void ShowUnitClassInfoWindow(struct Unit *a)
{
    RebuildMapUnitLayers2();
    gUnknown_03001418 = 0;
    gUnknown_03001FF8 = 0;
    sub_080152EC(gUnknown_0849E2C8, 0);
    sub_080152EC(gUnknown_0849E240, 0);
    if (a->x - gMap->scrollX / 16 <= 6)
    {
        gUnknown_0849D89C->unk00 = 0x80;
        gUnknown_0849D89C->unk02 = 0;
    }
    else
    {
        gUnknown_0849D89C->unk00 = 0;
        gUnknown_0849D89C->unk02 = 0xe;
    }
    if (a->y - gMap->scrollY / 16 <= 4)
        gUnknown_0849D89C->unk03 = 0xa;
    else
        gUnknown_0849D89C->unk03 = 0;
    gUnknown_0849D89C->unk04 = a;
    gUnknown_0849D89C->unk08 = ((a - gUnits) >> 6) + 1;
    PlayMusicOrSfx2(0x65);
}
asm(".global sub_0803A8F0\n.thumb_set sub_0803A8F0, ShowUnitClassInfoWindow\n");
