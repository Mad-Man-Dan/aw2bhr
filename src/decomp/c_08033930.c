#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033930.
 * sub_08033930 @ 0x08033930
 */

#include "hardware.h"

/* LinkScreenSetupMessageWindow @ 0x08033930, 128 bytes, THUMB.
 * Signature below is DECLARED in include/unknown-functions.h -- it is authoritative.
 * The compiler sees that header too, so a definition that
 * disagrees will not compile.
 *
 * Same shape as SetCreditsWindow1Rect's window-shadow writes but with constant
 * byte payloads and a plain SetWinEnable(0, 1, 0) tail instead of a
 * bare win1_enable OR -- only win0_enable/win1_enable/objwin_enable
 * are touched at gDispIo, bg0_enable is untouched here.
 */
void LinkScreenSetupMessageWindow(void)
{
    gUnknown_030030A4.bits.win1_enable_bg0 = 1;
    gUnknown_030030A4.bits.win1_enable_bg1 = 0;
    gUnknown_030030A4.bits.win1_enable_bg2 = 0;
    gUnknown_030030A4.bits.win1_enable_bg3 = 0;
    gUnknown_030030A4.bits.win1_enable_obj = 0;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_03002B68 = 0;
    gUnknown_03002B30 = 0x90;
    gUnknown_030024E4 = 0xf0;
    gUnknown_030020B8 = 0xa0;

    SetWinEnable(0, 1, 0);
}
asm(".global sub_08033930\n.thumb_set sub_08033930, LinkScreenSetupMessageWindow\n");
