#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801320C.
 * sub_0801320C @ 0x0801320C
 */

#include "hardware.h"
#include "proc.h"
/* Brighten fade step: gUnknown_030030E0.bits.effect = 2 (the `movs #0x80`
 * form of the byte-view store), EVA/EVB zeroed and BLDY ramped by
 * Interpolate. The duration lives at +0x64 as a SIGNED halfword -- the
 * `movs r1,#0; ldrsh r0,[r6,r1]` is THUMB's only sign-extending load form,
 * and proc+0x64 stays in a callee-saved register because it is read three
 * times. A zero duration breaks the proc immediately.
 *
 * The counter bump is INSIDE the assignment, not a statement before it:
 * gUnknown_03001FFC's address pseudo is created at the head of the
 * statement that stores to it, so the ROM's `ldr r4,=gUnknown_03001FFC`
 * sits ahead of the `ldr/adds/str` of proc->unk54. Written as a separate
 * `proc->unk54++;` statement the ldr lands after the bump -- 8 bytes,
 * same instruction count. */
struct Unk0801320C
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ int unk54;
    /* 0x58 */ u8 filler_58[0x64 - 0x58];
    /* 0x64 */ s16 unk64;
};

void WhiteFlash_FadeIn(struct Unk0801320C *proc)
{
    if (proc->unk64 == 0)
    {
        Proc_Break(proc);
    }
    else
    {
        gUnknown_030030E0.bits.effect = 2;
        gUnknown_03002020 = 0;
        gUnknown_03002B28 = 0;
        gUnknown_03001FFC = Interpolate(0, 0, 0x10, ++proc->unk54, proc->unk64);

        if (proc->unk54 == proc->unk64)
            Proc_Break(proc);
    }
}
asm(".global sub_0801320C\n.thumb_set sub_0801320C, WhiteFlash_FadeIn\n");
