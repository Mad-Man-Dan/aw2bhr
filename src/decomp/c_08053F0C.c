#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08053F0C.
 * sub_08053F0C @ 0x08053F0C, sub_08053F50 @ 0x08053F50, sub_08053F90 @ 0x08053F90
 */

/* Runs the current cutscene step, overriding it with entry [2] of the handler
 * table while the 0x03004504 flags say "skipping".
 *
 * The two flag tests must be NESTED IFS, not `bit0 && !bit6`. Both bits live in
 * the same byte, so agbcc's fold_truthop merges the `&&` into a single
 * `movs r0,#0x41; ands; cmp r0,#1` -- one test where the ROM has two, with
 * their own `movs #1` / `movs #0x40` masks and their own branches.
 *
 * `bl _call_via_r2` is a NULLARY indirect call here, so the r2 index does not
 * count arguments: the pointer is loaded before the flag test, and r0 and r1
 * are the registers that test needs, so the allocator had nowhere lower to put
 * it. include/unknown-globals.h records that with the table's declaration. */
void RunBattleAnimStepHandler(void)
{
    void (*f)(void) = gUnknown_08553734[gUnknown_030045A0[gUnknown_0300450C]];

    if (gUnknown_03004504.bit0)
    {
        if (!gUnknown_03004504.bit6)
            f = gUnknown_08553734[2];
    }

    f();
}
asm(".global sub_08053F0C\n.thumb_set sub_08053F0C, RunBattleAnimStepHandler\n");

/* Steps both channels of the cutscene player. gUnknown_0300450C is bound in r4
 * and RELOADED for the second call, which is what two separate reads of a
 * plain global give. */
void BattleAnimStep_Full(void)
{
    StepShotFireTimeline(gUnknown_0300450C);
    StepShotReturnTimeline(gUnknown_0300450C);
    StepThirdEffectTimeline(0);
    StepThirdEffectTimeline(1);
    StepHitTimeline(0);
    StepHitTimeline(1);
    StepDeathTimeline(0);
    StepDeathTimeline(1);
}
asm(".global sub_08053F50\n.thumb_set sub_08053F50, BattleAnimStep_Full\n");

/* BattleAnimStep_Full without the two script-advancing calls -- the same six
 * per-channel steps. */
void BattleAnimStep_NoShots(void)
{
    StepThirdEffectTimeline(0);
    StepThirdEffectTimeline(1);
    StepHitTimeline(0);
    StepHitTimeline(1);
    StepDeathTimeline(0);
    StepDeathTimeline(1);
}
asm(".global sub_08053F90\n.thumb_set sub_08053F90, BattleAnimStep_NoShots\n");
