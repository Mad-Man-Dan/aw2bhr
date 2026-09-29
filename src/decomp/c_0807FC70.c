#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807FC70.
 * sub_0807FC70 @ 0x0807FC70
 */

#include "proc.h"
/* CoPowerNameBanner_HoldLoop @ 0x0807FC70, 136 bytes, THUMB. Matched.
 *
 * Near-twin of SuperCoPowerName_HoldLoop: same Proc routine over the same
 * gUnknown_030059A0 sprite column table, differing only in the magic passed
 * to sub_08043C28, the sprite y expression, and the two constants in the
 * frame-counter bound.
 *
 * proc->unk58 is RE-READ at the bottom of every iteration because it is the
 * loop condition and PutSprite may write it -- it is not a hoistable
 * invariant and must not be cached in a local.
 *
 * `movs r0, #0; ldrsh r2, [r3, r0]` is the ldrsh-needs-a-register-offset
 * idiom: ldrsh has no immediate-offset form, so a zero register is
 * materialised. It is the signed read of the s16 field at +0x4c, not a cast.
 */
struct Unk0807FC70
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[0x58 - 0x4e];
    /* 0x58 */ int unk58;
};

void CoPowerNameBanner_HoldLoop(struct Unk0807FC70 *proc)
{
    int i;

    sub_08043C28(0xa8, 0xa0, 0x41ca, 2, 1);

    for (i = 0; i < proc->unk58; i++)
        PutSprite(0, gUnknown_030059A0[i] + 8, 0x40, gUnknown_0848B6E6, 0x3000 | (0x30a + i * 8));

    if (proc->unk4c < (10 - proc->unk58) * 16 + 0x38)
    {
        proc->unk4c++;
    }
    else
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_0807FC70\n.thumb_set sub_0807FC70, CoPowerNameBanner_HoldLoop\n");
