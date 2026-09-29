#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025C5C.
 * sub_08025C5C @ 0x08025C5C, sub_08025C98 @ 0x08025C98, CreateUnitAt @ 0x08025CC8
 */

/* An early `return NULL` and not `if (u != NULL) { ...; return u; }`: the
 * inverted spelling puts the `movs r0,#0` in the FALL-THROUGH and the body past
 * the branch, which is the mirror image of the ROM. Written as a guard clause
 * the body falls through and the zero sits after the literal pool, as here.
 *
 * The three parameters are `s16`. Nothing in this body can tell s16 from u16 --
 * PROMOTE_MODE emits the same `lsls #0x10; lsrs #0x10` for both (probed) and
 * the values only ever reach a `strb` -- so the evidence is entirely at the two
 * callers, CreateExhaustedUnitAt and CreateUnitAt, which SIGN-extend all three before
 * the `bl`. `u16` here makes both of them emit `lsrs` instead.
 *
 * The `lsls #0x18; lsrs #0x18` on a3 is the conversion to InitUnit's u8. */

struct Unit *CreateUnitAtNoRefresh(s16 a1, s16 a2, s16 a3)
{
    struct Unit *u = FindFreeUnitSlot();

    if (u == NULL)
        return NULL;

    InitUnit(u, a3);

    u->x = a1;
    u->y = a2;

    IncrementPlayerUnitsCreated(gUnknown_030033EC);

    return u;
}
asm(".global sub_08025C5C\n.thumb_set sub_08025C5C, CreateUnitAtNoRefresh\n");

/* Same guard-clause shape as CreateUnitAtNoRefresh: `if (u == NULL) return NULL;` puts
 * the body in the fall-through and the `movs r0,#0` at the end. The inverted
 * `if (u != NULL) { ...; return u; } return NULL;` swaps the two blocks and
 * misses by 20 bytes -- measured, not assumed.
 *
 * `orrs` on a bare `movs r0,#1`, so unk01 bit 0 is a plain mask and not a
 * bitfield. */

void *CreateExhaustedUnitAt(s16 a1, s16 a2, s16 a3)
{
    struct Unit *u = CreateUnitAtNoRefresh(a1, a2, a3);

    if (u == NULL)
        return NULL;

    u->flags |= 1;
    RebuildMapUnitLayers();

    return u;
}
asm(".global sub_08025C98\n.thumb_set sub_08025C98, CreateExhaustedUnitAt\n");

/* CreateExhaustedUnitAt without the `unk01 |= 1` -- see there for the guard-clause
 * shape. */

/* Wave 32 (W32-B) RETYPES the return `void *` -> `struct Unit *`. It
 * returns CreateUnitAtNoRefresh's result unchanged, and that function is already
 * declared `struct Unit *` right here -- the `void *` was the weakest
 * type that fit when nothing read the result. CoPowerCreateUnits_SpawnUnit, promoted this
 * wave, writes `->unk04_0 = 0x5a` through it, which is the discriminating use.
 * Byte-neutral; re-verified.
 *
 * Named per Xenesis's AW2 Subroutine List: "Attempts to create a unit at the
 * input grid co-ordinates (r0 = x, r1 = y, r2 = UID)". The old CreateUnitAt
 * symbol is kept as a linker alias below so every other unit keeps
 * resolving it unchanged. */
struct Unit *CreateUnitAt(s16 a1, s16 a2, s16 a3)
{
    struct Unit *u = CreateUnitAtNoRefresh(a1, a2, a3);

    if (u == NULL)
        return NULL;

    RebuildMapUnitLayers();

    return u;
}

asm(".global sub_08025CC8\n.thumb_set sub_08025CC8, CreateUnitAt\n");
