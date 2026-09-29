#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032698.
 * sub_08032698 @ 0x08032698
 */

#include "proc.h"
struct Unk32698Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void LinkStartMapTransfer(ProcPtr parent)
{
    struct Unk32698Proc *proc;

    gUnknown_03003F1C = 0;

    if (gUnknown_0849B060->unk09 == gUnknown_0849B018->unk06)
    {
        ReadSaveSlot(gUnknown_0849B060->unk08 + 5, gUnknown_02000000);
        proc = Proc_StartBlocking(gUnknown_0849B868, parent);
    }
    else
    {
        proc = Proc_StartBlocking(gUnknown_0849B7D8, parent);
    }

    proc->unk58 = gUnknown_0849B060->unk08;
}
asm(".global sub_08032698\n.thumb_set sub_08032698, LinkStartMapTransfer\n");
