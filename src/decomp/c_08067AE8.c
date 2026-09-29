#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067AE8.
 * sub_08067AE8 @ 0x08067AE8
 */

#include "hardware.h"
#include "proc.h"
/* Alpha-blend fade step, sibling of IntroSlidePanel_SlideIn. Differences: the first
 * Interpolate runs mode 4 from 0 down to -proc->unk30 (the `rsbs r2,r2,#0`),
 * the second ramps 0x10 -> 0, and the last frame does not clear the
 * counter pair before breaking. */
struct Unk08067AE8
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x38 - 0x34];
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
};

void IntroSlidePanel_SlideOut(struct Unk08067AE8 *proc)
{
    int a;
    int b;

    a = Interpolate(4, 0, -proc->unk30, proc->unk3c, proc->unk38);
    b = Interpolate(4, 0x10, 0, proc->unk3c, proc->unk38);
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = b;
    gUnknown_03002B28 = 0x10 - b;
    gUnknown_03001FFC = 0;
    SetBgScrollShadow(1, a, 0);

    if (proc->unk3c == proc->unk38)
        Proc_Break(proc);

    proc->unk3c++;
}
asm(".global sub_08067AE8\n.thumb_set sub_08067AE8, IntroSlidePanel_SlideOut\n");
