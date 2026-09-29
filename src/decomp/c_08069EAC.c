#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08069EAC.
 * sub_08069EAC @ 0x08069EAC
 */

#include "proc.h"
/* A two-sided slide-in proc body: one branch drives the pair of sprites in from
 * the left, the mirrored branch from the right, and the proc ends itself on the
 * frame its own duration counter reaches.
 *
 * unk2c/unk2e/unk30 are `u16` with explicit `(s16)` casts at the four bounds
 * tests, not `s16` members: the call sites emit a bare `ldrh` into
 * SetBgScrollShadow, which an `s16` member could not do -- it would have to
 * sign-extend and then re-narrow. The `lsl #16; asr #16` at each test is the
 * cast, applied to the value the `strh` just wrote rather than to a reload,
 * which is why no `ldrsh` appears anywhere. (This argument used to be stated
 * as "SetBgScrollShadow's third parameter is u32". That was wrong -- wave 21
 * matched SetBgScrollShadow itself and all three of its parameters are u16. The
 * conclusion is unchanged and the file still matches: a bare `ldrh` reaching
 * a u16 parameter needs no conversion either.)
 *
 * The `movs r5, #0` in the second branch and its absence in the first are one
 * source: `proc->unk38` is loaded for the `if` and the zero it compared against
 * is still live to be stored into unk2c/unk2e on the taken side. */
struct Unk08069EAC
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 filler_32[0x06];
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
};

void IntroCoReveal_Loop(struct Unk08069EAC *proc)
{
    if (proc->unk38 == 0)
    {
        proc->unk2a = Interpolate(0, 0xE8, -0x30, proc->unk40, proc->unk3c);
        proc->unk2e -= 2;
        if ((s16)proc->unk2e <= -0x28)
            proc->unk2e = 0;
        SetBgScrollShadow(2, proc->unk2a, proc->unk2e);
        proc->unk2c -= 8;
        proc->unk30 += 4;
        if ((s16)proc->unk30 <= 0x9F)
        {
            if ((s16)proc->unk2c <= -0x50)
                proc->unk2c = 0;
            SetBgScrollShadow(3, proc->unk2c, proc->unk30);
            if (proc->unk40 == 0x1E)
                SetIntroSplitWindow(0);
        }
    }
    else
    {
        proc->unk2a = Interpolate(0, -0x30, 0x108, proc->unk40, proc->unk3c);
        proc->unk2e += 2;
        if ((s16)proc->unk2e > 0x27)
            proc->unk2e = 0;
        SetBgScrollShadow(2, proc->unk2a, proc->unk2e);
        proc->unk2c += 8;
        proc->unk30 -= 4;
        if ((s16)proc->unk30 > -0x68)
        {
            if ((s16)proc->unk2c > 0x4F)
                proc->unk2c = 0;
            SetBgScrollShadow(3, proc->unk2c, proc->unk30);
            if (proc->unk40 == 0x1E)
                SetIntroSplitWindow(1);
        }
    }
    proc->unk40++;
    if (proc->unk40 == proc->unk3c)
        Proc_Break(proc);
}
asm(".global sub_08069EAC\n.thumb_set sub_08069EAC, IntroCoReveal_Loop\n");
