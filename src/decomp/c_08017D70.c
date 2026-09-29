#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017D70.
 * sub_08017D70 @ 0x08017D70
 */

#include "proc.h"

/* One more command handler in the gUnknown_0200C528 script family (see
 * c_08017B64.c, c_08018B18.c): read the current node, act on it, advance the
 * cursor by one node, return TRUE.  The cursor is re-read after the `bl`
 * because the call is opaque, which is the same reload c_08017B64.c gets from
 * its store through an opaque pointer.
 *
 * The node's +0x04 doubles as a proc script here and as a list link in
 * EventOp_Jump, so it is cast rather than retyped. */
bool8 EventOp_StartProc(s16 a)
{
    Proc_Start((const struct ProcCmd *)gUnknown_0200C528[a].unk04->unk04,
        (ProcPtr)gUnknown_0200C528[a].unk04->unk0c);
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08017D70\n.thumb_set sub_08017D70, EventOp_StartProc\n");
