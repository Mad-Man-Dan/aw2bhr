#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A7C4.
 * sub_0802A7C4 @ 0x0802A7C4
 */

#include "hardware.h"

/* ELEVEN separate bitfield assignments, not an OR of one mask. The ROM's
 * single `ldrb`, six `orrs` against six separately materialised constants and
 * single `strb` per byte is what agbcc makes of consecutive bitfield stores to
 * the same byte: CSE forwards the loaded value and kills the dead
 * intermediate stores, and the five one-bit masks stay live in r6/r5/r4/r3/r1
 * because each is used again in the second byte. A single
 * `gUnknown_030030E0.raw |= 0xdf` would emit one `ldrh`, one constant and one
 * `strh`. */
void RefreshMapCursorInfoPanel(void)
{
    if (gPlaySt.dispMiniPanel == 0)
        return;

    RunOrQueueDrawCallback((void *)sub_0802AA78, 1);
    SetDefaultColorEffects();

    gUnknown_030030E0.bits.effect = 3;
    gUnknown_030030E0.bits.target1_enable_bg0 = 1;
    gUnknown_030030E0.bits.target1_enable_bg1 = 1;
    gUnknown_030030E0.bits.target1_enable_bg2 = 1;
    gUnknown_030030E0.bits.target1_enable_bg3 = 1;
    gUnknown_030030E0.bits.target1_enable_obj = 1;
    gUnknown_030030E0.bits.target2_enable_bg0 = 1;
    gUnknown_030030E0.bits.target2_enable_bg1 = 1;
    gUnknown_030030E0.bits.target2_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;

    gUnknown_03001FFC = 5;
    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = 0;
}
asm(".global sub_0802A7C4\n.thumb_set sub_0802A7C4, RefreshMapCursorInfoPanel\n");
