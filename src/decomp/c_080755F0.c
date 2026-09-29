#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080755F0.
 * sub_080755F0 @ 0x080755F0
 */

#include "hardware.h"

/* Loads the palette and tiles for the blend overlay, then arms the blend
 * registers -- the c_0806C700.c tail with 0xF00 for its target2 group, plus two
 * single-bit fixups. The `movs #0x21; rsbs` mask is the bitfield tell described
 * in hardware.h: a scalar `&= ~0x20` would emit a bare `movs #0xdf`, whereas
 * arriving at 0xDF by negating 0x21 is what `bits.target1_enable_bd = 0` does
 * on byte 0. The `orrs #0x20` on byte 1 is bit 13, target2_enable_bd. */
void WorldMapReticle_Init(void)
{
    ApplyPaletteExt(gUnknown_081D20AC, 0x2C0, 0x20);
    Decompress(gUnknown_081D18E8, (void *)0x06011480);

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = 0;
    *(u16 *)&gUnknown_030030E0 = *(u16 *)&gUnknown_030030E0 & 0xFFE0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0xF00;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    gUnknown_030030E0.bits.target2_enable_bd = 1;

    PlayMusicOrSfx2(0x1DF);
}
asm(".global sub_080755F0\n.thumb_set sub_080755F0, WorldMapReticle_Init\n");
