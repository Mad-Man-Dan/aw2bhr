#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C024.
 * sub_0806C024 @ 0x0806C024
 */

#include "proc.h"
#include "hardware.h"
struct Unk6C024Proc
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ u32 unk30;
};

/* ONE bitfield write, not two. `movs r0, #0x41; rsbs r0, r0, #0` is -0x41 ==
 * 0xffffffbf, i.e. `& ~0x40` on BYTE 1 of gDispIo -- bit 6 of that byte, which
 * is DISPCNT bit 14, win1_enable. It is not `& ~0x41`: the 0x41 is what
 * store_fixed_bit_field negates, not the mask itself. Clearing bg0_enable as
 * well costs a second `movs #2; rsbs; ands` chain in front (measured).
 *
 * The counter is UNSIGNED: `bls` after `cmp r0, #0x20`. */
void CreditsPage_RevealLoop(struct Unk6C024Proc *proc)
{
    SetCreditsWindow1Rect(Interpolate(0, 0x48, 0xF0, proc->unk30, 0x20), 0, 0xF0, 0xA0);

    proc->unk30++;

    if (proc->unk30 > 0x20)
    {
        gDispIo.disp_ct.win1_enable = 0;

        Proc_Break(proc);
    }
}
asm(".global sub_0806C024\n.thumb_set sub_0806C024, CreditsPage_RevealLoop\n");
