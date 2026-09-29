#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028848.
 * sub_08028848 @ 0x08028848
 */

#include "proc.h"
/* Both arguments are saved across the `bl` with a `lsls #16; lsrs #16` pair,
 * which is PROMOTE_MODE on a u16 parameter -- an `int` would carry no
 * narrowing at all. */
struct Unk28848Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
    /* 66 */ u16 unk66;
};

void StartArmyDefeat(u16 a, u16 b)
{
    struct Unk28848Proc *proc = Proc_Start(gUnknown_08499FAC, PROC_TREE_3);

    proc->unk64 = a;
    proc->unk66 = b;
}
asm(".global sub_08028848\n.thumb_set sub_08028848, StartArmyDefeat\n");
