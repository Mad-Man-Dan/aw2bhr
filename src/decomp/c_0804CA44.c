#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804CA44.
 * sub_0804CA44 @ 0x0804CA44
 */

/* The same "current gUnknown_03001470 slot" preamble src/decomp/c_0804C828.c
 * documents: side and slot come out of the slot record and gUnknown_03001FBC is
 * re-read for each three-argument callee. There is no `mov r2` in front of the
 * FIRST `bl` because the allocator already had gUnknown_03001FBC in r2 for the
 * subscript above -- arity is invisible at the call and is read off each
 * callee's own prologue (sub_0804CA98 narrows three parameters, StepFigureSlide
 * two). */
void LanderFigure_Loop(void)
{
    u16 side;
    u16 slot;

    side = gUnknown_03001470[gUnknown_03001FBC].unk30;
    slot = gUnknown_03001470[gUnknown_03001FBC].unk34;

    sub_0804CA98(side, slot, gUnknown_03001FBC);
    StepFigureHitFlash2(side, slot, gUnknown_03001FBC);
    RidePartOnFigure(side, slot, gUnknown_03001FBC);
    StepFigureSlide(side, slot);
}
asm(".global sub_0804CA44\n.thumb_set sub_0804CA44, LanderFigure_Loop\n");
