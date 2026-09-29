#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805F4CC.
 * sub_0805F4CC @ 0x0805F4CC
 */

/* Dispatch through a ROM table of nullary handlers, guarded by a clamp on the
 * cursor byte. CycleWeather's shape one step further out: a bounds fix on a
 * counter held in a global record, then a call parameterised by it.
 *
 * The byte at +0x0b is struct Unk030040D8's unk07[4] -- a CONSTANT subscript, so
 * it folds into the `ldrb`/`strb` displacement and shows none of the
 * `adds rB,#7` member-array tell that a variable index would leave.
 *
 * `> 7` with the reset as the THEN arm, exactly as in CycleWeather: the `<= 7`
 * spelling emits the mirror branch. `bls` is UNSIGNED, agreeing with the u8.
 *
 * `bl _call_via_r0` with no argument register written before it -- r0 index is
 * the arity readout, so the table's entries are nullary.
 */

void AiRunRoleMove(void)
{
    if (gUnknown_030040D8->unk07[4] > 7)
        gUnknown_030040D8->unk07[4] = 1;

    gUnknown_085768E0[gUnknown_030040D8->unk07[4]]();
}
asm(".global sub_0805F4CC\n.thumb_set sub_0805F4CC, AiRunRoleMove\n");
