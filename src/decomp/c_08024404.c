#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08024404.
 * sub_08024404 @ 0x08024404
 */

#include "hardware.h"

/* Four BG priorities and nothing else. `movs r2,#4; rsbs r2,r2,#0; ands` is
 * the BITFIELD tell from docs/agbcc-codegen.md -- a mask arrived at by negating
 * a constant, where a scalar `&= ~3` would emit a bare `movs #0xFC` -- so
 * `(x & ~3) | K` is ONE 2-bit field assignment and not two operations. The
 * fourth store loses the mask entirely because priority 3 fills the field, and
 * `(x & ~3) | 3` folds to `x | 3`.
 *
 * gUnknown_03002B6C and its three neighbours are `union BgCntBuf` in
 * include/hardware.h, not unknown-globals.h. No `.raw` / `*(u16 *)&` question
 * arises here: every access is `ldrb` + a byte mask, i.e. `.bits`.
 *
 * This is src/decomp/c_08024378.c's opening with a different permutation of the
 * four priorities; SetMapLayersDefault and SetMapLayerPrioritiesDefault are the other two. */

void SetMapLayersRangeBehindUnits(void)
{
    gUnknown_030030B4.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_03002B6C.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;
    SetDefaultColorEffects();
    ResetWindowShadows();
}
asm(".global sub_08024404\n.thumb_set sub_08024404, SetMapLayersRangeBehindUnits\n");
