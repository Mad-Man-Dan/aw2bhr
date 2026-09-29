#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B6E8.
 * sub_0803B6E8 @ 0x0803B6E8
 */

#include "proc.h"
/* Only the word at +0x54 is ever touched here; the rest of the proc is
 * whatever struct Proc already describes. */
struct UnkB6E8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
};

void StartMusicDuck(int a)
{
    struct UnkB6E8Proc *proc = Proc_Start(gUnknown_0849E778, PROC_TREE_3);

    proc->unk54 = a;
}
asm(".global sub_0803B6E8\n.thumb_set sub_0803B6E8, StartMusicDuck\n");
