#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806502C.
 * sub_0806502C @ 0x0806502C
 */

/* One call whose first argument is a nested call. The three reads off the
 * parameter all happen AFTER `bl sub_080152EC`, which is what puts the nested
 * call in argument position 1 rather than in a statement of its own.
 *
 * `movs r2, #0x28; ldrsh r1, [r4, r2]` -- an `ldrsh` whose offset is in a
 * REGISTER is a signed halfword at a constant displacement, so unk28 (s16),
 * not an array index. unk1c is a bare `ldrh` (u16), loaded once and used for
 * both the third and fourth arguments.
 *
 * The `(struct Unk08580934_Obj *)` cast is the marker for the open type
 * conflict recorded beside TeamBadge_Setup in include/unknown-functions.h --
 * sub_080152EC is declared to return `struct Unk03001470 *` and the two
 * structs are almost certainly one object. A pointer cast emits nothing, so
 * the cast costs the match nothing and does not prejudge the unification. */
void SpawnTeamBadgeForColumn(struct Unk08580934_Obj *obj)
{
    TeamBadge_Setup((struct Unk08580934_Obj *)sub_080152EC(gUnknown_08580A38, 3),
                 obj->unk28 + 8, obj->unk1c, obj->unk1c * 2);
}
asm(".global sub_0806502C\n.thumb_set sub_0806502C, SpawnTeamBadgeForColumn\n");
