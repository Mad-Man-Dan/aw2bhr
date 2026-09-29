#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A47C.
 * sub_0808A47C @ 0x0808A47C
 */

#include "hardware.h"

/* The long member of the family: c_0807F238.c's blend run, then the four BG
 * priority shadows, then the window groups, then four u8 globals and the
 * gDispIo window enables.
 *
 * THE CAST IS REQUIRED HERE and it is the whole difference -- `.raw` compiles
 * to 336 bytes against 328. With `.raw`, cse hoists the `0` belonging to
 * `gUnknown_03002B68 = 0;` -- twenty instructions LATER -- back into the middle
 * of the 0xA00 insert and parks it in a third high register, so the prologue
 * becomes `push {r4,r5,r6}` of high regs where the ROM pushes two. The cast
 * keeps that zero local to its own store. This is the one function of wave 23's
 * six where the two spellings differ, and it differs in the direction OPPOSITE
 * to the wave-21 rule (no zero is live ACROSS the write here). See the wave-23
 * section of docs/agbcc-codegen.md.
 *
 * `gUnknown_030030DC.bits.win0_enable_blend = 1;` is written before the
 * 030030A4 group and its byte-0 store SINKS past it: agbcc merges it with the
 * win0_enable_bg0..obj writes that follow into one read-modify-write, which is
 * the same A4/DC interleave already recorded on these two shadows in
 * include/unknown-globals.h. */
void CoDesignEditor_SetupBlend(void)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 8;
    gUnknown_03002B28 = 8;
    gUnknown_03001FFC = gUnknown_03001FFC;

    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x02;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0xA00;
    gUnknown_030030E0.bits.target2_enable_bd = 0;

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 3;

    gUnknown_030030DC.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 0;

    gUnknown_030030A4.bits.win1_enable_bg0 = 1;
    gUnknown_030030A4.bits.win1_enable_bg1 = 0;
    gUnknown_030030A4.bits.win1_enable_bg2 = 0;
    gUnknown_030030A4.bits.win1_enable_bg3 = 0;
    gUnknown_030030A4.bits.win1_enable_obj = 0;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_03002B68 = 0;
    gUnknown_03002B30 = 0x90;
    gUnknown_030024E4 = 0xF0;
    gUnknown_030020B8 = 0xA0;

    gDispIo.disp_ct.win0_enable = 0;
    gDispIo.disp_ct.win1_enable = 1;
    gDispIo.disp_ct.objwin_enable = 0;
}
asm(".global sub_0808A47C\n.thumb_set sub_0808A47C, CoDesignEditor_SetupBlend\n");
