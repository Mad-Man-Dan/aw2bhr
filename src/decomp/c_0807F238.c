#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F238.
 * sub_0807F238 @ 0x0807F238
 */

#include "hardware.h"

/* Blend/BG setup with no proc, and the immediate neighbour of CoSelectConfirm_BeginFadeToWhite
 * (already matched) -- same statement run, different constants.
 *
 * NO zero is live anywhere in this function (the coefficients are stored 8/8
 * and BLDY is a volatile SELF-STORE, not a `= 0`), so the union `.raw` view is
 * byte-exact here and the `*(u16 *)&` cast is not needed -- the control case in
 * the blend-shadow section of docs/agbcc-codegen.md. Probed: the two spellings
 * are identical in this function.
 *
 * `gUnknown_03002020 = 8; gUnknown_03002B28 = 8;` needs no `v = ` chaining --
 * probed against the c_08037260.c spelling and both emit the same single
 * `movs r1,#8` feeding both stores.
 *
 * `gUnknown_03001FFC = gUnknown_03001FFC;` is real source: only a volatile
 * lvalue emits the bare `ldrh`/`strh` pair to the same address. */
void CoSelect_SetupBlend(void)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 8;
    gUnknown_03002B28 = 8;
    gUnknown_03001FFC = gUnknown_03001FFC;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 0x02;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0xA00;
    gUnknown_030030E0.bits.target2_enable_bd = 0;

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 3;

    gUnknown_030030DC.bits.win0_enable_blend = 0;
    gUnknown_030030DC.bits.win1_enable_blend = 0;
}
asm(".global sub_0807F238\n.thumb_set sub_0807F238, CoSelect_SetupBlend\n");
