#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080677E8.
 * sub_080677E8 @ 0x080677E8
 */

#include "proc.h"
struct Unk677E8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x58);
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
};

/* Resets the gUnknown_08580FE4 scroll proc IntroBgScroll_Loop ticks: clears the two
 * positions and the enable flag, then republishes the base value. The zero in
 * r1 is written three times and then serves as SetBgScrollShadow's second argument
 * unchanged -- constant CSE, not evidence about argument order. */
void ResetIntroBgScroll(void)
{
    struct Unk677E8Proc *proc = Proc_Find(gUnknown_08580FE4);

    proc->unk30 = 0;
    proc->unk2c = 0;
    proc->unk5c = 0;
    SetBgScrollShadow(proc->unk58, 0, 0);
}
asm(".global sub_080677E8\n.thumb_set sub_080677E8, ResetIntroBgScroll\n");
