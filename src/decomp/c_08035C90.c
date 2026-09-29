#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035C90.
 * sub_08035C90 @ 0x08035C90
 */

#include "proc.h"
/* "Is the cell under this proc terrain kind 2?"  The cell key is the standard
 * gUnknown_08499590 one -- `rowOffset[y] + x` off the +0x417A row table into
 * the +0x1432 terrain plane -- with the proc's PIXEL coordinates converted to
 * cells first: `(v + 8) / 16` is a signed divide, hence the
 * `adds #8; bge; adds #0xf; asrs #4` rounding chain on each axis.
 *
 * The map MUST be reached through a struct laid over the header rather than by
 * byte arithmetic on the u8 * symbol: the ROM computes `(map + 0x417A) + y * 2`
 * and `(map + 0x1432) + idx`, and only a COMPONENT_REF keeps that association
 * (see the gUnknown_08499590 note in include/unknown-globals.h).
 *
 * `movs #0` / `movs #1` split across the literal pool with an unconditional
 * `b` is the if/else-return form, not a returned comparison -- and the arm
 * ORDER settles the sense: `== 2` returning 1 puts the `beq` on the 1 arm,
 * which is what the ROM has; the `!= 2` spelling emits the two arms swapped. */
struct Unk35C90Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x42);
    /* 0x42 */ s16 unk42;
    /* 0x44 */ s16 unk44;
};

u8 IsMoveSlideOnRiver(ProcPtr procArg)
{
    struct Unk35C90Proc *proc = procArg;
    struct Map *map = gMap;

    if ((map->terrain[map->rowOffset[(proc->unk44 + 8) / 16]
                    + (proc->unk42 + 8) / 16] & 0x1f) == 2)
        return 1;

    return 0;
}
asm(".global sub_08035C90\n.thumb_set sub_08035C90, IsMoveSlideOnRiver\n");
