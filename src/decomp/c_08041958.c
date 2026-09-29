#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08041958.
 * sub_08041958 @ 0x08041958
 */

#include "proc.h"
/* A DIFFERENT proc from the 0x080412A4 group: +0x4c here is a full word
 * (`ldr`, then handed on as a pointer), not the s16 counter those read. */
struct Unk41958Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4c);
    /* 4c */ void *unk4c;
};

/* StartPipeSeamHit's THIRD argument is invisible at this call: r2 already holds
 * the proc, so no instruction sets it up. StartPipeSeamHit's own prologue does
 * `adds r1, r2, #0; bl Proc_StartBlocking`, which is what proves it is there. */
void InventionAttack_StartHit(struct Unk41958Proc *proc)
{
    if (proc->unk4c != 0)
        StartInventionHit(proc->unk4c, proc);
    else
        StartPipeSeamHit(proc->unk2c, proc->unk30, proc);
}
asm(".global sub_08041958\n.thumb_set sub_08041958, InventionAttack_StartHit\n");
