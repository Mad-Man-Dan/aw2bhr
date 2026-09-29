#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080345C8.
 * RunMapStateMachine @ 0x080345C8
 */

/* The gUnknown_030032D8 state machine's per-frame tick.
 *
 * `state` is the `-fforce-addr` workaround, not the original source: the ROM's
 * pool word is agbcc's own address-constant for &gUnknown_030032D8, parked at
 * 0x08090D88 because the address is live across the switch's calls. Writing
 * the honest `gUnknown_030032D8` gives byte-identical .text but relocates
 * against this unit's own .rodata, which the .text-only split cannot place.
 * See the gUnknown_08090D88 note in unknown-globals.h. Taking the address into
 * a LOCAL is what keeps -fforce-addr from adding a second .rodata level;
 * dereferencing `gUnknown_08090D88` in place costs one extra `ldr`.
 *
 * `idle` exists for the same reason: the pool load has to be generated after
 * the call so the scheduler can drop it in the slot behind the `bl`.
 *
 * `case 0: break;` is not decoration -- it is what makes the jump table start
 * at 0 and run 21 entries. Without it agbcc bases the table at 1 and adds a
 * `subs r0, #1`. The 19/16/18 order is likewise the source's: GCC lays case
 * bodies out in source order, and the ROM has 19's before 16's.
 */
void RunMapStateMachine(void)
{
    u16 *const *state;
    int idle;

    idle = GetMapLock();
    state = &gUnknown_08090D88;

    if (idle == 0 && gUnknown_030040E4 == 0)
    {
        switch (**state)
        {
        case 0:  break;
        case 1:  MapState_CheckTurnLimit(); break;
        case 2:  MapState_PrepareTurnHandover(); break;
        case 3:  MapState_TurnHandoverPrompt(); break;
        case 4:  MapState_WaitForCampaignIntro(); break;
        case 5:  MapState_PlayTurnMusic(); break;
        case 6:  sub_08034DF8(); break;
        case 7:  MapState_StartDayStartScreen(); break;
        case 8:  MapState_StartFuelUpkeep(); break;
        case 9:  MapState_StartTurnStartSupply(); break;
        case 10: sub_08034CB8(); break;
        case 11: MapState_StartInventionTurnScript(); break;
        case 12: MapState_DispatchTurnByController(); break;
        case 13: RunMapCursorState(); break;
        case 14: AiDriverStep(); break;
        case 19: MapState_RemoteTurn(); break;
        case 16: MapState_RunParkedWinLossCheck(); break;
        case 18: MapState_EndOfGame(); break;
        case 20: MapState_ResumeCursorAfterCommand(); return;
        }
    }

    if (**state == 14 || gUnknown_030044DC == 14)
    {
        if (FindSlotScript((s32)gUnknown_0849A00C) != -1 || GetMapLock() == 0)
            SetInfoBoxMode(3);
    }
}

asm(".global sub_080345C8\n.thumb_set sub_080345C8, RunMapStateMachine\n");
