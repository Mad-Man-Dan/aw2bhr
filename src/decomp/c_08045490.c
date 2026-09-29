#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045490.
 * sub_08045490 @ 0x08045490
 */

#include "proc.h"
/* The proc scans the map for the first cell of its own army's terrain type
 * whose 0x51A plane byte is still clear, hands the coordinates to
 * ScrollCameraToKeepCellInView and returns; running off the end of the map (or finding no
 * free gUnits slot at all) goes to label 1 instead.
 *
 * The cursor lives in the PROC, not in locals: `proc->unk2c` and
 * `proc->unk30` are read and written through memory on every iteration, which
 * is why both loops reload them and why the outer body re-reads unk30 even
 * though the tail just stored it.
 *
 * The three planes are members of a struct laid over gUnknown_08499590 -- the
 * ROM computes `(map + 0x417A) + y * 2`, `(map + 0x1432) + idx` and
 * `(map + 0x51A) + idx`, and only a COMPONENT_REF preserves that association.
 * The 0x51A base is the one that will not fit in a register at this pressure,
 * so it takes the single stack slot.
 *
 * `(gUnknown_030033EC << 5) | 6` is an `orrs`, not an add: the low bits of the
 * shifted army number are known zero but the ROM still spells the combine as
 * a bitwise or. */
struct Unk45490Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
};

void CoPowerCreateUnits_FindCity(struct Unk45490Proc *proc)
{
    int idx;

    if (FindFreeUnitSlot() == NULL)
    {
        Proc_Goto(proc, 1);
        return;
    }

    while (proc->unk30 < gMap->height)
    {
        while (proc->unk2c < gMap->width)
        {
            idx = gMap->rowOffset[proc->unk30] + proc->unk2c;

            if (gMap->terrain[idx] == ((gUnknown_030033EC << 5) | 6)
             && gMap->unitUnk[idx] == 0)
            {
                ScrollCameraToKeepCellInView(proc->unk2c, proc->unk30);
                return;
            }

            proc->unk2c++;
        }

        proc->unk2c = 0;
        proc->unk30++;
    }

    Proc_Goto(proc, 1);
}
asm(".global sub_08045490\n.thumb_set sub_08045490, CoPowerCreateUnits_FindCity\n");
