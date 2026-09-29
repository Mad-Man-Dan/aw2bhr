#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080202A4.
 * sub_080202A4 @ 0x080202A4
 */

/* The unit-record dispatch of GenerateUnitExtendedMovementMap's family, with the fourth argument
 * clamped: the record's own 7-bit unk06_0 unless that is already at or above
 * the type's GetUnitMovementWithCoBonus ceiling, in which case the ceiling.
 *
 * The clamp has to land in a `u16` LOCAL rather than in the argument
 * expression. Written as one `(s16)(a < f() ? a : (u16)f())` the compiler folds
 * the (s16) into the (u16) and the ROM's second narrowing disappears; a u16
 * local keeps the two apart -- `lsls #0x10; lsrs #0x10` when the call result is
 * stored, `lsls #0x10; asrs #0x10` at the use -- which is exactly the ROM, and
 * it is also what moves the merged value into r3 with no extra copy.
 *
 * The parameter is declared `struct Unk030040D8 *` because that is what every
 * caller hands it; the body casts to the identical `struct Unit` to
 * reach unk06_0 and the pointer difference, the same way c_08074320.c does. */
void GenerateUnitMovementMap(struct Unk030040D8 *a1)
{
    struct Unit *e;
    u16 v;

    e = (struct Unit *)a1;

    if (e->fuel < GetUnitMovementWithCoBonus(((e - gUnits) >> 6) + 1, e->type))
        v = e->fuel;
    else
        v = GetUnitMovementWithCoBonus(((e - gUnits) >> 6) + 1, e->type);

    gUnknown_030013EC(e->x, e->y, e->type, (s16)v, 1);
}
asm(".global sub_080202A4\n.thumb_set sub_080202A4, GenerateUnitMovementMap\n");
