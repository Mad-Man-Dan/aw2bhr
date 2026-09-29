#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017DA0.
 * sub_08017DA0 @ 0x08017DA0, sub_08017DD4 @ 0x08017DD4
 */

#include "proc.h"

/* Advance only once the proc the node names has finished. The node's +0x04 is a
 * proc script here, exactly as in c_08017D70.c. Same inverted-arm spelling as
 * EventOp_CallFunctionSkippable -- both arms return, so agbcc emits the else inline. */
bool8 EventOp_WaitForProc(s16 a)
{
    if (Proc_Find((const struct ProcCmd *)gUnknown_0200C528[a].unk04->unk04) != NULL)
        return FALSE;
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}
asm(".global sub_08017DA0\n.thumb_set sub_08017DA0, EventOp_WaitForProc\n");

/* Three `ldrsh` reads of the node payload. unk0c is declared `u32`, so its
 * `ldrsh` can only be a cast; unk08/unk0a get the same spelling for
 * consistency. */
bool8 EventOp_CreateExhaustedUnit(s16 a)
{
    struct Unk0200C528Node *p = gUnknown_0200C528[a].unk04;

    CreateExhaustedUnitAt((s16)p->unk08, (s16)p->unk0a, (s16)p->unk0c);
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08017DD4\n.thumb_set sub_08017DD4, EventOp_CreateExhaustedUnit\n");
