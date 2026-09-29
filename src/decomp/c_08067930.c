#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067930.
 * sub_08067930 @ 0x08067930
 */

#include "proc.h"
struct Unk67930Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x50);
    /* 50 */ u8 unk50;
};

/* The `adds r0, #0x50` is only the THUMB ldrb displacement limit of 31, not an
 * address being taken. */
void IntroBgAffineTween_WaitTrigger(struct Unk67930Proc *proc)
{
    if (proc->unk50 == 1)
        Proc_Break(proc);
}
asm(".global sub_08067930\n.thumb_set sub_08067930, IntroBgAffineTween_WaitTrigger\n");
