#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071E40.
 * sub_08071E40 @ 0x08071E40, sub_08071E80 @ 0x08071E80, sub_08071EB8 @ 0x08071EB8
 */

#include "hardware.h"
#include "proc.h"
/* FadeToCommon_OnLoopUnused's fade-DOWN twin: subtract instead of accumulate, clamp at 0
 * instead of 0x100, and end when BLDY has reached 0 instead of 0x10.
 *
 * The floor test is `lsls #0x10; cmp #0; bgt`, which is the sign test on the
 * s16 -- no `asrs` is needed when the only question is the sign bit. */
struct Unk8071E40
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
};

void FadeFromCommon_OnLoopUnused(struct Unk8071E40 *proc)
{
    if (gUnknown_03001FFC == 0)
    {
        Proc_End(proc);
        return;
    }

    proc->unk66 -= proc->unk64;

    if (proc->unk66 <= 0)
        proc->unk66 = 0;

    gUnknown_03001FFC = proc->unk66 >> 4;
}
asm(".global sub_08071E40\n.thumb_set sub_08071E40, FadeFromCommon_OnLoopUnused\n");

/* Opens the fade through FadeToBlack_OnInitUnused (src/decomp/c_08071CF4.c), forwarding
 * its own proc untouched, then overrides the blend effect to 2 and zeroes the
 * three coefficient shadows.
 *
 * `.bits.effect = 2` is `ldrb; ands #0x3f; orrs #0x80; strb` -- the AND is
 * present because 2 does not fill the 2-bit field. ForceScreenBlack's `= 3` fills
 * it and agbcc drops the AND, which is the discriminator hardware.h's note on
 * this field already records. */
void FadeToWhite_OnInitUnused(struct Unk08071CF4 *proc)
{
    FadeToBlack_OnInitUnused(proc);

    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;
}
asm(".global sub_08071E80\n.thumb_set sub_08071E80, FadeToWhite_OnInitUnused\n");

/* FadeToWhite_OnInitUnused's twin over the other opener: FadeFromBlack_OnInitUnused instead of
 * FadeToBlack_OnInitUnused, and BLDY seeded at 0x10 instead of 0 -- which is the fade-out
 * end state, matching FadeFromBlack_OnInitUnused's own 0x10/0x100 seeding. Everything else
 * is identical. */
void FadeFromWhite_OnInitUnused(struct Unk08071DB4 *proc)
{
    FadeFromBlack_OnInitUnused(proc);

    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0x10;
}
asm(".global sub_08071EB8\n.thumb_set sub_08071EB8, FadeFromWhite_OnInitUnused\n");
