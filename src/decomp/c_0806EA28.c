#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806EA28.
 * sub_0806EA28 @ 0x0806EA28, sub_0806EA88 @ 0x0806EA88
 */

#include "hardware.h"
#include "proc.h"
/* Alpha-blend fade step; 0x20-frame ramp counted at +0x58. Sibling of
 * sub_0806E968, which differs only in the EVA/EVB pair it publishes. */
struct Unk0806EA28
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void sub_0806EA28(struct Unk0806EA28 *proc)
{
    int v;

    v = Interpolate(0, 0, 8, proc->unk58, 0x20);
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = v;
    gUnknown_03002B28 = 0x10 - v;
    gUnknown_03001FFC = 0;

    if (proc->unk58 > 0x1f)
        Proc_Break(proc);
    else
        proc->unk58++;
}

/* Window setup: turns on window 0 in DISPCNT, sets its four edge registers
 * (left 0x38 / top 0x80 / right 0xB8 / bottom 0x90), and opens all five
 * layers plus blending inside it, with BG0 masked out of the second shadow.
 *
 * The three saved high registers are NOT a loop signal -- they hold the
 * constants. Every field here is a distinct one-bit `orr`, and agbcc CSEs
 * 1 / 2 / 4 / 8 / 0x10 / 0x20 into registers because each is used more than
 * once; 0x20 (sb) is live from the DISPCNT store at the top all the way to
 * the win1 blend bit at the bottom, which is what forces the r8/sb/sl spill.
 *
 * The two gUnknown_030030A4 stores are one statement run apart: agbcc keeps
 * the accumulated byte in r1 across the whole gUnknown_030030DC sequence and
 * only stores it after, because the two shadows are distinct objects and
 * neither is volatile. Source order is exactly as written below --
 * the `.bits.win0_enable_blend = 1` really does come after the second
 * shadow's block. */
void SoundRoomSetTitleTextWindow(void)
{
    gDispIo.disp_ct.win0_enable = 1;

    gUnknown_03002B40 = 0x38;
    gUnknown_03002EFC = 0x80;
    gUnknown_03002B4C = 0xB8;
    gUnknown_03002B44 = 0x90;

    gUnknown_030030A4.bits.win0_enable_bg0 = 1;
    gUnknown_030030A4.bits.win0_enable_bg1 = 1;
    gUnknown_030030A4.bits.win0_enable_bg2 = 1;
    gUnknown_030030A4.bits.win0_enable_bg3 = 1;
    gUnknown_030030A4.bits.win0_enable_obj = 1;

    gUnknown_030030DC.bits.win0_enable_bg0 = 0;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;
}
asm(".global sub_0806EA88\n.thumb_set sub_0806EA88, SoundRoomSetTitleTextWindow\n");
