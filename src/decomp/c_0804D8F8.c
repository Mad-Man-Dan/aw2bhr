#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804D8F8.
 * PartRideFigure_Loop @ 0x0804D8F8
 */

/* MATCHED, and the first of a byte-identical pair with PartRideFigure_Loop2. Two-line
 * forwarder into RidePartOnFigure for the current slot.
 *
 * The third argument is gUnknown_03001FBC and there is NO `mov r2` at the call:
 * the allocator had already loaded it into r2 for the gUnknown_03001470
 * subscript, and nothing clobbers it. Read the callee's prologue for arity --
 * a caller that sets no register is not a caller that passes no argument. Same
 * readout as SubmarinePart_Loop. */
void PartRideFigure_Loop(void)
{
    RidePartOnFigure(gUnknown_03001470[gUnknown_03001FBC].unk30,
        gUnknown_03001470[gUnknown_03001FBC].unk34, gUnknown_03001FBC);
}

asm(".global sub_0804D8F8\n.thumb_set sub_0804D8F8, PartRideFigure_Loop\n");
