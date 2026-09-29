#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F2FC.
 * sub_0807F2FC @ 0x0807F2FC
 */

#include "hardware.h"

/* Brighten setup with no proc: effect = 2, the two BLDALPHA coefficients
 * left alone, BLDY zeroed, and a target1/target2 mask that deliberately
 * EXCLUDES the backdrop.
 *
 * `gUnknown_03002020 = gUnknown_03002020;` is real source, not a typo. Both
 * coefficients are volatile, and a self-assignment on a volatile lvalue is
 * the only thing that emits `ldrh r0,[r1]; strh r0,[r1]` -- agbcc deletes
 * `g = g` outright on a plain object. It is the same tell recorded for
 * gUnknown_03002F18 in unknown-globals.h, here on two symbols at once, and
 * it is independent confirmation that the volatile qualifier on this pair is
 * right.
 *
 * The `movs #0x21; rsbs` clearing target1_enable_bd is the byte-mask
 * bitfield idiom from c_080688E4.c, sitting between two `.raw` inserts on
 * the same object -- `ldrh` with a pool mask either side of an `ldrb` with a
 * `movs`/`neg` mask, which is the hardware.h rule in one statement run. */
void CoSelectConfirm_BeginFadeToWhite(void)
{
    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = gUnknown_03002020;
    gUnknown_03002B28 = gUnknown_03002B28;
    gUnknown_03001FFC = 0;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 0xF;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0xF00;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
}
asm(".global sub_0807F2FC\n.thumb_set sub_0807F2FC, CoSelectConfirm_BeginFadeToWhite\n");
