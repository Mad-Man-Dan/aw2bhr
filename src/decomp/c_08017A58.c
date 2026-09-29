#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017A58.
 * sub_08017A58 @ 0x08017A58
 */

/*
 * EventOp_Wait -- script command: copy the current node's .unk0c into the slot.
 *
 * gUnknown_0200C528[a].unk04 is the slot's cursor into its list of script
 * nodes. The node's .unk0c is copied to the slot's own .unk0c and the cursor
 * steps on one node. Returns FALSE, which ends the slot's turn for this frame
 * (see the dispatcher in src/decomp/c_08019404.c).
 *
 * The s16 return type comes from the caller and not from this body: EventOp_WaitSkippable
 * (src/decomp/c_08017C4C.c) passes the result straight on and sign-extends it
 * as a halfword, which a bool8 return could not produce. See the family note in
 * include/unknown-functions.h.
 */
s16 EventOp_Wait(s16 a)
{
    gUnknown_0200C528[a].unk0c = gUnknown_0200C528[a].unk04->unk0c;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08017A58\n.thumb_set sub_08017A58, EventOp_Wait\n");
