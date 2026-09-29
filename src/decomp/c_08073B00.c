#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073B00.
 * sub_08073B00 @ 0x08073B00
 */

#include "hardware.h"
#include "proc.h"
/* Per-frame body of a window-wipe proc: three Interpolate curves drive three
 * scanline-range fills, the result is DMA0'd into WIN0H/WIN0V every HBlank,
 * and the proc breaks once the frame counter reaches its duration. */
struct Unk73B00Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58; /* frame counter */
    /* 5C */ int unk5c; /* duration */
};

void PolygonWipe_Loop(struct Unk73B00Proc *proc)
{
    s32 a = Interpolate(4, 0, 360, proc->unk58, proc->unk5c);
    s32 b = Interpolate(4, 0, 0xf0, proc->unk58, proc->unk5c);
    s32 c = Interpolate(4, 0x50, 0xa0, proc->unk58, proc->unk5c);

    SetWinEnable(1, 1, 0);

    gUnknown_030030A4.bits.win0_enable_bg0 = 1;
    gUnknown_030030A4.bits.win0_enable_bg1 = 0;
    gUnknown_030030A4.bits.win0_enable_bg2 = 1;
    gUnknown_030030A4.bits.win0_enable_bg3 = 1;
    gUnknown_030030A4.bits.win0_enable_obj = 1;

    gUnknown_030030A4.bits.win1_enable_bg0 = 0;
    gUnknown_030030A4.bits.win1_enable_bg1 = 1;
    gUnknown_030030A4.bits.win1_enable_bg2 = 0;
    gUnknown_030030A4.bits.win1_enable_bg3 = 0;
    gUnknown_030030A4.bits.win1_enable_obj = 0;

    gUnknown_030030DC.bits.win0_enable_bg0 = 0;
    gUnknown_030030DC.bits.win0_enable_bg1 = 0;
    gUnknown_030030DC.bits.win0_enable_bg2 = 0;
    gUnknown_030030DC.bits.win0_enable_bg3 = 0;
    gUnknown_030030DC.bits.win0_enable_obj = 0;

    DrawWipeEdgeLine(b * 2 - 0xf0, c, a, c - 0xa0, 0);
    DrawWipeEdgeLine(b * 2 - 0xf0, c, b, 0xa0, 0);

    a += 0xa0;
    a -= c;
    DrawWipeEdgeLine(a, c - 0xa0, b, 0xa0, 1);

    proc->unk58++;

    SwapWipeEdgeBuffers();

    REG_DMA0CNT_H = 0;
    REG_DMA0SAD = (u32)gUnknown_0202FDE0;
    REG_DMA0DAD = REG_BASE + REG_OFFSET_WIN0H;
    REG_DMA0CNT_L = 1;
    REG_DMA0CNT_H = 0xA640;

    if (proc->unk58 >= proc->unk5c)
    {
        SetHBlankInterruptHandler(0);

        REG_DMA0CNT_H = 0;
        REG_DMA0SAD = 0;
        REG_DMA0DAD = 0;
        REG_DMA0CNT_L = 0;

        SetWinEnable(0, 0, 0);

        Proc_Break(proc);
    }
}
asm(".global sub_08073B00\n.thumb_set sub_08073B00, PolygonWipe_Loop\n");
