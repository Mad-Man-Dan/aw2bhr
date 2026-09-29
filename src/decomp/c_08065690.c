#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065690.
 * sub_08065690 @ 0x08065690
 */

/* unk24 is UNSIGNED: the bound check at the tail is `cmp; bls`, and the two
 * mask constants share one register -- 0x200 is materialised with
 * `movs #0x80; lsls #2` and then decremented in place to 0x1ff, which is
 * agbcc's own constant CSE and not a second literal. */
struct Unk65690Proc
{
    /* 0x00 */ u8 filler_00[0x24];
    /* 0x24 */ u32 unk24;
    /* 0x28 */ u32 unk28;
    /* 0x2c */ u8 filler_2c[0x08];
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ void *unk3c;
};

void sub_08065690(struct Unk65690Proc *proc)
{
    proc->unk34 += proc->unk38;
    proc->unk24 += proc->unk34;
    PutOamHi((proc->unk24 + 0x200) & 0x1ff, (proc->unk28 + 0x100) & 0xff, proc->unk3c, 0);

    if (proc->unk24 + 0x30 > 0x120)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
