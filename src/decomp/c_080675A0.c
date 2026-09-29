#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080675A0.
 * sub_080675A0 @ 0x080675A0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* Tear-down: whites out the whole 512-entry palette, re-enables all four
 * backgrounds and forced blank, and then either restarts this proc's script at
 * label 0 or breaks it, on the +0x64 flag.
 *
 * +0x64 is the same result flag src/decomp/c_08067554.c's sub_08067564 SETS to
 * 1 when A or START dismisses the message, so this is the far end of that pair.
 * It is read `ldrsh` here where that file declares it u16; the field only ever
 * holds 0 or 1, so the two spellings are byte-neutral for both functions and I
 * have left that file alone -- s16 here is what this function's own load
 * proves, and nothing yet discriminates.
 *
 * The palette loop descends: the ROM's giv init is `gPal + 0x3FE` stepping
 * `subs r0, #2`, which an ascending source cannot produce -- check_dbra_loop
 * reverses the COUNTER, not the pointer. 0x1ff, 0x7fff and 0x3fe are all pool
 * words because none fits an imm8.
 *
 * The four DISPCNT bg bits are one `ldrb` / four `orr` / one `strb` on byte 1,
 * and forced_blank a separate pair on byte 0: agbcc CSEs consecutive bitfield
 * writes that land in the same byte, exactly as in the matched
 * src/decomp/c_0802E960.c. */

struct Unk675A0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ s16 unk64;
};

void IntroT0_IDLE_080675A1(struct Unk675A0Proc *proc)
{
    int i;

    for (i = 0x1ff; i >= 0; i--)
    {
        gPal[i] = 0x7fff;
    }

    EnablePaletteSync();
    SetDefaultColorEffects();
    ResetWindowShadows();

    gDispIo.disp_ct.bg0_enable = 1;
    gDispIo.disp_ct.bg1_enable = 1;
    gDispIo.disp_ct.bg2_enable = 1;
    gDispIo.disp_ct.bg3_enable = 1;
    gDispIo.disp_ct.forced_blank = 1;

    SetVCountInterruptHandler(0);
    Proc_EndEach(ProcScr_IntroT3);

    if (proc->unk64 != 0)
    {
        Proc_Goto(proc, 0);
    }

    Proc_Break(proc);
}

asm(".global sub_080675A0\n.thumb_set sub_080675A0, IntroT0_IDLE_080675A1\n");
