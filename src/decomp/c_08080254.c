#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080254.
 * sub_08080254 @ 0x08080254
 */

#include "hardware.h"
#include "proc.h"
/* Two-axis scroll ramp: BG0's x-scroll shadow gets a 0x100 -> 0 slide added to
 * a 0 -> -0x100 one and BG1's the mirror pair, both over a 0x30-frame counter
 * at +0x4c. When BG0's shadow reaches 0 the two BG-control shadows drop their
 * screen-size field. Sibling of the Interpolate fade procs (sub_0806EA28). */
struct Unk08080254
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
};

void SuperCoPowerScene_BgSlideInLoop(struct Unk08080254 *proc)
{
    gUnknown_03001FF8 = Interpolate(1, 0x100, 0, proc->unk4c, 0x30)
                      + Interpolate(0, 0, -0x100, proc->unk4c, 0x30);
    gUnknown_030030A0 = Interpolate(1, -0x100, 0, proc->unk4c, 0x30)
                      + Interpolate(0, 0, 0x100, proc->unk4c, 0x30);

    if ((s16)gUnknown_03001FF8 <= 0)
    {
        gUnknown_03002B6C.bits.size = 0;
        gUnknown_030030B4.bits.size = 0;
    }

    if (proc->unk4c <= 0x2f)
    {
        proc->unk4c++;
    }
    else
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_08080254\n.thumb_set sub_08080254, SuperCoPowerScene_BgSlideInLoop\n");
