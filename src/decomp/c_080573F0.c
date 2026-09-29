#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080573F0.
 * sub_080573F0 @ 0x080573F0, sub_0805741C @ 0x0805741C
 */

#include "hardware.h"

void EnableBattleAnimWindowLayers(void)
{
    gUnknown_030030A4.bits.win0_enable_bg2 = 1;
    gUnknown_030030A4.bits.win0_enable_obj = 1;
    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_bg3 = 1;
    gUnknown_030030A4.bits.win1_enable_obj = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;
}
asm(".global sub_080573F0\n.thumb_set sub_080573F0, EnableBattleAnimWindowLayers\n");

void SetBg2Bg3ControlBySide(u16 a)
{
    /* The BG3 shadow is written through a scalar cast, not through
     * gUnknown_030030B4.raw: an aggregate-member destination swaps the
     * operands of the `orr` that builds the value (see docs/agbcc-codegen.md),
     * and the ROM has the loaded halfword as the OR's destination. The
     * gUnknown_0300251C store two lines down is the same expression and takes
     * the same cast, for the same reason -- that symbol became a union in
     * wave 13 when MatchSetupScreen_Init read its chr_block bitfield. */
    *(u16 *)&gUnknown_030030B4 = gUnknown_085538AE[a ^ 1] | 0x608;
    *(u16 *)&gUnknown_0300251C = gUnknown_085538AE[a] | 0x70C;
}
asm(".global sub_0805741C\n.thumb_set sub_0805741C, SetBg2Bg3ControlBySide\n");
