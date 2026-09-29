#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080531D4.
 * sub_080531D4 @ 0x080531D4, sub_08053270 @ 0x08053270
 */

#include "hardware.h"
#include "proc.h"

/* The DISPCNT shadow is zeroed WHOLE, `strh` at gDispIo+0, so it is the
 * `*(u16 *)&gDispIo.disp_ct` idiom of src/decomp/c_080123EC.c and not a run of
 * bitfield stores.
 *
 * The bit0/bit6 guard is NESTED, not `&&`. Spelled `bit0 && !bit6` agbcc merges
 * the two 1-bit tests on the same byte into one `movs r0,#0x41; ands; cmp #1`
 * -- three instructions where the ROM has six. Nesting keeps them apart, and
 * that in turn is what makes the constant 1 live across the whole function in
 * r5 (shared with the bit0 test at the top), which is the difference between
 * `push {r4,lr}` and the ROM's `push {r4,r5,lr}`. */
void BattleAnimScene_Init(void)
{
    InstallQueuedSpritesFrameCallbacks();
    sub_080546F0();

    if (!gUnknown_03004504.bit0)
        *(u16 *)&gDispIo.disp_ct = 0;

    FlushDisplayRegisters();
    PlanBattleAnim();
    LoadBattleAnimScene();
    sub_080152C0((int)gUnknown_08553820, 0);
    gUnknown_0300453C = 0;
    gUnknown_0300451C = 0;
    SetUpBattleAnimDisplay();
    sub_0804BA4C();

    if (gUnknown_03004504.bit0)
    {
        if (!gUnknown_03004504.bit6)
            gUnknown_03004508 =
                gUnknown_085537EC[gUnknown_030045A0[gUnknown_0300450C]];
    }

    FlushDisplayRegisters();
}
asm(".global sub_080531D4\n.thumb_set sub_080531D4, BattleAnimScene_Init\n");

/* The counter bump appears TWICE in the ROM, once per arm, and that is the
 * source: written as one test with a combined predicate there would be a single
 * `ldrh; adds #1; strh`. Same shape as the doubled `bl StepBattleHud` in
 * src/decomp/c_080535E0.c, the neighbour this block is derived from. */
void BattleAnimScene_Loop(ProcPtr proc)
{
    RunBattleAnimStepHandler();
    StepBattleHudAndPanelScroll();
    RegisterDataMove(gUnknown_08551A04, (void *)0x06002800, 0x800);

    if (gUnknown_03004508 == 0x12c)
        Proc_Break(proc);

    StepPanelBounceBothSides();
    StepStageFlags();

    if (gUnknown_03004504.bit0)
    {
        if (gUnknown_03004504.unk02 != gUnknown_03004508)
            gUnknown_03004508 = gUnknown_03004508 + 1;
    }
    else
    {
        gUnknown_03004508 = gUnknown_03004508 + 1;
    }
}
asm(".global sub_08053270\n.thumb_set sub_08053270, BattleAnimScene_Loop\n");
