#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806BA6C.
 * sub_0806BA6C @ 0x0806BA6C
 */

#include "hardware.h"

/* Window 0 setup taking its four edges as arguments -- the parameterised
 * sibling of SoundRoomSetTitleTextWindow (src/decomp/c_0806EA28.c). Same two window shadows,
 * same accumulate-in-a-register behaviour; it differs in which layer bit the
 * second shadow masks out (bg3 here, bg0 there) and in DISPCNT coming last. */
void SetCreditsWindow0Rect(int left, int top, int right, int bottom)
{
    gUnknown_030030A4.bits.win0_enable_bg0 = 1;
    gUnknown_030030A4.bits.win0_enable_bg1 = 1;
    gUnknown_030030A4.bits.win0_enable_bg2 = 1;
    gUnknown_030030A4.bits.win0_enable_bg3 = 1;
    gUnknown_030030A4.bits.win0_enable_obj = 1;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 0;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_03002B40 = left;
    gUnknown_03002EFC = top;
    gUnknown_03002B4C = right;
    gUnknown_03002B44 = bottom;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;
    gDispIo.disp_ct.win0_enable = 1;
}
asm(".global sub_0806BA6C\n.thumb_set sub_0806BA6C, SetCreditsWindow0Rect\n");
