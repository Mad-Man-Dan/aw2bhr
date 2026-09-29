#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071B28.
 * sub_08071B28 @ 0x08071B28
 */

#include "hardware.h"
#include "proc.h"
/* Starts a palette-fade proc on one bank: it takes the slot record first,
 * starts the proc under the caller's parent, snapshots the bank's current
 * gPal contents into the record with CpuSet, then fills in the target palette,
 * the destination slice and the step counters, and hands the record to the
 * proc's +0x2c.
 *
 * The record address is computed BEFORE Proc_Start and the gPal slice AFTER,
 * which is the ROM's order and what two separate statements give -- the record
 * is live across the call (r4) and the slice is not.
 *
 * The proc's own type is local to this file: only its +0x2c is ever touched
 * here, and Proc_Start's `ProcPtr` return carries no fields. */
struct Unk8071B28Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ struct Unk0202F2DC *unk2c;
};

void *StartPalFade(const void *pal, int index, int b, ProcPtr parent)
{
    struct Unk0202F2DC *p;
    struct Unk8071B28Proc *proc;
    u16 *slice;

    p = &gUnknown_0202F2DC[index];
    proc = Proc_Start(gUnknown_08613E54, parent);
    slice = &gPal[index * 16];

    CpuSet(slice, p, 0x10);

    p->unk24 = slice;
    p->unk20 = pal;
    p->unk28 = 0;
    p->unk2a = b;
    p->unk2c = b + 1;

    proc->unk2c = p;

    return p;
}
asm(".global sub_08071B28\n.thumb_set sub_08071B28, StartPalFade\n");
