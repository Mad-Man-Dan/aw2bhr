#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A340.
 * sub_0808A340 @ 0x0808A340
 */

#include "hardware.h"

/* `(x & 0x3f) | 0x40` on BYTE 1 of gUnknown_03001FE8 is the 2-bit BGCNT `size`
 * field at bits 14-15 being assigned 1 -- an AND against the complement of the
 * field's group mask followed by an OR, which is store_fixed_bit_field. Byte 1
 * and `ldrb`/`strb` is what makes it `.bits` rather than the `.raw` view its
 * neighbours use for pool-masked `ldrh` writes. */
void CoDesignEditor_ReturnToBrowse(void)
{
    gUnknown_03001FE8.bits.size = 1;
    gUnknown_03005908 = 0;

    CoDesignEditor_DrawHelpText();
}
asm(".global sub_0808A340\n.thumb_set sub_0808A340, CoDesignEditor_ReturnToBrowse\n");
