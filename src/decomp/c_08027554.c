#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027554.
 * sub_08027554 @ 0x08027554
 */

#include "proc.h"

/* Family F025: `push {lr}; ldr r0,[r0,#0x50]; bl f; pop {r0}; bx r0`,
 * i.e. `f(proc->unk50)`.
 *
 * The parameter is a Proc and this is PROVED, not assumed from the address:
 * the ROM holds `PROC_ONEND(SupplyIconEffect_OnEnd)` at 0x08499D2C -- a proc-script command whose
 * callback is exactly this function, so what arrives in r0 is the running
 * proc. All six members of the family check out this way (five PROC_ONEND, one
 * PROC_CALL). PROC_HEADER is 0x29 bytes and every offset the family reaches
 * (0x30, 0x4c, 0x50, 0x54) is above it, so the header fits -- and STRUCT_PAD
 * would have failed loudly, as a negative array size, if it did not. */

struct Unk08027554Proc
{
    PROC_HEADER;
    STRUCT_PAD(0x29, 0x50);
    /* 50 */ void *unk50;
};

void SupplyIconEffect_OnEnd(struct Unk08027554Proc *proc)
{
    AP_Delete(proc->unk50);
}
asm(".global sub_08027554\n.thumb_set sub_08027554, SupplyIconEffect_OnEnd\n");
