#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AD24.
 * sub_0808AD24 @ 0x0808AD24
 */

#include "hardware.h"

void StopFlashTimer(void)
{
    REG_IME = 0;

    *gUnknown_03000F78++ = 0;
    *gUnknown_03000F78-- = 0;

    REG_IE &= ~(INTR_FLAG_TIMER0 << gUnknown_03000F70);
    REG_IME = gUnknown_03000F7C;
}
asm(".global sub_0808AD24\n.thumb_set sub_0808AD24, StopFlashTimer\n");
