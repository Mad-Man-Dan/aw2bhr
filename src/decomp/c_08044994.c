#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044994.
 * sub_08044994 @ 0x08044994
 */

struct Unk8044994
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
};

void CoPowerMeteor_ApplyDamage(struct Unk8044994 *proc)
{
    struct Unit *e;
    int col;
    int row;

    if (proc->unk66 > 0)
    {
        e = &gUnits[proc->unk66];
        col = e->x;
        row = e->y;

        DamageUnitAtCellCopy(col, row - 2, proc->unk64);

        DamageUnitAtCellCopy(col - 1, row - 1, proc->unk64);
        DamageUnitAtCellCopy(col, row - 1, proc->unk64);
        DamageUnitAtCellCopy(col + 1, row - 1, proc->unk64);

        DamageUnitAtCellCopy(col - 2, row, proc->unk64);
        DamageUnitAtCellCopy(col - 1, row, proc->unk64);
        DamageUnitAtCellCopy(col, row, proc->unk64);
        DamageUnitAtCellCopy(col + 1, row, proc->unk64);
        DamageUnitAtCellCopy(col + 2, row, proc->unk64);

        DamageUnitAtCellCopy(col - 1, row + 1, proc->unk64);
        DamageUnitAtCellCopy(col, row + 1, proc->unk64);
        DamageUnitAtCellCopy(col + 1, row + 1, proc->unk64);

        DamageUnitAtCellCopy(col, row + 2, proc->unk64);
    }

    ApplyCoPowerStatus();
}
asm(".global sub_08044994\n.thumb_set sub_08044994, CoPowerMeteor_ApplyDamage\n");
