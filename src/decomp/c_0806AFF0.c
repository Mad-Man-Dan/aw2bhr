#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AFF0.
 * sub_0806AFF0 @ 0x0806AFF0
 */

#include "hardware.h"
/* Same blend-shadow shape as CreditsResultFade_Init. Here nothing separates the two
 * target-group writes, so agbcc forwards the first store into the second read
 * and both collapse into one `strh` -- which is exactly why the scalar `u16 *`
 * cast is load-bearing: through `.raw` the merge picks up the live zero left
 * by the three scalar stores above and emits an extra `orr`. */

struct Unk806AFF0
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void CreditsListFade_Init(struct Unk806AFF0 *proc)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;

    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x02;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x800;
    gUnknown_030030E0.bits.target2_enable_bd = 1;

    proc->unk58 = 0xE;
}
asm(".global sub_0806AFF0\n.thumb_set sub_0806AFF0, CreditsListFade_Init\n");
