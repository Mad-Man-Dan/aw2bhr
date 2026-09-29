#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085F40.
 * sub_08085F40 @ 0x08085F40
 */

#include "hardware.h"

void MapSelect_SetBlend(void)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0xc;
    gUnknown_03002B28 = 4;
    gUnknown_03001FFC = 0;

    /* Target1 = BG2, target2 = BG1. Both have to go through the scalar cast
     * rather than through `.raw`: the aggregate-member lvalue swaps the OR's
     * operands (docs/agbcc-codegen.md), and here that also drags the live
     * zero from the `gUnknown_03001FFC = 0` above into a spurious
     * `orr r0, r0, r2` and costs a `push {r4, lr}`. The `vu16` cast is wrong
     * a third way -- it keeps the intermediate `strh` instead of forwarding
     * the store. Only the plain `u16 *` cast is exact. */
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x04;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x200;
}
asm(".global sub_08085F40\n.thumb_set sub_08085F40, MapSelect_SetBlend\n");
