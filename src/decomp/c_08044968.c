#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044968.
 * sub_08044968 @ 0x08044968
 */

#include "proc.h"
struct UnkP448E4
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x66);
    /* 66 */ s16 unk66;
};

/* CoPowerMeteor_PickTarget's second half on its own, against StartMeteorImpact instead of
 * ScrollCameraToKeepCellInView. Here the single `ldrsh` serves both the guard and the index,
 * which is what the field being read directly looks like. */
void sub_08044968(struct UnkP448E4 *proc)
{
    struct Unit *e;

    if (proc->unk66 != 0) {
        e = &gUnits[proc->unk66];
        StartMeteorImpact(e->x, e->y);
    }
}
