#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C700.
 * sub_0806C700 @ 0x0806C700
 */

#include "hardware.h"

/* Same blend-shadow shape as CreditsResultFade_Init, with no proc argument and the
 * first target group cleared outright rather than set. */

void sub_0806C700(void)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = 8;
    gUnknown_03001FFC = gUnknown_03001FFC;

    *(u16 *)&gUnknown_030030E0 = *(u16 *)&gUnknown_030030E0 & 0xFFE0;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0xF00;
    gUnknown_030030E0.bits.target2_enable_bd = 0;
}
