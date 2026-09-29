#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080132B0.
 * sub_080132B0 @ 0x080132B0
 */

#include "hardware.h"
#include "proc.h"
/* Brighten fade step, the reverse ramp of WhiteFlash_FadeIn: Interpolate's
 * endpoints are swapped (0x10 -> 0 instead of 0 -> 0x10), the counter is
 * at +0x5c and the signed-halfword duration at +0x68. The counter bump
 * is inside the assignment for the same reason as in WhiteFlash_FadeIn. */
struct Unk080132B0
{
    /* 0x00 */ u8 filler_00[0x5c];
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 filler_60[0x68 - 0x60];
    /* 0x68 */ s16 unk68;
};

void WhiteFlash_FadeOut(struct Unk080132B0 *proc)
{
    if (proc->unk68 == 0)
    {
        Proc_Break(proc);
    }
    else
    {
        gUnknown_030030E0.bits.effect = 2;
        gUnknown_03002020 = 0;
        gUnknown_03002B28 = 0;
        gUnknown_03001FFC = Interpolate(0, 0x10, 0, ++proc->unk5c, proc->unk68);

        if (proc->unk5c == proc->unk68)
            Proc_Break(proc);
    }
}
asm(".global sub_080132B0\n.thumb_set sub_080132B0, WhiteFlash_FadeOut\n");
