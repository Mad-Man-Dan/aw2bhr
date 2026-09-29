#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037260.
 * sub_08037260 @ 0x08037260
 */

#include "hardware.h"

void SetDefaultDisplayState(void)
{
    u16 v;

    gDispIo.disp_ct.mode = 0;
    gDispIo.disp_ct.forced_blank = 0;

    SetDispEnable(0, 0, 0, 0, 0);
    SetWinEnable(0, 0, 0);

    gUnknown_03002B40 = 0;
    gUnknown_03002EFC = 0;
    gUnknown_03002B4C = 0;
    gUnknown_03002B44 = 0;
    gUnknown_03002B68 = 0;
    gUnknown_03002B30 = 0;
    gUnknown_030024E4 = 0;
    gUnknown_030020B8 = 0;

    gUnknown_030030A4.bits.win0_enable_bg0 = 0;
    gUnknown_030030A4.bits.win0_enable_bg1 = 0;
    gUnknown_030030A4.bits.win0_enable_bg2 = 0;
    gUnknown_030030A4.bits.win0_enable_bg3 = 0;
    gUnknown_030030A4.bits.win0_enable_obj = 0;

    gUnknown_030030A4.bits.win1_enable_bg0 = 0;
    gUnknown_030030A4.bits.win1_enable_bg1 = 0;
    gUnknown_030030A4.bits.win1_enable_bg2 = 0;
    gUnknown_030030A4.bits.win1_enable_bg3 = 0;
    gUnknown_030030A4.bits.win1_enable_obj = 0;

    gUnknown_030030DC.bits.win1_enable_bg0 = 0;
    gUnknown_030030DC.bits.win1_enable_bg1 = 0;
    gUnknown_030030DC.bits.win1_enable_bg2 = 0;
    gUnknown_030030DC.bits.win1_enable_bg3 = 0;
    gUnknown_030030DC.bits.win1_enable_obj = 0;

    gUnknown_030030DC.bits.win0_enable_bg0 = 0;
    gUnknown_030030DC.bits.win0_enable_bg1 = 0;
    gUnknown_030030DC.bits.win0_enable_bg2 = 0;
    gUnknown_030030DC.bits.win0_enable_bg3 = 0;
    gUnknown_030030DC.bits.win0_enable_obj = 0;

    *(u8 *)&gDispIo.disp_ct &= 0x38;

    gUnknown_030030E0.bits.effect = 0;
    gUnknown_03002020 = v = 0;
    gUnknown_03002B28 = v;
    gUnknown_03001FFC = v;

    gUnknown_030030E0.raw &= 0xFFE0;
    gUnknown_030030E0.raw &= 0xE0FF;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    gUnknown_030030E0.bits.target2_enable_bd = 0;
    gUnknown_030030E0.bits.effect = 0;

    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = v;
    gUnknown_03001FFC = v;

    gUnknown_03002B6C.bits.priority = 1;
    gUnknown_03002B6C.bits.priority = 2;
    gUnknown_03002B6C.bits.priority = 3;
    gUnknown_03002B6C.bits.priority = 0;

    gUnknown_03001FE8.bits.priority = 0;
    gUnknown_030030B4.bits.priority = 0;
    ((union BgCntBuf *)&gUnknown_0300251C)->bits.priority = 0;
}
asm(".global sub_08037260\n.thumb_set sub_08037260, SetDefaultDisplayState\n");
