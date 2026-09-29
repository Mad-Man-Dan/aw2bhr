#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A534.
 * sub_0806A534 @ 0x0806A534, sub_0806A578 @ 0x0806A578
 */

#include "hardware.h"
/* `proc->unk2c` is the object GetIntroSceneDuration handed back; +0x5a is reached by
 * `adds r0, #0x5a` followed by a register-offset `ldrsh` at displacement zero,
 * which is a plain s16 field and not an array index. The `cmp #0; bge; adds
 * #3; asrs #2` chain is a signed divide by 4. */
struct Unk806A578Obj
{
    /* 0x00 */ u8 filler_00[0x5a];
    /* 0x5a */ s16 unk5a;
};
struct Unk806A578
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ struct Unk806A578Obj *unk2c;
};

/* An H-blank callback registered by MeteorImpactGlow_Loop. Both `movs #K; lsls #n`
 * pairs are plain constants -- 0x82 << 1 is 0x104 and 0x80 << 5 is 0x1000 --
 * not shifts in the source; Thumb immediates stop at 0xFF, so that is how
 * agbcc builds every larger literal. */
void MeteorImpactGlow_DrawCallback(void)
{
    PutOamHi((gUnknown_0300060C + 0x206) & 0x1FF,
                 (gUnknown_0202F210 + 0x104) & 0xFF,
                 gUnknown_085815C8, 0x1000);
}
asm(".global sub_0806A534\n.thumb_set sub_0806A534, MeteorImpactGlow_DrawCallback\n");

void MeteorImpactGlow_Loop(struct Unk806A578 *proc)
{
    int v = proc->unk2c->unk5a / 4;

    if (v <= 4)
    {
        gUnknown_03002020 = 4 - v;
        gUnknown_03002B28 = v + 0xC;
        RunOrQueueDrawCallback((void *)MeteorImpactGlow_DrawCallback, 0x7F);
    }
}
asm(".global sub_0806A578\n.thumb_set sub_0806A578, MeteorImpactGlow_Loop\n");
