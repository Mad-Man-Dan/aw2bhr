#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080735EC.
 * sub_080735EC @ 0x080735EC
 */

#include "hardware.h"

/* Points the HBlank DMA slot at gUnknown_0202F8DC, fills all 0xA0 scanlines
 * of that table with 0x10, and then sets up the blend: effect 3, both
 * coefficient shadows and BLDY cleared, target1 = bg0 alone, target1_enable_bd
 * cleared.
 *
 * The table pointer is re-loaded from the global on EVERY pass rather than
 * hoisted, because the halfword store through it may alias the pointer
 * variable itself and agbcc's alias analysis does not rule that out. Only the
 * global's ADDRESS is hoisted (r4).
 *
 * The blend tail is the same shape as src/decomp/c_0806717C.c: `.bits.effect`
 * is a QImode read-modify-write (here an all-ones value, so the AND drops out
 * and only `orr #0xc0` remains), the five target1 layer bits are ONE HImode
 * read-modify-write on `.raw` (a 5-bit field at bit 0 would have been QImode),
 * and target1_enable_bd really is a single bit -- the `movs #0x21; rsbs` mask
 * is the bitfield tell, where a scalar `&= ~0x20` would emit a bare
 * `movs #0xdf`. */

void ScanlineDarkenBg0_Init(void)
{
    int i;

    gUnknown_0202FDE4 = gUnknown_0202F8DC;

    for (i = 0; i < 0xA0; i++)
        ((u16 *)gUnknown_0202FDE4)[i] = 0x10;

    gUnknown_030030E0.bits.effect = 3;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 1;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
}
asm(".global sub_080735EC\n.thumb_set sub_080735EC, ScanlineDarkenBg0_Init\n");
