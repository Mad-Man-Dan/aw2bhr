#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067FB4.
 * sub_08067FB4 @ 0x08067FB4
 */

#include "hardware.h"
#include "proc.h"
/* Alpha-blend fade step driven off a proc counter at +0x40.
 * gUnknown_030030E0.bits.effect = 1 is BLDCNT's alpha mode -- the
 * `ldrb; movs #0x3f; ands; movs #0x40; orrs; strb` byte view of the
 * union (see the .bits vs .raw note in hardware.h). EVA/EVB are the
 * complementary pair summing to 0x10, and BLDY is parked at 0.
 * The three halfword shadows are volatile, which is why each store
 * re-loads its own pool word instead of walking one address chain. */
struct Unk08067FB4
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ u8 filler_38[0x40 - 0x38];
    /* 0x40 */ int unk40;
};

void IntroBlendFade_Loop(struct Unk08067FB4 *proc)
{
    int v;

    v = Interpolate(0, proc->unk2c, proc->unk30, proc->unk40, proc->unk34);
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = v;
    gUnknown_03002B28 = 0x10 - v;
    gUnknown_03001FFC = 0;
    proc->unk40++;
    if (proc->unk40 == proc->unk34)
        Proc_Break(proc);
}
asm(".global sub_08067FB4\n.thumb_set sub_08067FB4, IntroBlendFade_Loop\n");
