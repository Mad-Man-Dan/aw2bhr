#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080287D0.
 * sub_080287D0 @ 0x080287D0
 */

#include "proc.h"
/* MATCHED in wave 38 (W38-I), after being parked at 90.8% in wave 34.
 *
 * Two facts were derived in wave 34 and both survive -- the body below reaches
 * the ROM's instruction stream exactly, and a from-scratch rewrite this wave
 * arrived at the identical stream independently:
 *   - gUnknown_084995FE is a declared `s16 []` read with a plain `ldrh`, so the
 *     source casts the table entry to `(u16)`; without it agbcc emits `ldrsh`.
 *   - THE SUM'S OPERANDS ARE (table + unk68), NOT (unk68 + table). The ROM
 *     re-reads proc->unk68 with a SECOND `ldrsh` inside the index even though
 *     the `<= 0x32` compare loaded it four instructions earlier. That only
 *     happens when unk68 is evaluated AFTER the table lookup; written the other
 *     way agbcc keeps the compare's value alive and emits `adds r2, r0, #0`.
 *
 * WHAT THE 11-BYTE RESIDUAL WAS, and it is NOT what the wave-34 note said. That
 * note called it a live-range tie in allocno_compare that "nothing source-level
 * found here moves". It is source-level movable, and it is a REFERENCE COUNT,
 * not a live range. gcc 2.9 ranks allocnos by
 *     floor_log2(n_refs) * n_refs / live_length
 * and `proc` has seven references in the flat spelling (its def, the two
 * Proc_Break arguments, the StartUnitDestroy argument, and the three address bases
 * &unk64, &unk66, &unk68) against five for `proc + 0x68` in a range a little
 * over half as long -- 14/45 to 10/26, so `proc + 0x68` sorted first and took
 * r4, where the ROM gives r4 to `proc`.
 *
 * The lever is the `do { } while (0)`, and the reason it works is that
 * REG_N_REFS IS WEIGHTED BY LOOP DEPTH: gcc 2.x's flow.c adds `loop_depth` per
 * reference rather than 1, so wrapping a block in a degenerate loop scales up
 * every reference inside it. Two of proc's seven references sit in the wrapped
 * block against two of `proc + 0x68`'s five, which is enough to invert the
 * ratio -- and it takes proc's weighted count past 8, where floor_log2 steps
 * from 2 to 3 and the numerator jumps 14 -> 24. Confirmed independently by
 * controlled probe before the permuter ran: adding one redundant
 * `Proc_Break(proc)` to the else arm -- which LENGTHENS proc's live range, i.e.
 * works against it -- moves proc into r4 and `proc + 0x68` into r5 and changes
 * no other instruction.
 *
 * So this is a genuine allocation lever here and not the wave-18 trap, where a
 * `do { } while (0)` was masking a wrongly-declared table type: the stream was
 * already instruction-exact before the wrapper went on, so there is no type
 * error left for it to compensate for. The wrapper is permuter output, so it is
 * a spelling that produces the ROM's bytes and NOT a claim about what the
 * original source looked like -- the original presumably referenced proc an
 * eighth time some other way. */

struct Unk080287D0Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
};

void ArmyDefeat_Loop(struct Unk080287D0Proc *proc)
{
    struct Unit *unit;

    if (proc->unk66 == 1 || proc->unk66 == 0x10)
    {
        Proc_Break(proc);
    }
    else if (Proc_Find(gUnknown_0849A198) == NULL)
    {
        if (proc->unk68 <= 0x32)
        {
            do
            {
                unit = &gUnits[(u16)gUnknown_084995FE[proc->unk64] + proc->unk68];
                proc->unk68++;
                StartUnitDestroy(unit, proc);
            } while (0);
        }
        else
        {
            Proc_Break(proc);
        }
    }
}
asm(".global sub_080287D0\n.thumb_set sub_080287D0, ArmyDefeat_Loop\n");
