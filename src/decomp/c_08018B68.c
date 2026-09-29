#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018B68.
 * sub_08018B68 @ 0x08018B68
 */

#include "hardware.h"

/* "Wait for one of the keys in the node's mask." The node's +0x04 is the
 * polymorphic script operand again -- here a NULLARY function -- and its +0x0c
 * the key mask ANDed with gpKeySt->unk0c.
 *
 * `bl _call_via_r2` is NOT a two-argument call: r0 and r1 are simply both live
 * (the AND result and the node) when the pointer is materialised, so gcc takes
 * the next free scratch. The register index only counts arguments when the
 * argument registers are the reason they are occupied. */
bool8 EventOp_WaitForKeyThenCall(s16 a)
{
    struct Unk0200C528Node *p;
    void (*f)(void);

    p = gUnknown_0200C528[a].unk04;
    f = (void (*)(void))p->unk04;

    if (gpKeySt->unk0c & p->unk0c)
    {
        f();
        EnableScriptedInput();
        gUnknown_0200C528[a].unk04++;
    }
    return FALSE;
}
asm(".global sub_08018B68\n.thumb_set sub_08018B68, EventOp_WaitForKeyThenCall\n");
