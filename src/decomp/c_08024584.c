#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08024584.
 * sub_08024584 @ 0x08024584
 */

#include "hardware.h"

/* See src/decomp/c_08024404.c for the 2-bit-field reading of the
 * `movs #4; rsbs; ands` chains. This permutation is the same one
 * src/decomp/c_08024378.c opens with -- 0/2/1/3 across BG0..BG3. */

void SetMapLayersDefault(void)
{
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 3;
    SetDefaultColorEffects();
    ResetWindowShadows();
}
asm(".global sub_08024584\n.thumb_set sub_08024584, SetMapLayersDefault\n");
