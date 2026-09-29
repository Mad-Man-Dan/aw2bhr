#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080638A8.
 * sub_080638A8 @ 0x080638A8
 */

#include "hardware.h"

/* Clears one 4bpp tile (32 bytes) of VRAM with DMA3: fixed source pointing at
 * a zeroed halfword on the stack, 0x10 units of 16 bits, destination
 * 0x06000000 + tile * 32.
 *
 * The three registers are reached through a `vu32 *` cursor, not by name. That
 * is what the ROM has and it is load-bearing: one pool word plus displacements
 * 0/4/8. Writing REG_DMA3SAD/DAD/CNT as three separate lvalues emits
 * `add r1, r1, #4` between the stores instead, even though every address is a
 * compile-time constant.
 *
 * The trailing read of dma[2] is discarded -- the usual dummy read after
 * triggering a transfer, and it only survives because the pointer is volatile.
 */

void ClearVramTileDma(int tile)
{
    vu16 fill = 0;
    vu32 *dma = (vu32 *)(REG_BASE + REG_OFFSET_DMA3SAD);

    dma[0] = (u32)&fill;
    dma[1] = 0x06000000 + tile * 32;
    dma[2] = 0x81000010;
    dma[2];
}
asm(".global sub_080638A8\n.thumb_set sub_080638A8, ClearVramTileDma\n");
