#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CEB0.
 * sub_0802CEB0 @ 0x0802CEB0, sub_0802CEFC @ 0x0802CEFC
 */

void MapMenu_Power(void)
{
    u8 *p;

    CloseTopMenu();

    p = gUnknown_030044B0;
    *(u32 *)(p + 8) = gUnknown_03001FD4;

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0xf, 0, gUnknown_030033EC, 0);

    PayForCoPower(gUnknown_030033EC, 1);
    RebuildMapUnitLayers2();
}
asm(".global sub_0802CEB0\n.thumb_set sub_0802CEB0, MapMenu_Power\n");

void MapMenu_SuperPower(void)
{
    u8 *p;

    CloseTopMenu();

    p = gUnknown_030044B0;
    *(u32 *)(p + 8) = gUnknown_03001FD4;

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0x10, 0, gUnknown_030033EC, 0);

    PayForCoPower(gUnknown_030033EC, 2);
    RebuildMapUnitLayers2();
}
asm(".global sub_0802CEFC\n.thumb_set sub_0802CEFC, MapMenu_SuperPower\n");
