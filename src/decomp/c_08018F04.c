#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018F04.
 * sub_08018F04 @ 0x08018F04
 */

/* Family F041 (tools/families.py): four 48-byte cursor bumps on the
 * gUnknown_0200C528 list. `unk04` is a `struct Unk0200C528Node *` and the node
 * is 0x10 bytes, so the ROM's `adds r0, #0x10` is a plain `++` and not a
 * member at +0x10.
 *
 * The parameter really is `s16`, and the prologue is the proof despite the
 * general rule that a prologue narrowing means nothing when the value feeds a
 * call: the value does NOT feed the call here. All four callees are `(void)` --
 * r0 is simply left untouched across the `bl` -- so the `lsls #0x10; lsrs #0x10`
 * is PROMOTE_MODE on the parameter itself. Probed: `int` drops that pair and is
 * 4 bytes short; `u16 a` with `gUnknown_0200C528[(s16)a]` is byte-identical, so
 * the sign is read off the SECOND pair (`lsls #0x10; asrs #0x10` on the index),
 * not the first. agbcc's PROMOTE_MODE zero-extends every sub-word parameter
 * regardless of signedness, which is why an `s16` parameter carries an unsigned
 * narrowing at entry and a signed one at its use.
 *
 * The address shape (`ldr` base, index math, `adds r0, #4` on the BASE) is the
 * hoisted `g[i].member` row of the fold table -- a word member of an ARRAY
 * global under a variable index -- so the plain subscript is the right
 * spelling and no pointer local is wanted.
 */
bool8 EventOp_EndCurrentArmyTurn(s16 a)
{
    EndCurrentArmyTurn();

    gUnknown_0200C528[a].unk04++;

    return FALSE;
}
asm(".global sub_08018F04\n.thumb_set sub_08018F04, EventOp_EndCurrentArmyTurn\n");
