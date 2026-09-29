#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C828.
 * SubmarinePart_Loop @ 0x0804C828
 */

/* MATCHED, and the first of a byte-identical pair with LanderPart_Loop. Re-tints
 * the current gUnknown_03001470 slot: run RidePartOnFigure for the side/slot, fetch
 * the slot's OBJ attributes, replace paletteNum from a table indexed by a sum
 * of two per-slot halfwords, and hand the attributes back.
 *
 * Two things carry over from the wave-17/18 cluster and neither is guessable:
 *
 *  - `pal` is load-bearing, for exactly the reason src/decomp/c_080524C0.c
 *    gives: assigning a u16 table element straight into a bitfield narrower
 *    than 16 bits lets force_to_mode push the store's byte mask back into the
 *    load and emit `ldrb`. The ROM has `ldrh`.
 *  - gUnknown_020298E0 is a STRUCT array whose members are the two five-slot
 *    halfword arrays, not a flat `u16 [][72]`. The ROM adds 0x26 / 0x30 to the
 *    BASE register and leaves the `ldrh` displacement at zero.
 *
 * unk26 is named FIRST in the sum although its `ldrh` is issued SECOND -- the
 * addresses expand in source order and the loads come out in the opposite one,
 * the same readout src/decomp/c_08051DE0.c documents for a two-term sum.
 *
 * RidePartOnFigure's third argument is gUnknown_03001FBC and there is no `mov r2`
 * at the call: the allocator had already loaded it into r2 for the
 * gUnknown_03001470 subscript above and nothing clobbers it. Arity is invisible
 * in the caller, so this is read off the callee's prologue. */
void SubmarinePart_Loop(void)
{
    struct OamData oam;
    u16 side;
    u16 slot;
    s16 t;
    u16 pal;

    side = gUnknown_03001470[gUnknown_03001FBC].unk30;
    slot = gUnknown_03001470[gUnknown_03001FBC].unk34;

    RidePartOnFigure(side, slot, gUnknown_03001FBC);
    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);

    t = gUnknown_020298E0[side].unk26[slot] + gUnknown_020298E0[side].unk30[slot];
    pal = gUnknown_08553B40[side][t];
    oam.paletteNum = pal;

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
}

asm(".global sub_0804C828\n.thumb_set sub_0804C828, SubmarinePart_Loop\n");
