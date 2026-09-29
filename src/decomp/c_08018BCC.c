#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018BCC.
 * sub_08018BCC @ 0x08018BCC
 */

/* "Advance if the node's predicate says so, otherwise follow its link."
 * The node's +0x0c is a NULLARY function here (`bl _call_via_r0`) returning
 * s16 -- `lsls r0, #0x10; cmp r0, #0` is a signed 16-bit truth test.
 *
 * Both arms return, so the arms are written inverted (c_08017CF0.c rule): the
 * ELSE is emitted inline and the branch goes to the THEN.
 *
 * The trailing `lsls r0, #0x10; asrs r0, #0x10` on EventOp_Jump's result is an
 * INT converted to this function's own `s16` return type, which is what retyped
 * EventOp_Jump from `bool8`. */
s16 EventOp_JumpIfCallTrue(s16 a)
{
    s16 (*f)(void);

    f = (s16 (*)(void))gUnknown_0200C528[a].unk04->unk0c;
    if (f() != 0)
        return EventOp_Jump(a);
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}
asm(".global sub_08018BCC\n.thumb_set sub_08018BCC, EventOp_JumpIfCallTrue\n");
