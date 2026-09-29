#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078F60.
 * sub_08078F60 @ 0x08078F60
 */

#include "hardware.h"

/* Statement-for-statement the same run as the matched c_0807F238.c, with three
 * constants changed: the coefficients are 0/0x10 rather than 8/8, the target1
 * group is cleared to 0 rather than set to 2, and the target2 insert is 0x800
 * (target2_enable_bg3 alone) rather than 0xA00.
 *
 * `gUnknown_030030E0.raw = gUnknown_030030E0.raw & 0xFFE0;` with no `| K` is
 * the target1 group set to zero; the ROM emits the pool mask and no `orr` at
 * all, which is what an insert of 0 looks like.
 *
 * This function was wave 23's test of the wave-21 "live CSE-able zero" rule:
 * `gUnknown_03002020 = 0;` is the statement immediately before the first raw
 * insert, which is the FadeFromBlack_OnInitUnused shape the rule predicts must diverge. It
 * does not. BOTH spellings -- `.raw` and `*(u16 *)&gUnknown_030030E0` -- match
 * byte-for-byte. See the wave-23 section of docs/agbcc-codegen.md. */
void ResultsScreen_SetupBlend(void)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = gUnknown_03001FFC;

    gUnknown_030030E0.raw = gUnknown_030030E0.raw & 0xFFE0;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0x800;
    gUnknown_030030E0.bits.target2_enable_bd = 0;

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 3;

    gUnknown_030030DC.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;
}
asm(".global sub_08078F60\n.thumb_set sub_08078F60, ResultsScreen_SetupBlend\n");
