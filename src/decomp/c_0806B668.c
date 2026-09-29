#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B668.
 * sub_0806B668 @ 0x0806B668
 */

#include "hardware.h"
#include "proc.h"
/* Alpha-blend fade step on a fixed 0x3c-frame ramp counted at +0x5c.
 * Two Interpolate results: the first splits across EVA/EVB as
 * ceil/floor of a halving -- `(a >> 1) + (a & 1)` and `a >> 1`, which
 * sum back to a -- and the second drives SetBgScrollShadow channel 2 twice
 * over, once halved. `b >> 1` reaching a u16 parameter is the single
 * `lsls #0xf; lsrs #0x10` pair (net right shift of one plus the u16
 * truncation), not a shift followed by a mask. */
struct Unk0806B668
{
    /* 0x00 */ u8 filler_00[0x5c];
    /* 0x5c */ int unk5c;
};

void CreditsEpilogue_FadeOut(struct Unk0806B668 *proc)
{
    int a;
    int b;

    a = Interpolate(0, 0xc, 0, proc->unk5c, 0x3c);
    b = Interpolate(0, -2, -8, proc->unk5c, 0x3c);
    SetBgScrollShadow(3, 0, 0);
    SetBgScrollShadow(2, b >> 1, (u16)b);
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = (a >> 1) + (a & 1);
    gUnknown_03002B28 = a >> 1;
    gUnknown_03001FFC = 0;

    if (proc->unk5c > 0x3b)
        Proc_Break(proc);
    else
        proc->unk5c++;
}
asm(".global sub_0806B668\n.thumb_set sub_0806B668, CreditsEpilogue_FadeOut\n");
