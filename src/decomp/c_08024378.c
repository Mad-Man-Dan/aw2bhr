#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08024378.
 * sub_08024378 @ 0x08024378
 */

#include "hardware.h"

/* Blend/BG setup: BG priorities, then the shared reset SetDefaultColorEffects, then
 * BLDCNT built field by field, then the alpha coefficients, then ResetWindowShadows.
 *
 * Every write here is `.bits` -- `ldrb` + a `movs`/`rsbs` mask throughout, no
 * `ldrh` + pool mask anywhere -- so no `.raw` / `*(u16 *)&` question arises.
 * The byte-0 store combines two field sets: agbcc merges the `effect` and
 * `target1_enable_bg0` read-modify-writes into one ldrb/strb pair, and the
 * `orrs r0, r4` reuses the constant 1 that the gUnknown_030030B4 priority
 * write left in r4 (which is what forces r4/r5 into the push list).
 */

void SetMapLayersBg0BlendFade(void)
{
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 3;
    SetDefaultColorEffects();
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg0 = 1;
    gUnknown_030030E0.bits.target2_enable_bg1 = 1;
    gUnknown_030030E0.bits.target2_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    ResetWindowShadows();
}
asm(".global sub_08024378\n.thumb_set sub_08024378, SetMapLayersBg0BlendFade\n");
