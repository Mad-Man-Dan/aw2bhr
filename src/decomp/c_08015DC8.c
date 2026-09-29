#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015DC8.
 * sub_08015DC8 @ 0x08015DC8
 */

/* One command of the gUnknown_03001470 script interpreter, the indirect-call
 * form: the command word is a handler address, the handler is passed the slot
 * itself, the cursor then steps one 8-byte command forward, and the slot's
 * unk00 decides the return.
 *
 * The handler is bound to a LOCAL before the call. That is load-bearing: as a
 * single call expression agbcc computes the slot address before loading the
 * handler and the two `ldr`s come out in the wrong order. See SlotOp_StartEventScript in
 * src/decomp/c_08015E80.c for why `gUnknown_03001470[a].unk04` is named twice
 * rather than bound to a pointer.
 *
 * The `!= 0` sense is read off the block order: the branch is taken to the
 * block AFTER the literal pool, and that block is `movs r0,#1`, so the
 * TRUE-returning arm is the one under the `if`. */
bool8 SlotOp_CallHandler(u8 a)
{
    bool8 (*handler)(struct Unk03001470 *) =
        *(bool8 (**)(struct Unk03001470 *))gUnknown_03001470[a].unk04;

    handler(&gUnknown_03001470[a]);
    gUnknown_03001470[a].unk04 = (const u8 *)gUnknown_03001470[a].unk04 + 8;
    if (gUnknown_03001470[a].unk00 != 0)
        return TRUE;
    return FALSE;
}
asm(".global sub_08015DC8\n.thumb_set sub_08015DC8, SlotOp_CallHandler\n");
