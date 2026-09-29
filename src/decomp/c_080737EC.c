#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080737EC.
 * sub_080737EC @ 0x080737EC
 */

#include "hardware.h"
#include "proc.h"
struct Unk080737EC
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
};

void CircleWipe_Loop(struct Unk080737EC *proc)
{
    int v;
    int t;

    t = Interpolate(4, 1, 0x1E0, proc->unk58, proc->unk5c);
    gUnknown_0202FDE4 = gUnknown_0202F8DC;
    v = t;
    if (t < 0)
        v = 0;
    if (v > 0xF0)
        v = 0xF0;
    gUnknown_03002B68 = 0;
    gUnknown_030024E4 = v * 2;
    gDispIo.disp_ct.win1_enable = 1;
    gUnknown_030030A4.bits.win1_enable_bg0 = 1;
    gUnknown_030030A4.bits.win1_enable_bg1 = 1;
    gUnknown_030030A4.bits.win1_enable_bg2 = 1;
    gUnknown_030030A4.bits.win1_enable_bg3 = 1;
    gUnknown_030030A4.bits.win1_enable_obj = 1;
    gUnknown_030030DC.bits.win0_enable_bg0 = 0;
    gUnknown_030030DC.bits.win0_enable_bg1 = 0;
    gUnknown_030030DC.bits.win0_enable_bg2 = 0;
    gUnknown_030030DC.bits.win0_enable_bg3 = 0;
    gUnknown_030030DC.bits.win0_enable_obj = 0;
    BuildCircleWindowTable(t);
    proc->unk58++;
    REG_DMA0CNT_H = 0;
    REG_DMA0SAD = (u32)gUnknown_0202FDE4;
    REG_DMA0DAD = (u32)&REG_WIN1H;
    REG_DMA0CNT_L = 1;
    REG_DMA0CNT_H = 0xA240;
    if (proc->unk58 >= proc->unk5c)
    {
        REG_DMA0CNT_H = 0;
        REG_DMA0SAD = 0;
        REG_DMA0DAD = 0;
        REG_DMA0CNT_L = 0;
        gDispIo.disp_ct.win1_enable = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_080737EC\n.thumb_set sub_080737EC, CircleWipe_Loop\n");
