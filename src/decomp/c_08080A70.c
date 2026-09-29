#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080A70.
 * sub_08080A70 @ 0x08080A70, sub_08080AF8 @ 0x08080AF8
 */

#include "proc.h"
/* SuperCoPowerName_HoldLoop @ 0x08080A70, 136 bytes, THUMB. Matched.
 *
 * Near-twin of CoPowerNameBanner_HoldLoop (src/decomp/c_0807FC70.c): the same Proc routine
 * over the same gUnknown_030059A0 sprite column table. Four differences and
 * nothing else -- the magic handed to sub_08043C28 (0x11CA vs 0x41CA), the
 * sprite y (a gUnknown_030058D0 base instead of a flat +8), the OAM word
 * passed straight instead of OR-ed with 0x3000, and the frame-counter bound
 * (<< 3 with no bias instead of << 4 + 0x38).
 *
 * gUnknown_030058D0 is the LEFT operand of the y sum: that is what puts its
 * address in the callee-saved r7 across the loop while gUnknown_030059A0's is
 * re-materialised each pass -- of two address constants used equally often,
 * the pseudo created FIRST wins the register (docs/agbcc-codegen.md).
 *
 * proc->unk58 is RE-READ at the bottom of every iteration: it is the loop
 * condition and PutSprite may write it, so it is not a hoistable invariant.
 */
struct Unk08080A70
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[0x58 - 0x4e];
    /* 0x58 */ int unk58;
};
/* SuperCoPowerName_BarsExtendLoop @ 0x08080AF8, 248 bytes, THUMB. Matched.
 *
 * The long-form sibling of SuperCoPowerName_HoldLoop (src/decomp/c_08080A70.c): same
 * sub_08043C28 setup with the same 0x11CA, and its second loop is
 * SuperCoPowerName_HoldLoop's sprite-column loop unchanged, down to the register
 * assignment. What is added around it is a 0x1E-step Interpolate ramp into
 * proc->unk2c while the frame counter is still <= 0x10, and a pair of
 * PutSpriteExt wipes per ramp step -- one closing in from 0xE8, one opening
 * out from 0.
 *
 * `mov sb, r4` parks &proc->unk4c across both loops because the tail reads it
 * twice; the ROM's `ldrh` and `ldrsh` of the same halfword before the branch
 * are the increment's operand and the comparison's, hoisted together.
 *
 * gUnknown_0848B688 is declared `u8 []` in unknown-globals.h and PutSpriteExt
 * takes `u16 *`, hence the cast -- it is byte-neutral and the shared
 * declaration is left alone.
 */
struct Unk08080AF8
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x4c - 0x30];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
    /* 0x50 */ u8 filler_50[0x58 - 0x50];
    /* 0x58 */ int unk58;
};

void SuperCoPowerName_HoldLoop(struct Unk08080A70 *proc)
{
    int i;

    sub_08043C28(0xa8, 0xa0, 0x11ca, 2, 1);

    for (i = 0; i < proc->unk58; i++)
        PutSprite(0, gUnknown_030058D0 + gUnknown_030059A0[i], 0x40, gUnknown_0848B6E6, 0x30a + i * 8);

    if (proc->unk4c < (10 - proc->unk58) * 8)
    {
        proc->unk4c++;
    }
    else
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_08080A70\n.thumb_set sub_08080A70, SuperCoPowerName_HoldLoop\n");

void SuperCoPowerName_BarsExtendLoop(struct Unk08080AF8 *proc)
{
    int i;

    if (proc->unk4c <= 0x10)
        proc->unk2c = Interpolate(1, 0, 0x1e, proc->unk4c, 0x10);

    sub_08043C28(0xa8, 0xa0, 0x11ca, 2, 1);

    for (i = 0; i < proc->unk2c; i++)
    {
        PutSpriteExt(0, 0xe8 - i * 8, 0x440, (u16 *)gUnknown_0848B688, 0x3ba);
        PutSpriteExt(0, i * 8, 0x458, (u16 *)gUnknown_0848B688, 0x3ba);
    }

    for (i = 0; i < proc->unk58; i++)
        PutSprite(0, gUnknown_030058D0 + gUnknown_030059A0[i], 0x40, gUnknown_0848B6E6, 0x30a + i * 8);

    if (proc->unk4c <= 0x4f)
    {
        proc->unk4c++;
    }
    else
    {
        proc->unk4c = 0;
        proc->unk4e = 0x1e;
        Proc_Break(proc);
    }
}
asm(".global sub_08080AF8\n.thumb_set sub_08080AF8, SuperCoPowerName_BarsExtendLoop\n");
