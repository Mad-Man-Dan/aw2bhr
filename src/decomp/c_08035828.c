#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035828.
 * sub_08035828 @ 0x08035828
 */

#include "proc.h"
struct Unk35828Sub
{
    /* 0x00 */ u8 filler_00[0x01];
    /* 0x01 */ u8 unk01;
};
struct Unk35828Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x30);
    /* 0x30 */ struct Unk35828Sub *unk30;
};

/* The mask is a plain scalar `&=`, not a bitfield: clearing bits 1..2 of a
 * bitfield would build the complement negative (bit 7 of 0xf9 is set) and emit
 * `mov #7; neg`, and the ROM has a bare `movs #0xf9`.
 */
void EndMoveSlide(struct Unk35828Proc *proc)
{
    if (proc->unk30 != NULL)
        proc->unk30->unk01 &= 0xf9;

    Proc_End(proc);

    gUnknown_030040E4 = 0;
}
asm(".global sub_08035828\n.thumb_set sub_08035828, EndMoveSlide\n");
