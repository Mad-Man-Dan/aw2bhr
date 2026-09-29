#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078B08.
 * sub_08078B08 @ 0x08078B08, sub_08078B74 @ 0x08078B74
 */

#include "hardware.h"

void BeginFadeToWhite(void)
{
    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 0x1F;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;
}
asm(".global sub_08078B08\n.thumb_set sub_08078B08, BeginFadeToWhite\n");

void BeginFadeToBlack(void)
{
    gUnknown_030030E0.bits.effect = 3;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 0x1F;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;
}
asm(".global sub_08078B74\n.thumb_set sub_08078B74, BeginFadeToBlack\n");
