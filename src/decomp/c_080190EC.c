#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080190EC.
 * sub_080190EC @ 0x080190EC, sub_0801911C @ 0x0801911C
 */

/* The gUnknown_0200C528 cursor-advance family again, one step further than
 * src/decomp/c_0801903C.c: same `s16` parameter, same hoisted `g[i].unk04` row,
 * same `p++` on the 0x10-byte node, with a call that reads the node first.
 *
 * The `(s16)` cast on `unk08` is NOT evidence that the member is signed, and
 * the member stays `u16`. Probed: `p->unk08` on the u16 member emits the
 * one-instruction `ldrh r0, [r0, #8]`, while `(s16)p->unk08` folds the
 * truncate-then-sign-extend into the ROM's two-instruction register-offset
 * `movs r1, #8; ldrsh r0, [r0, r1]` -- byte-identical to declaring the member
 * `s16`. So an `ldrsh` at a member offset does not prove a signed member, and
 * c_08018C54.c's `ldrh` on the same member is left undisturbed. */
bool8 EventOp_SetCampaignFlag(s16 a)
{
    SetCampaignCompletionFlag((s16)gUnknown_0200C528[a].unk04->unk08, 1);
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_080190EC\n.thumb_set sub_080190EC, EventOp_SetCampaignFlag\n");

/* EventOp_SetCampaignFlag with SetCampaignCompletionFlag's second argument 0 instead of 1; see the note
 * on the `(s16)` cast there. */
bool8 EventOp_ClearCampaignFlag(s16 a)
{
    SetCampaignCompletionFlag((s16)gUnknown_0200C528[a].unk04->unk08, 0);
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_0801911C\n.thumb_set sub_0801911C, EventOp_ClearCampaignFlag\n");
