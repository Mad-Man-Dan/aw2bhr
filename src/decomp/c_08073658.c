#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073658.
 * sub_08073658 @ 0x08073658
 */

#include "hardware.h"

/* MATCHED, wave 46 (W46-A), first attempt.
 *
 * PROMOTION NEEDS A .rodata WORD PLACED: add "rodata": ["0x081CC028"] to this
 * function's data/promoted.json entry. 0x081CC028 holds 0x0202FDE4, i.e.
 * &gUnknown_0202FDE4 -- it is this unit's own -fforce-addr copy of that address,
 * not an object (verified against baserom.gba; 0x081CC024 and 0x081CC02C hold
 * the same value and are the private copies belonging to BgWave_Loop and
 * PolygonWipe_HBlankHandler). Naming the global directly is the honest spelling and agbcc
 * rebuilds the word.
 *
 * Points the HBlank slot at gUnknown_0202F8DC, refills the BOTTOM 28 of its
 * 0xA0 scanline entries from the ROM table gUnknown_08614154, and arms DMA0 to
 * feed REG_BLDY one halfword per line. Every reach to gUnknown_0202FDE4 in this
 * function -- the store, the per-pass reload inside the loop and the
 * REG_DMA0SAD read -- goes through that one .rodata word. */

void ScanlineDarkenBg0_Loop(void)
{
    int i;

    gUnknown_0202FDE4 = gUnknown_0202F8DC;

    for (i = 0; i < 0x1c; i++)
        ((u16 *)gUnknown_0202FDE4)[i + 0x84] = gUnknown_08614154[i];

    REG_DMA0CNT_H = 0;
    REG_DMA0SAD = (u32)gUnknown_0202FDE4;
    REG_DMA0DAD = (u32)&REG_BLDY;
    REG_DMA0CNT_L = 1;
    REG_DMA0CNT_H = 0xA240;
}
asm(".global sub_08073658\n.thumb_set sub_08073658, ScanlineDarkenBg0_Loop\n");
