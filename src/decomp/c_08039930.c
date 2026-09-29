#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039930.
 * sub_08039930 @ 0x08039930
 */

#include "proc.h"
/* Only the word at +0x54 is ever touched here; the rest of the proc is
 * whatever struct Proc already describes. */
struct Unk39930Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
};

void StartCoPowerPanel(int a, ProcPtr parent)
{
    struct Unk39930Proc *proc = Proc_Start(gUnknown_0849D7FC, parent);

    proc->unk54 = a;
}
asm(".global sub_08039930\n.thumb_set sub_08039930, StartCoPowerPanel\n");
