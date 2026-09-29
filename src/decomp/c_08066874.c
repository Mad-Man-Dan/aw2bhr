#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066874.
 * sub_08066874 @ 0x08066874
 */

#include "hardware.h"

/* MATCHED wave 90 (W90-C). The 93.9% park was a +2/+2/-4 cancellation around
 * one fact: after `bl ClearCallbackOfSlotsRunningScript` the ROM re-derives &gUnknown_08580934 from
 * its -fforce-addr word (`mov r1,r8; ldr r0,[r1]`), while a draft with the arm
 * written twice keeps the address in callee-saved r5. cse.c does not
 * invalidate an unchanging `.rodata` MEM at a call, so inside one extended
 * basic block the address from the dispatch test is reused after the call.
 * Written twice, the arm is fall-through code in that block. The ROM's arm
 * starts a new CSE block, so it follows a label with uses: it is written ONCE
 * and reached by two gotos.
 * The gotos are spelled out to keep the ROM's layout, `bne call; b arm;
 * <cnt = 0 break block>; call:`. jump1 folds `bne L1; b L2; L1:` into
 * `beq L2` when L1 directly follows the `b`. Here the loop's break block sits
 * between them in the source, as loop.c's exit-block move put it in the ROM.
 * `key = &keys[i]` in the second loop is still needed for the pointer giv.
 * Needs "rodata": ["0x0816E150"] (the force-addr word for gUnknown_08580934). */

void MatchSetupHandleTeamStageInputLink(void)
{
    int i;
    int cnt;
    struct KeySt *key;

    cnt = 0;
    gUnknown_08580934->unk32 = gUnknown_08580934->unk25;

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        if (((struct KeySt *)&gUnknown_03002040)[i].pressed & 2)
            cnt++;

        if ((s8)gUnknown_08580934->unk70[i] == -1)
            goto brk;
    }

dispatch:
    if (gUnknown_08580934->unk08 == 2)
    {
        if (gUnknown_08580934->unk31 != 1)
            goto call;
        goto arm;
    brk:
        cnt = 0;
        goto dispatch;
    call:
        sub_080152EC(gUnknown_08580D54, 4);
        return;
    }

    if (cnt != 0)
    {
    arm:
        ClearCallbackOfSlotsRunningScript(gUnknown_08580D0C);
        gUnknown_08580934->unk26 = 0;
        ForEachSlotRunningScript(gUnknown_08580AF0, sub_08066200);
        ForEachSlotRunningScript(gUnknown_08580B90, sub_08066200);
        ForEachSlotRunningScript(gUnknown_08580BC8, sub_08066200);
        ForEachSlotRunningScript(gUnknown_08580A38, TeamBadge_StartExitDown);
        ForEachSlotRunningScript(gUnknown_08580A08, TeamBadge_StartExitDown);
        return;
    }

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        if ((s8)gUnknown_08580934->unk70[i] == 0)
        {
            key = &((struct KeySt *)&gUnknown_03002040)[i];
            MatchSetupCycleTeam(key->repeated, i, i == gUnknown_08580934->unk25);
        }
    }

    MatchSetupHandleBadgeReadyKeys();

    if ((s8)gUnknown_08580934->unk70[gUnknown_08580934->unk25] == 0)
        MatchSetupDrawTeamArrows();

    for (i = 0; i < gUnknown_08580934->unk08; i++)
        if ((s8)gUnknown_08580934->unk70[i] == 0)
            break;

    if (i == gUnknown_08580934->unk08)
        sub_080152EC(gUnknown_08580D54, 4);
}
asm(".global sub_08066874\n.thumb_set sub_08066874, MatchSetupHandleTeamStageInputLink\n");
