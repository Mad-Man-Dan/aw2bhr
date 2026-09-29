#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803EB84.
 * sub_0803EB84 @ 0x0803EB84
 */

#include "proc.h"
struct Unk3EB84Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ s16 unk4c;
};

/* A Proc_ForEach-style filter: end this proc unless its own +0x4c tag is the
 * currently selected one. The `movs r3, #0` next to the `ldrsh` is not an
 * operand of anything -- `ldrsh` has no immediate-offset form, which is also
 * why the base gets `adds r0, #0x4c` first.
 */
void InventionFire_EndWhenDone(struct Unk3EB84Proc *proc)
{
    if (proc->unk4c == gUnknown_03003F40)
        Proc_End(proc);
}
asm(".global sub_0803EB84\n.thumb_set sub_0803EB84, InventionFire_EndWhenDone\n");
