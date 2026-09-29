#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080448E4.
 * sub_080448E4 @ 0x080448E4
 */

#include "proc.h"
struct UnkP448E4
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x66);
    /* 66 */ s16 unk66;
};

/* Look up a unit id and, if there is one, report its cell to ScrollCameraToKeepCellInView.
 *
 * The guard tests the RETURN VALUE, not the field: the `cmp r0,#0` reuses the
 * byte AiPickMissileTargetUnit handed back, whereas the index a few instructions later is
 * a fresh `ldrsh` off the proc. A single `proc->unk66 != 0` guard would have
 * been the same `ldrsh` both times, so the local is in the ROM and not a
 * convenience here.
 *
 * `&gUnits[...]` is bound to a pointer before the two byte reads;
 * that is what keeps the pool word in its own register and defers the
 * dereference past the index arithmetic, which is the ROM's order. */
void CoPowerMeteor_PickTarget(struct UnkP448E4 *proc)
{
    u8 id = AiPickMissileTargetUnit(gUnknown_030033EC, 1);
    struct Unit *e;

    proc->unk66 = id;
    if (id != 0) {
        e = &gUnits[proc->unk66];
        ScrollCameraToKeepCellInView(e->x, e->y);
    }
}
asm(".global sub_080448E4\n.thumb_set sub_080448E4, CoPowerMeteor_PickTarget\n");
