#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030F60.
 * sub_08030F60 @ 0x08030F60
 */

#include "hardware.h"

/* Closes a wave-33 park. Two facts, both of which the park had recorded
 * backwards:
 *
 * 1. gUnknown_0849B188 is NOT `const`. The qualifier let cse keep the HImode
 *    value of g[i] alive across the store to gUnknown_03002B40, so the sums
 *    below reused it; without it cse must reload, and since the sums are stored
 *    into u8 globals shorten_binary_op has narrowed them to QImode, so the
 *    reloads come back as `ldrb`. Those two `ldrb`s are exactly the 4 bytes the
 *    park was missing. See the header comment on the global.
 * 2. `int i = a1 * 4;` does NOT cost the folded `lsls #3`. combine merges the
 *    local's `i << 1` with its own `a1 << 2` into one `a1 << 3` while leaving
 *    the `a1 * 4` alive for the i+1 / i+2 / i+3 siblings -- which is how that
 *    pseudo is created before every other statement and wins the register the
 *    ROM holds it in.
 *
 * Operand order in the sums is readable: agbcc loads the SECOND operand first,
 * so `g[i] + g[i+2]` is what puts the `ldrb` of g[i+2] ahead of the reload of
 * g[i]. Four bitfield writes per group collapse into byte-wide
 * read-modify-writes the same way c_08030F20.c documents. */
void LinkScreenSetupWindow(int a1)
{
    int i = a1 * 4;

    gDispIo.disp_ct.win0_enable = 1;

    gUnknown_03002B40 = gUnknown_0849B188[i];
    gUnknown_03002B4C = gUnknown_0849B188[i] + gUnknown_0849B188[i + 2];
    gUnknown_03002EFC = gUnknown_0849B188[i + 1];
    gUnknown_03002B44 = gUnknown_0849B188[i + 1] + gUnknown_0849B188[i + 3];

    gUnknown_030030A4.bits.win0_enable_bg0 = 1;
    gUnknown_030030A4.bits.win0_enable_bg1 = 1;
    gUnknown_030030A4.bits.win0_enable_bg2 = 1;
    gUnknown_030030A4.bits.win0_enable_bg3 = 1;
    gUnknown_030030A4.bits.win0_enable_obj = 1;
    gUnknown_030030A4.bits.win0_enable_blend = 0;

    gUnknown_030030DC.raw = 0;
    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 0;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;

    LinkScreenSetupBlend();
}
asm(".global sub_08030F60\n.thumb_set sub_08030F60, LinkScreenSetupWindow\n");
