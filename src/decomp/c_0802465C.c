#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802465C.
 * sub_0802465C @ 0x0802465C
 */

#include "hardware.h"

/* SetMapLayersDefault without the trailing ResetWindowShadows -- see
 * src/decomp/c_08024404.c for the 2-bit-field reading. */

void SetMapLayerPrioritiesDefault(void)
{
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 3;
    SetDefaultColorEffects();
}
asm(".global sub_0802465C\n.thumb_set sub_0802465C, SetMapLayerPrioritiesDefault\n");
