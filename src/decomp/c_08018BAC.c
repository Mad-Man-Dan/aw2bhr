#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018BAC.
 * sub_08018BAC @ 0x08018BAC
 */

/*
 * EventOp_Jump -- script command: follow the current node's link.
 *
 * The slot's cursor is set to the node's .unk04, which is how a script jumps
 * instead of falling through to the next node. Returns TRUE, which makes the
 * dispatcher in src/decomp/c_08019404.c run the next command in the same frame.
 *
 * The int return type comes from the callers, not from this body: EventOp_JumpIfCallTrue,
 * EventOp_JumpIfCompletionFlagSet and EventOp_JumpIfCompletionFlagClear each sign-extend the result as a halfword into
 * their own s16 return, which neither a bool8 nor an s16 callee would produce.
 * See the note in include/unknown-functions.h.
 */
int EventOp_Jump(s16 a)
{
    gUnknown_0200C528[a].unk04 = gUnknown_0200C528[a].unk04->unk04;
    return TRUE;
}
asm(".global sub_08018BAC\n.thumb_set sub_08018BAC, EventOp_Jump\n");
