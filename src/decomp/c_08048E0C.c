#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048E0C.
 * sub_08048E0C @ 0x08048E0C
 */

#include "hardware.h"

/* Same family as sub_080039E4/sub_08003A80 but with the two library calls
 * split around the window setup: ResetWindowShadows first, the gDispIo/030030A4/
 * 030030DC block, then SetDefaultColorEffects and only afterwards the blend groups.
 *
 * No raw-halfword write and no zero anywhere in the function -- every constant
 * that appears twice (2, 4, 8, 0x10, 0x20) is materialised once and held, and
 * `gUnknown_03002B40 = 2;` is what creates the `2` the blend group later
 * reuses, which is why that store cannot move. */
void sub_08048E0C(void)
{
    ResetWindowShadows();

    gDispIo.disp_ct.win0_enable = 1;

    gUnknown_03002B40 = 2;
    gUnknown_03002EFC = 0x72;
    gUnknown_03002B4C = 0xEF;
    gUnknown_03002B44 = 0xA0;

    gUnknown_030030A4.bits.win0_enable_bg0 = 1;
    gUnknown_030030A4.bits.win0_enable_bg1 = 1;
    gUnknown_030030A4.bits.win0_enable_bg2 = 1;
    gUnknown_030030A4.bits.win0_enable_bg3 = 1;
    gUnknown_030030A4.bits.win0_enable_obj = 1;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;

    SetDefaultColorEffects();

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg1 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;

    gUnknown_03002020 = 0xF;
    gUnknown_03002B28 = 6;
}
