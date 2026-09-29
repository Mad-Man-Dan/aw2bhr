#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F378.
 * sub_0807F378 @ 0x0807F378
 */

#include "hardware.h"

/* The only member of this family with any control flow: a 16-iteration
 * `StartPalFadeToBlack(i, 0, proc)` loop (signed `cmp #0xf; ble`, so `i` is `int`),
 * then the usual straight run.
 *
 * `proc` is `ProcPtr` because StartPalFadeToBlack forwards its third argument to
 * StartPalFade in r3, which hands it to Proc_Start as the parent -- exactly
 * StartPalFadeToWhite's shape. The prologue is bare, so it is a full word either way.
 *
 * `gUnknown_03002020 = gUnknown_03002020;` and the same for 03002B28 are real
 * volatile self-stores: only a volatile lvalue emits the bare ldrh/strh pair to
 * one address.
 *
 * The target1 group insert was probed BOTH ways -- `.raw` and
 * `*(u16 *)&gUnknown_030030E0` -- and they are byte-identical here. */
void sub_0807F378(ProcPtr proc)
{
    int i;

    for (i = 0; i < 16; i++)
        StartPalFadeToBlack(i, 0, proc);

    SetDispEnable(1, 1, 1, 1, 0);

    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = gUnknown_03002020;
    gUnknown_03002B28 = gUnknown_03002B28;
    gUnknown_03001FFC = 0x10;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 0x1F;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
}
