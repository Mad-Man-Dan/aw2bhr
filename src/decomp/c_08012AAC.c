#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012AAC.
 * sub_08012AAC @ 0x08012AAC, sub_08012AD4 @ 0x08012AD4
 */

#include "hardware.h"

/* Enables the VBlank interrupt in the DISPSTAT shadow, pushes the shadow to
 * the register immediately rather than waiting for the next flush, and enables
 * interrupt source 2.
 *
 * This function and DisableVBlankInterruptNow are why gUnknown_030020B4 has to be a union:
 * the bit is set through `.bits` (`ldrb` + `movs` mask) and the very next
 * statement reads the same object whole as a halfword. The hardware.h note on
 * the symbol names DisableVBlankInterruptNow as the case that forced it. */
void EnableVBlankInterruptNow(void)
{
    gUnknown_030020B4.bits.vblank_int_enable = 1;
    REG_DISPSTAT = gUnknown_030020B4.raw;

    UpdateInterruptEnable(2, 1);
}
asm(".global sub_08012AAC\n.thumb_set sub_08012AAC, EnableVBlankInterruptNow\n");

/* EnableVBlankInterruptNow's undo, and the exact shape src/decomp/c_08012A24.c's
 * sub_08012A34 has for the HBlank bit: clear the bit, push the shadow, then
 * UpdateInterruptEnable with a NEGATIVE second argument. Clearing a single bit is
 * `movs #9; rsbs #0` -- agbcc materialises ~8 as -9 rather than pooling
 * 0xFFFFFFF7. */
void DisableVBlankInterruptNow(void)
{
    gUnknown_030020B4.bits.vblank_int_enable = 0;
    REG_DISPSTAT = gUnknown_030020B4.raw;

    UpdateInterruptEnable(1, -2);
}
asm(".global sub_08012AD4\n.thumb_set sub_08012AD4, DisableVBlankInterruptNow\n");
