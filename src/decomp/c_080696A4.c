#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080696A4.
 * sub_080696A4 @ 0x080696A4
 */

#include "proc.h"
/* Three near-identical scroll steps on the proc PauseIntroParallaxScroll/ResumeIntroParallaxScroll
 * gate with +0x36: each advances a position pair, wraps one of the two on a
 * bound, then pushes both to SetBgScrollShadow halved.
 *
 * The members are u16 with explicit (s16) casts at the bounds tests and at the
 * halving, not s16 members -- same reading as src/decomp/c_08069EAC.c. Every
 * `lsl #0x10; asr #0x11` pair is `(s16)x >> 1`; the `lsl #0x10; lsr #0x10`
 * that follows is SetBgScrollShadow's u16 parameter, not a second cast. */
struct Unk696A4Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u8 unk36;
};

void IntroParallaxScroll_Loop(struct Unk696A4Proc *proc)
{
    if (proc->unk36 != 0)
    {
        proc->unk2a--;
        proc->unk30 += 4;
        if ((s16)proc->unk30 > 0x8f)
            proc->unk30 = 0;
        SetBgScrollShadow(1, (s16)proc->unk2a >> 1, (s16)proc->unk30 >> 1);
        proc->unk2c++;
        proc->unk32 -= 2;
        if ((s16)proc->unk32 <= -0x50)
            proc->unk32 = 0;
        SetBgScrollShadow(2, (s16)proc->unk2c >> 1, (s16)proc->unk32 >> 1);
        proc->unk2e += 4;
        proc->unk34 += 2;
        if ((s16)proc->unk2e > 0x9f)
            proc->unk2e = 0;
        SetBgScrollShadow(3, (s16)proc->unk2e >> 1, (s16)proc->unk34 >> 1);
    }
}
asm(".global sub_080696A4\n.thumb_set sub_080696A4, IntroParallaxScroll_Loop\n");
