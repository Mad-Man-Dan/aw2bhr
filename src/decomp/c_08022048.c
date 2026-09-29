#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022048.
 * sub_08022048 @ 0x08022048
 */

#include "hardware.h"

/* A frame-driven VRAM refresh: one of four blits on a 0x32-frame cycle, gated
 * on GetCopyQueuePendingSize() being within the first 0x1000 of the scanline counter --
 * the same guard UpdateTerrainAnimation opens with, and it compiles to
 * `lsls #0x10; cmp` against 0x1000 << 16 rather than a plain compare.
 *
 * The destination is the BG1 character base, `gUnknown_03001FE8.bits.chr_block
 * * 0x4000`, plus 0x46A0 -- the same expression src/decomp/c_08013C54.c already
 * uses off the same IWRAM shadow of REG_BG1CNT. (Wave 36, W36-L first wrote
 * this as a `u32` global and a `lsls #0x1c; lsrs #0x1e` shift pair, which reads
 * the right bits off the wrong object and collides with hardware.h; W36-M
 * caught it.)
 *
 * All four arms are written out in full, in ascending case order. Cross-jumping
 * then folds only the last three instructions of cases 0x11 and 0x19 together
 * and leaves 0x2a -- which is instruction-identical to 0x11 -- completely
 * alone; that partial merge is the fingerprint of the source order, exactly as
 * in UpdateTerrainAnimation.
 *
 * Case 0 discards a GetUnitSheetFrameTileCount() result before the blit and adds no tile
 * offset to the source; the other three add `(GetUnitSheetFrameTileCount() & 0x3ff) * 0x20`
 * (0x40 for case 0x19) instead. */
void UpdateUnitSheetAnimation(void)
{
    if (GetCopyQueuePendingSize() <= 0x1000)
    {
        gUnknown_03003330++;

        switch (gUnknown_03003330 % 0x32)
        {
        case 0:
            GetUnitSheetFrameTileCount();
            CpuCopyAuto(GetUnitSheetGraphics(),
                         (void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x060046A0),
                         (GetUnitSheetFrameTileCount() & 0x3ff) << 5);
            break;
        case 0x11:
            CpuCopyAuto(GetUnitSheetGraphics() + ((GetUnitSheetFrameTileCount() & 0x3ff) << 5),
                         (void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x060046A0),
                         (GetUnitSheetFrameTileCount() & 0x3ff) << 5);
            break;
        case 0x19:
            CpuCopyAuto(GetUnitSheetGraphics() + ((GetUnitSheetFrameTileCount() & 0x3ff) << 6),
                         (void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x060046A0),
                         (GetUnitSheetFrameTileCount() & 0x3ff) << 5);
            break;
        case 0x2a:
            CpuCopyAuto(GetUnitSheetGraphics() + ((GetUnitSheetFrameTileCount() & 0x3ff) << 5),
                         (void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x060046A0),
                         (GetUnitSheetFrameTileCount() & 0x3ff) << 5);
            break;
        }
    }
}
asm(".global sub_08022048\n.thumb_set sub_08022048, UpdateUnitSheetAnimation\n");
