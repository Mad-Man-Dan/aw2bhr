#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011FF0.
 * sub_08011FF0 @ 0x08011FF0
 */

/* The gUnknown_0200B3B4 deferred-copy queue DRAIN, the consumer for the six
 * pushes promoted in src/decomp/c_08011D10.c. It walks the queue backwards --
 * `for (count--; (s16)count >= 0; count--)` -- and dispatches on the tag at
 * +0x0a, then clears the queue with ClearTileRigistry and blanks palette entry 0.
 *
 * BOTH DATA REFS ARE agbcc -fforce-addr ADDRESS CONSTANTS, not globals:
 *   [0x0808E51C] = 0x03002F30 -> gUnknown_03002F30
 *   [0x0808E520] = 0x0200B3B4 -> gUnknown_0200B3B4
 * Naming the real globals is the honest spelling and agbcc reproduces the
 * double load by itself. PROMOTION MUST CARRY BOTH POOL WORDS: add
 *   "rodata": ["0x0808E51C", "0x0808E520"]
 * to this function's data/promoted.json entry, then re-run
 * tools/split_rodata.py and tools/gen_lds.py.
 *
 * `fill16` MUST BE `vu16` AND `fill32` MUST NOT BE VOLATILE -- this was the
 * whole residual, 6 bytes at +0x108. A plain `u16 fill16` gives
 *   movs r1,#0 / mov r0,sp / strh r1,[r0]
 * because the Thumb `strh` address reload is created AFTER the constant
 * reload and then reused as CpuSet's first argument; the ROM has
 *   mov r1,sp / movs r0,#0 / strh r0,[r1]
 * plus a SECOND `mov r0, sp` at the call. Making the local volatile forces the
 * address into its own pseudo first and re-materialises it for the argument.
 * `fill32` needs no such thing: Thumb `str` addresses sp directly, so no
 * reload is created and the plain local already matches. */
void FlushTiles(void)
{
    vu16 fill16;
    u32 fill32;

    if (gUnknown_03002F30 == 0)
        return;

    for (gUnknown_03002F30--; (s16)gUnknown_03002F30 >= 0; gUnknown_03002F30--)
    {
        switch (gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk0a)
        {
        case 0:
        case 1:
            CpuCopyAuto((const void *)gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00,
                         (void *)gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk04,
                         gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk08);
            break;
        case 2:
            CpuSet((const void *)gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00,
                   (void *)gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk04,
                   gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk08 >> 1);
            break;
        case 3:
            Decompress((u8 *)gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00,
                       (void *)gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk04);
            break;
        case 4:
            fill16 = 0;
            CpuSet((const void *)&fill16, (void *)gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00,
                   (gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk08 >> 1) | 0x1000000);
            break;
        case 5:
            fill32 = 0;
            CpuFastSet(&fill32, (void *)gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk00,
                       (gUnknown_0200B3B4[(s16)gUnknown_03002F30].unk08 >> 2) | 0x1000000);
            break;
        }
    }

    ClearTileRigistry();
    *(vu16 *)0x05000000 = 0;
}
asm(".global sub_08011FF0\n.thumb_set sub_08011FF0, FlushTiles\n");
