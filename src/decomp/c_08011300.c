#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011300.
 * sub_08011300 @ 0x08011300
 */

#include "hardware.h"

/* Five consecutive 1-bit clears on gUnknown_030030A4 give the `mov/neg`,
 * `mov/neg`, then bare `subs` chain described in docs/agbcc-codegen.md; the
 * six sets on gUnknown_030030DC give one `orr` apiece, and the last of them
 * reuses the `movs r3, #0x20` set up for gDispIo's win0_enable at the top.
 * Both shadows are written at byte 0, which is the win0_* group of
 * struct WinCnt. */
void SetupWipeWindow(void)
{
    gDispIo.disp_ct.win0_enable = 1;

    gUnknown_030030A4.bits.win0_enable_bg0 = 0;
    gUnknown_030030A4.bits.win0_enable_bg1 = 0;
    gUnknown_030030A4.bits.win0_enable_bg2 = 0;
    gUnknown_030030A4.bits.win0_enable_bg3 = 0;
    gUnknown_030030A4.bits.win0_enable_obj = 0;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
}
asm(".global sub_08011300\n.thumb_set sub_08011300, SetupWipeWindow\n");
