#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018F34.
 * sub_08018F34 @ 0x08018F34, sub_08018F74 @ 0x08018F74, sub_08018FB4 @ 0x08018FB4
 */

/* EventOp_JumpIfCallTrue with IsCampaignCompletionFlagSet in place of the node's own predicate; same
 * inverted arms and the same EventOp_Jump re-narrow. */
s16 EventOp_JumpIfCompletionFlagSet(s16 a)
{
    if (IsCampaignCompletionFlagSet((s16)gUnknown_0200C528[a].unk04->unk08) != 0)
        return EventOp_Jump(a);
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}
asm(".global sub_08018F34\n.thumb_set sub_08018F34, EventOp_JumpIfCompletionFlagSet\n");

/* EventOp_JumpIfCompletionFlagSet with the predicate inverted. */
s16 EventOp_JumpIfCompletionFlagClear(s16 a)
{
    if (IsCampaignCompletionFlagSet((s16)gUnknown_0200C528[a].unk04->unk08) == 0)
        return EventOp_Jump(a);
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}
asm(".global sub_08018F74\n.thumb_set sub_08018F74, EventOp_JumpIfCompletionFlagClear\n");

bool8 EventOp_DefeatOtherTeamsAndEndMatch(s16 a)
{
    DefeatOtherTeamsAndEndMatch(gUnknown_0200C528[a].unk04->unk08, 0x80);
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08018FB4\n.thumb_set sub_08018FB4, EventOp_DefeatOtherTeamsAndEndMatch\n");
