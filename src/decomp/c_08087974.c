#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087974.
 * sub_08087974 @ 0x08087974
 */

#include "proc.h"
struct Unk08087974
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ int unk54;
};

void ToggleMapRecordsPanel(int a, ProcPtr parent)
{
    if (Proc_Find(gUnknown_08616DB4) != NULL)
    {
        EndMapRecordsPanel();
    }
    else
    {
        struct Unk08087974 *proc = Proc_Start(gUnknown_08616DB4, parent);

        proc->unk54 = a;
    }
}
asm(".global sub_08087974\n.thumb_set sub_08087974, ToggleMapRecordsPanel\n");
