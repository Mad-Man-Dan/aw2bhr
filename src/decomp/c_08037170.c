#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037170.
 * sub_08037170 @ 0x08037170
 */

/* Draws a3 as right-aligned decimal digits, one 8-pixel cell per digit,
 * walking leftwards from a1 - 8.
 *
 * The third argument is the OAM blob gUnknown_0848B6D6 named DIRECTLY. There is
 * no pointer global here: agbcc -fforce-addr parks `&gUnknown_0848B6D6` in this
 * unit's own .rodata, and that word is what the splitter named
 * gUnknown_08090EEC (0x08090EEC holds 0x0848B6D6). Declaring gUnknown_08090EEC
 * as a `void *` global and dereferencing it costs a third load and +12 bytes;
 * binding its address to a local kills the extra load but makes agbcc
 * precompute the argument before the DivRem call, which spends another
 * callee-saved register. An array name is an address constant, so it is neither
 * precomputed nor double-loaded -- that is what puts the load in the ROM's
 * position, after the call and straight into r2.
 *
 * `(u16)(a2 & 0xff)` keeps its cast: PutOamHi's second parameter is `s32`,
 * so without it the value is a plain int and LICM hoists the whole thing. With
 * it, only the invariant `<< 16` hoists and the `>> 16` stays at the call,
 * which is the ROM's `lsls r7, r0, #0x10` / `lsrs r1, r7, #0x10` pair. */
void PutRightAlignedNumberSprites(u16 a1, u16 a2, u16 a3, u16 a4)
{
    u16 x;

    x = a1 - 8;
    if (a3 > 999)
        a3 = 999;
    while (1)
    {
        PutOamHi(x & 0x1ff, (u16)(a2 & 0xff), gUnknown_0848B6D6,
                     DivRem(a3, 10) * 2 + a4 + 0x1000);
        a3 = Div(a3, 10);
        if (a3 == 0)
            break;
        x -= 8;
    }
}
asm(".global sub_08037170\n.thumb_set sub_08037170, PutRightAlignedNumberSprites\n");
