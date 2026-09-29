#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08041E48.
 * sub_08041E48 @ 0x08041E48
 */

#include "proc.h"
struct Unk41E48Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ STRUCT_PAD(0x34, 0x64);
    /* 0x64 */ s16 unk64; /* `ldrsh` through a scratch register, so signed */
};

/* The same three bracketing calls src/decomp/c_0802CFFC.c opens with, and the
 * same gPlaySt.unk32 gate on the same SendActionCommand command -- this
 * is that function's per-proc variant, taking the two coordinates off the proc
 * instead of passing zeros. The `lsls #0x18; lsrs #0x18` pair on each is what
 * retyped SendActionCommand's third and fourth parameters to u8; c_0802CFFC.c's two
 * literal zeros could never have shown it. */

void InventionAttack_CommitMove(struct Unk41E48Proc *proc)
{
    BackupUnitStartPosition();
    CommitUnitMove();
    RestoreUnitStartPosition();

    if (proc->unk64 != 0 && gPlaySt.savingEnabled != 0)
        SendActionCommand(5, gUnknown_03003F38, proc->unk2c, proc->unk30);
}
asm(".global sub_08041E48\n.thumb_set sub_08041E48, InventionAttack_CommitMove\n");
