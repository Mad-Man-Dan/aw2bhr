#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B9CC.
 * sub_0806B9CC @ 0x0806B9CC
 */

#include "hardware.h"

/* SetCreditsWindow1Rect @ 0x0806B9CC, 160 bytes, THUMB. No prototype existed;
 * declared in include/unknown-functions.h. Sets the win1_* group on
 * gUnknown_030030A4 and the win0_* group on gUnknown_030030DC (same
 * shape as LinkScreenSetupMessageWindow's shadow writes, mirrored bg0), stores the
 * four byte parameters, then sets win1_enable_blend on both shadows
 * and gDispIo.disp_ct.win1_enable.
 */
void SetCreditsWindow1Rect(int a1, int a2, int a3, int a4)
{
    gUnknown_030030A4.bits.win1_enable_bg0 = 0;
    gUnknown_030030A4.bits.win1_enable_bg1 = 1;
    gUnknown_030030A4.bits.win1_enable_bg2 = 1;
    gUnknown_030030A4.bits.win1_enable_bg3 = 0;
    gUnknown_030030A4.bits.win1_enable_obj = 1;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 0;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_03002B68 = a1;
    gUnknown_03002B30 = a2;
    gUnknown_030024E4 = a3;
    gUnknown_030020B8 = a4;

    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;

    gDispIo.disp_ct.win1_enable = 1;
}
asm(".global sub_0806B9CC\n.thumb_set sub_0806B9CC, SetCreditsWindow1Rect\n");
