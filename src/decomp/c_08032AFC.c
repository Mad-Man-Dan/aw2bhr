#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032AFC.
 * sub_08032AFC @ 0x08032AFC
 */

#include "hardware.h"

/* Fixed-geometry window 0 setup (0x5E..0xDC by 0x20..0x80), the constant-edge
 * relative of SoundRoomSetTitleTextWindow (src/decomp/c_0806EA28.c) and SetCreditsWindow0Rect. The
 * 0x20 that DISPCNT's win0 bit needs is CSEd into r5 and reused both as the
 * top edge's value and as the second shadow's blend bit. */
void LinkMapPick_SetupWindow(void)
{
    gDispIo.disp_ct.win0_enable = 1;

    gUnknown_03002B40 = 0x5E;
    gUnknown_03002B4C = 0xDC;
    gUnknown_03002EFC = 0x20;
    gUnknown_03002B44 = 0x80;

    gUnknown_030030A4.bits.win0_enable_bg0 = 1;
    gUnknown_030030A4.bits.win0_enable_bg1 = 1;
    gUnknown_030030A4.bits.win0_enable_bg2 = 1;
    gUnknown_030030A4.bits.win0_enable_bg3 = 1;
    gUnknown_030030A4.bits.win0_enable_obj = 1;
    gUnknown_030030A4.bits.win0_enable_blend = 0;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 0;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
}
asm(".global sub_08032AFC\n.thumb_set sub_08032AFC, LinkMapPick_SetupWindow\n");
