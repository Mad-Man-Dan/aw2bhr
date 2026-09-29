#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804074C.
 * sub_0804074C @ 0x0804074C
 */

#include "proc.h"
/* One of the two arms of InventionAttack_StartHit. Resolves the entry's tile position with
 * GetInventionTargetCell, starts the 0x0849FBEC proc under the caller's parent, and
 * copies the position, the entry's 4-bit field and its unk04 byte into it,
 * keeping the entry itself at unk4c.
 *
 * `ldrh; lsls #0x16; lsrs #0x1c` is extract_bit_field on the unk02_6 bitfield,
 * not `(unk02 >> 6) & 0xf` on a plain u16 -- the latter gives
 * `lsr #6; movs #0xf; and`, which is longer. include/unknown-globals.h already
 * models unk02 as a 6/4/6 bitfield for exactly this reason. */
struct Unk4074CProc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x18];
    /* 4c */ struct Unk02028360 *unk4c;
    /* 50 */ u8 filler_50[0x14];
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
};

void StartInventionHit(struct Unk02028360 *ent, ProcPtr parent)
{
    struct Unk02028360Pos pos;
    struct Unk4074CProc *proc;

    GetInventionTargetCell(ent, &pos);

    proc = Proc_StartBlocking(gUnknown_0849FBEC, parent);
    proc->unk2c = pos.unk00;
    proc->unk30 = pos.unk02;
    proc->unk64 = ent->unk02_6;
    proc->unk66 = ent->unk04;
    proc->unk4c = ent;
}
asm(".global sub_0804074C\n.thumb_set sub_0804074C, StartInventionHit\n");
