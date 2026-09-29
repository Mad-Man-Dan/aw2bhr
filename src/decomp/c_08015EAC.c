#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015EAC.
 * sub_08015EAC @ 0x08015EAC, sub_08015EE0 @ 0x08015EE0
 */

/* A gate command: sub_08019260 decides whether the slot may advance at all.
 * It takes no argument -- r0 still holds the caller's `a << 24` when the `bl`
 * is reached, which only a nullary callee can tolerate. */
bool8 SlotOp_WaitForEventScripts(u8 a)
{
    if (sub_08019260())
        return FALSE;
    gUnknown_03001470[a].unk04 = (const u8 *)gUnknown_03001470[a].unk04 + 8;
    return TRUE;
}
asm(".global sub_08015EAC\n.thumb_set sub_08015EAC, SlotOp_WaitForEventScripts\n");

/* One command of the gUnknown_03001470 script stream. The operand is the
 * halfword at +4 of the 8-byte command, read SIGNED (`movs r1,#4;
 * ldrsh r0,[r0,r1]` -- the register-offset form `ldrsh` is forced into), which
 * agrees with EndSlotScriptAt's declared `s16` parameter. Its neighbours in
 * src/decomp/c_08015FE4.c read the same halfword unsigned for callees that
 * take u16. */
bool8 SlotOp_EndSlotAt(u8 a)
{
    EndSlotScriptAt(((const s16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u8 *)gUnknown_03001470[a].unk04 + 8;
    return TRUE;
}
asm(".global sub_08015EE0\n.thumb_set sub_08015EE0, SlotOp_EndSlotAt\n");
