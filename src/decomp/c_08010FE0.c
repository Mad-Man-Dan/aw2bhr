#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010FE0.
 * sub_08010FE0 @ 0x08010FE0, sub_08011018 @ 0x08011018
 */

#include "hardware.h"

/* The 0x08011 counterpart of FadeToWhite_OnInit: sub_08010FA0 (src/decomp) sets the
 * full target1 mask and clears the coefficients, then this overrides the blend
 * effect and seeds BLDY at 0x10 before kicking sub_080128D0.
 *
 * `.bits.effect = 3` emits `ldrb; orrs #0xc0; strb` with NO masking AND,
 * because 3 fills the 2-bit field -- the exact discriminator hardware.h
 * records, and the only difference from sub_08011018 below, which sets 2. */
void sub_08010FE0(void)
{
    sub_08010FA0();

    gUnknown_030030E0.bits.effect = 3;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0x10;

    sub_080128D0();
}

/* sub_08010FE0's twin, differing only in the blend effect: 2 rather than 3, so
 * the field no longer fills and agbcc emits the masking `ands #0x3f` ahead of
 * the `orrs #0x80`. */
void sub_08011018(void)
{
    sub_08010FA0();

    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0x10;

    sub_080128D0();
}
