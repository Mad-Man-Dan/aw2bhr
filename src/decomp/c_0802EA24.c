#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802EA24.
 * sub_0802EA24 @ 0x0802EA24
 */

#include "hardware.h"

/* Arm the VBlank IRQ, enable interrupts, blank the screen, and install the
 * handler pair -- the link-error screen's entry point (the Fire Emblem decomps
 * call the same shape StartSioErrorScreen).
 *
 * REG_DISPCNT is the one register here built with `movs #0x80; lsls #0x13`
 * rather than a pool word: 0x04000000 is an 8-bit constant shifted, and the
 * other two addresses are not. Nothing in the source distinguishes them. */
void StartSioErrorScreen(void)
{
    REG_DISPSTAT = 8;
    REG_IME = 1;
    REG_DISPCNT = 0;
    sub_080366D0(OnVBlank_SioError);
    sub_080366C4(OnMain_SioError);
}
asm(".global sub_0802EA24\n.thumb_set sub_0802EA24, StartSioErrorScreen\n");
