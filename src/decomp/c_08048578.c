#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048578.
 * sub_08048578 @ 0x08048578
 */

/* gUnknown_03003F2C + proc->unk28 indexes the 12-byte unit records
 * gUnits points at -- the lsls #1; adds; lsls #2 chain is x * 3 * 4,
 * agbcc's usual shape for a stride of 12. unk02 and unk03 are that struct's
 * cell column and row, and ScrollCameraToKeepCellInView takes them as s16.
 *
 * proc->unk28 is loaded twice; the guard and the index are separate reads in
 * the ROM, which is what an if (p->unk28) around a body that reads it again
 * produces here. */
struct Unk48578Proc
{
    /* 00 */ u8 filler_00[0x28];
    /* 28 */ u16 unk28;
};

void UnitList_ScrollToSelectedUnit(struct Unk48578Proc *proc)
{
    struct Unit *u;

    if (proc->unk28 != 0)
    {
        u = &gUnits[gUnknown_03003F2C + proc->unk28];
        ScrollCameraToKeepCellInView(u->x, u->y);
    }
}
asm(".global sub_08048578\n.thumb_set sub_08048578, UnitList_ScrollToSelectedUnit\n");
