#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080338C0.
 * sub_080338C0 @ 0x080338C0, sub_080338DC @ 0x080338DC
 */

#include "proc.h"
/* Only the word at +0x38 is touched here; the rest of the proc is whatever
 * struct Proc already describes. */
struct Unk338C0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x38);
    /* 38 */ int unk38;
};

void LinkC4_SetCursorRow(int a)
{
    struct Unk338C0Proc *proc = Proc_Find(gUnknown_0849BB50);

    proc->unk38 = a * 32 + 0x2e;
}
asm(".global sub_080338C0\n.thumb_set sub_080338C0, LinkC4_SetCursorRow\n");

void sub_080338DC(void)
{
    if (gGameClock & 1)
        gUnknown_03001FF8++;
}
