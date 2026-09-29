#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067A4C.
 * sub_08067A4C @ 0x08067A4C
 */

#include "hardware.h"
#include "proc.h"
/* Alpha-blend fade step that also drives SetBgScrollShadow channel 1 with a
 * second, independently interpolated value. The `lsls #0x10; lsrs #0x10`
 * on that value is SetBgScrollShadow's u16 parameter re-narrowing the s32
 * Interpolate result, not a cast in the source. The +0x38 limit and the
 * +0x3c counter are both cleared on the last frame, and the counter is
 * bumped unconditionally afterwards. */
struct Unk08067A4C
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x38 - 0x34];
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
};

void IntroSlidePanel_SlideIn(struct Unk08067A4C *proc)
{
    int a;
    int b;

    a = Interpolate(1, proc->unk30, 0, proc->unk3c, proc->unk38);
    b = Interpolate(0, 0, 0x10, proc->unk3c, proc->unk38);
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = b;
    gUnknown_03002B28 = 0x10 - b;
    gUnknown_03001FFC = 0;
    SetBgScrollShadow(1, a, 0);

    if (proc->unk3c == proc->unk38)
    {
        proc->unk38 = 0;
        proc->unk3c = 0;
        Proc_Break(proc);
    }

    proc->unk3c++;
}
asm(".global sub_08067A4C\n.thumb_set sub_08067A4C, IntroSlidePanel_SlideIn\n");
