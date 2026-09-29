#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019A0C.
 * sub_08019A0C @ 0x08019A0C
 */

#include "hardware.h"

/* Sets window 0 to the rectangle (x, y, x + w, y + h) and opens every layer
 * inside it while leaving the blend target out of the outside region.  The
 * enable goes through the gDispIo shadow (bit 13 of DISPCNT is byte 1 bit 5,
 * hence `ldrb [r6,#1]; movs #0x20; orrs; strb`); the four bound registers have
 * no shadow and are written directly. */
void SetWindow0Rect(s16 x, s16 y, s16 w, s16 h)
{
    gDispIo.disp_ct.win0_enable = TRUE;
    REG_WIN0H = (x << 8) | (x + w);
    REG_WIN0V = (y << 8) | (y + h);
    REG_WININ = 0x3f;
    REG_WINOUT = 0x1f;
}
asm(".global sub_08019A0C\n.thumb_set sub_08019A0C, SetWindow0Rect\n");
