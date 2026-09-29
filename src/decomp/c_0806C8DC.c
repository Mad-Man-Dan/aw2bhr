#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C8DC.
 * sub_0806C8DC @ 0x0806C8DC
 */

#include "hardware.h"
#include "proc.h"
struct Unk6C8DCProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};

/* A 20-frame slide-out: interpolate a scroll offset from 0 down to -0xb0,
 * publish it to the three x-scroll shadows, drive the two window edges from its
 * negation, and on the last frame reset the counter, clear the map, turn window
 * 1 off and break.
 *
 * The three scroll shadows and their types come from src/decomp/c_0807898C.c,
 * whose data_refs are a superset of this function's -- two of the three are
 * `volatile u16` and the interpolated value stays in r4 across all three
 * stores, which is what a plain `strh` of an `int` gives.
 *
 * `movs r0, #0x41; rsbs` is -0x41 = ~0x40 on DISPCNT byte 1, i.e.
 * `win1_enable = 0` -- the same field-assignment reading c_0807898C.c records
 * for the ~0x20 / ~0x40 pair, not a hand-written mask.
 *
 * `-a` is negated once and reused for both window calls, and ClearCreditsPageArea
 * takes no arguments (its first instruction writes r0); the zero sitting in r0
 * at that `bl` is left over from the counter reset. */
void CreditsPageWipe_OutLoop(struct Unk6C8DCProc *proc)
{
    int a;

    a = Interpolate(1, 0, -0xb0, proc->unk58, 0x14);

    gUnknown_03001FF8 = a;
    gUnknown_03002B34 = a;
    gUnknown_030030A0 = a;

    SetCreditsWindow1Rect(0, 0, -a, 0xa0);
    SetCreditsWindow0Rect(-a, 0, -a + 0x40, 0xa0);

    if (proc->unk58 > 0x13)
    {
        proc->unk58 = 0;
        ClearCreditsPageArea();
        gDispIo.disp_ct.win1_enable = 0;
        Proc_Break(proc);
    }
    else
    {
        proc->unk58++;
    }
}
asm(".global sub_0806C8DC\n.thumb_set sub_0806C8DC, CreditsPageWipe_OutLoop\n");
