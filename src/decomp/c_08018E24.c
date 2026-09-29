#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018E24.
 * sub_08018E24 @ 0x08018E24
 */

/*
 * EventOp_StartCursorScript -- script command: start the gUnknown_0848A378 script and pass
 * it this node.
 *
 * Skipped while gUnknown_03002514 is 1. StartEventScript puts the script in a free
 * gUnknown_0200C528 slot, and that slot's .unk14 is pointed at this script's
 * current node, which is where EventCursorScript_Draw and its neighbours read their
 * parameters from. The cursor then steps one node on and TRUE comes back, so
 * the dispatcher runs the next command in the same frame.
 *
 * .unk14 is declared u32 and takes a whole word, so the node pointer is cast
 * rather than the member retyped; src/decomp/c_08018DF8.c casts it back the
 * same way.
 */
bool8 EventOp_StartCursorScript(s16 a)
{
    if (gUnknown_03002514 != 1)
        StartEventScript(gUnknown_0848A378)->unk14 = (u32)gUnknown_0200C528[a].unk04;
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08018E24\n.thumb_set sub_08018E24, EventOp_StartCursorScript\n");
