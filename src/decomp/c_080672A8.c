#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080672A8.
 * BlendRamp_Loop @ 0x080672A8
 */

#include "hardware.h"
#include "proc.h"
struct Unk672A8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ int unk3c;
};

/* MATCHED, and the first of a byte-identical pair with sub_08067358. A blend
 * proc tick: publish the blend coefficients for this frame, interpolate the
 * next one, and break when the ramp is finished.
 *
 * Two spellings are load-bearing and both were probe rounds:
 *
 *  - `(g & 0xFFE0) | ((unk3c << 4) | 0xF)` and NOT `... | (unk3c << 4) | 0xF`.
 *    The ROM ORs 0xF into the shifted field FIRST and merges the result into
 *    the masked global second; the flat left-to-right spelling reverses those
 *    two `orr`s.
 *  - `v = Interpolate(...); gUnknown_03001FFC = v;` and not the direct
 *    assignment. gUnknown_03001FFC is `volatile`, so the LHS address is forced
 *    into a register BEFORE the call is expanded, and it then needs a
 *    callee-saved home across it -- worth one extra saved register and a
 *    misplaced pool `ldr`. This is a new corner of the volatile note in
 *    hardware.h, which recorded the other eight users as byte-neutral because
 *    all of them store a CONSTANT; a call result is not byte-neutral.
 *
 * `.raw` and not `.bits`: the ROM has `ldrh` with a pool mask, which
 * hardware.h's union note reads as the HImode read-modify-write. */
void BlendRamp_Loop(struct Unk672A8Proc *proc)
{
    s32 v;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0)
        | ((proc->unk3c << 4) | 0xF);

    v = Interpolate(0, proc->unk30, proc->unk34, proc->unk38, proc->unk2c);
    gUnknown_03001FFC = v;

    if (proc->unk2c == proc->unk38)
        Proc_Break(proc);

    proc->unk38++;
}
asm(".global sub_080672A8\n.thumb_set sub_080672A8, BlendRamp_Loop\n");
