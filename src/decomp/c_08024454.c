#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08024454.
 * sub_08024454 @ 0x08024454
 */

#include "hardware.h"

void SetMapLayersRangeBlend(void)
{
    gUnknown_030030B4.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_03002B6C.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;

    SetDefaultColorEffects();

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg0 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;

    if (gPlaySt.weather == 1)
    {
        gUnknown_03002020 = 10;
        gUnknown_03002B28 = 8;
    }
    else
    {
        gUnknown_03002020 = 6;
        gUnknown_03002B28 = 16;
    }

    ResetWindowShadows();
}
asm(".global sub_08024454\n.thumb_set sub_08024454, SetMapLayersRangeBlend\n");
