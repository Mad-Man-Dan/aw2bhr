#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807610C.
 * sub_0807610C @ 0x0807610C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* `v` must be an `int` LOCAL and the OR must be its own statement. Written as
 * `proc->unk3a |= 0xFF` the field's value is known to be 0 from the `cmp`, so
 * agbcc folds the whole thing to `movs #1; rsbs` (-1); written as
 * `proc->unk3a = v | 0xFF` in one statement, combine narrows the OR into the
 * `strb` and folds it the same way. Only `v |= <mask>;` as a separate statement
 * keeps the `orrs`.
 *
 * THE MASK IS ITS OWN `int` LOCAL, and that is what fixes the ORR's operand
 * orientation -- the last 2 bytes of this function. `v |= 0xFF` emits
 * `mov r0,#0xff; orr r0, r0, r1`, i.e. the OR's DESTINATION is the constant's
 * pseudo and not v's; `int m = 0xFF; ... v |= m;` emits
 * `mov r0,#0xff; orr r1, r1, r0`, which is the ROM. Same instruction, same two
 * registers, same size -- only the tied operand differs. The mechanism is that
 * a THUMB-immediate-impossible constant written inline stays a CONST_INT in the
 * tree, so expand forces it into a fresh pseudo AT the binop and the commutative
 * `%0` constraint lets reload tie the destination to that pseudo instead of to
 * v; given its own local the constant already has a pseudo of its own before the
 * binop is expanded, and v's is the one the result ties to. `m` itself is folded
 * away -- there is no trace of it in the output. Also ruled out by probe:
 * `v = v | 0xFF`, `v = 0xFF | v`, `register int v`, a separate destination
 * local, a comma operator, `proc->unk3a = v |= 0xFF`, testing `(u8)v == 0`
 * instead of the field, and reaching the field through a `s8 *`.
 *
 * The two gUnknown_0202FDFC sums are spelled with the FAR member first
 * (`unk04 + unk00`): agbcc evaluates the second operand first, so that is what
 * puts `ldrh [r0, #0]` ahead of `ldrh [r0, #4]`. */
struct Unk807610C
{
    /* 0x00 */ u8 filler_00[0x36];
    /* 0x36 */ u16 unk36;
    /* 0x38 */ u16 unk38;
    /* 0x3a */ s8 unk3a;
};

void WorldMapNationPanel_Setup(struct Unk807610C *proc)
{
    int v = GetWorldMapNationPanelSide();
    int m = 0xFF;

    proc->unk3a = v;

    if (proc->unk3a == 0)
    {
        v |= m;
        proc->unk3a = v;
    }

    WorldMapNationPanel_Refresh(proc);
    proc->unk36 = gUnknown_0202FDFC.unk04 + gUnknown_0202FDFC.unk00;
    proc->unk38 = gUnknown_0202FDFC.unk06 + gUnknown_0202FDFC.unk02;
}

asm(".global sub_0807610C\n.thumb_set sub_0807610C, WorldMapNationPanel_Setup\n");
