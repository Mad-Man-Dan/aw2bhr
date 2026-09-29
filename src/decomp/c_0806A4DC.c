#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A4DC.
 * sub_0806A4DC @ 0x0806A4DC
 */

#include "hardware.h"

/* All bitfield, and the mask arithmetic reads straight off it: one `orr` per
 * field set to 1 and one `and` per field cleared, in source order, with the
 * complement constants shared by `subs` from the second one on. The lone
 * `mov #0x11; neg` is materialised separately because ~0x10 is wanted twice --
 * once for target1_enable_obj in byte 0 and once for target2_enable_obj in
 * byte 1 -- so it lives in r3 across both halves. */
void MeteorImpactGlow_Init(void)
{
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg0 = 0;
    gUnknown_030030E0.bits.target1_enable_bg1 = 0;
    gUnknown_030030E0.bits.target1_enable_bg2 = 0;
    gUnknown_030030E0.bits.target1_enable_bg3 = 0;
    gUnknown_030030E0.bits.target1_enable_obj = 0;
    gUnknown_030030E0.bits.target2_enable_bg0 = 1;
    gUnknown_030030E0.bits.target2_enable_bg1 = 1;
    gUnknown_030030E0.bits.target2_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 0;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
}
asm(".global sub_0806A4DC\n.thumb_set sub_0806A4DC, MeteorImpactGlow_Init\n");
