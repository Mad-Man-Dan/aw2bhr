#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08053434.
 * sub_08053434 @ 0x08053434, sub_080534A0 @ 0x080534A0
 */

/* A leaf -- no push, `bx lr`. gUnknown_030045A4 and gUnknown_03004510 are each
 * a PAIR filled from the two rows of gUnknown_03004580, and both source rows
 * are read at index 5 (`[r1,#0xa]` and `[r1,#0x1a]`, i.e. row 0 and row 1 of a
 * 16-byte row). */
void BattleAnimSceneBody_Init(void)
{
    gUnknown_03004570 = gUnknown_03001FBC;
    gUnknown_03004530 = 0;
    gUnknown_03004544 = 0;
    gUnknown_030045A4[0] = gUnknown_085537F4[gUnknown_03004580[0][5]];
    gUnknown_030045A4[1] = gUnknown_085537F4[gUnknown_03004580[1][5]];
    gUnknown_03004510[0] = gUnknown_0855380A[gUnknown_03004580[0][5]];
    gUnknown_03004510[1] = gUnknown_0855380A[gUnknown_03004580[1][5]];
}
asm(".global sub_08053434\n.thumb_set sub_08053434, BattleAnimSceneBody_Init\n");

/* The `blo` on the guard is agbcc choosing an unsigned branch for two
 * zero-extended u16 operands, not evidence of an unsigned type -- both sides
 * promote to int and the values cannot be negative, so either branch is
 * correct and combine picks this one. */
void BattleAnimSceneBody_Loop(void)
{
    u16 t;

    t = gUnknown_0300450C;

    if (gUnknown_03004544 >= gUnknown_030045A4[t])
    {
        PlayMusicOrSfx(gUnknown_085643B0[gUnknown_03004580[t][3]]
                                      [gUnknown_03004520]
                                      [gUnknown_03004530 & 1]);
        gUnknown_03004530++;
        gUnknown_03004544 = 0;
    }

    StepRepeatedShotAndHitSfx(0);
    StepRepeatedShotAndHitSfx(1);
}
asm(".global sub_080534A0\n.thumb_set sub_080534A0, BattleAnimSceneBody_Loop\n");
