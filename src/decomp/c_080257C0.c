#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080257C0.
 * sub_080257C0 @ 0x080257C0
 */

#include "hardware.h"

/* The unit-id form of IsUnitVisibleToViewer's "is this unit boxed in?" test, and NOT a
 * sibling of it -- the callee sets are disjoint and only the four-neighbour
 * tail is shared (family F061's shape, see src/decomp/c_0800B5C0.c, with a u16
 * accumulator).
 *
 * Three things here are not free, and all three are register-allocation facts
 * rather than semantics:
 *
 * 1. `x` and `y` are `s16` LOCALS, not `int`. Both spellings emit the identical
 *    `ldrb` pair and the identical signed `cmp; ble`, because the values are
 *    provably 0..255 either way. The difference is that the s16 spelling puts
 *    `x` in r8 and shuffles it into r4 at the first use, which is what the ROM
 *    does; with `int` the pair lands in r5/r4 and no high register is pushed.
 *
 * 2. IsVisibleAllyUnitAtCell's parameters are `int`. This function is the only evidence
 *    for that width and it is an ORDERING readout, not a narrowing one: every
 *    argument here is `ldrb`-derived so an s16 conversion would be elided and
 *    invisible. With `int` parameters the ROM's `subs r1, r5, #1; adds r0, r4,
 *    #0` order falls out on the two calls whose SECOND argument carries the
 *    arithmetic; with `s16` the two come out in argument-number order instead.
 *
 * 3. The whole map lookup is sequenced INSIDE the second argument with commas.
 *    The ROM evaluates IsTerrainOwnedByUnitsTeam's first argument -- including the `ldrsh`,
 *    not just its address -- before the map base is even loaded, and no
 *    statement form reaches that: written as statements ahead of the call the
 *    map goes first, and binding the first argument to a local of any width
 *    leaves the address in ip and lets CSE sink the `ldrsh` down to the call
 *    (the load has one use and nothing writes memory in between). The commas
 *    are almost certainly a macro in the original; what matters is that they
 *    are the only spelling found that emits the LOAD where the ROM has it. The
 *    map lookup is gMap->terrain[gMap->rowOffset[y] + x]. */
u8 IsUnitVisibleToCurrentTeam(u16 id)
{
    struct Unit *unit = &gUnits[id];
    u16 total = 0;
    s16 x = unit->x;
    s16 y = unit->y;

    if (unit == NULL)
        return 1;

    if (!(unit->flags & 0x20))
        return 1;

    if (IsUnitOnCurrentTeam(unit - gUnits))
        return 1;

    if (IsTerrainOwnedByUnitsTeam(gUnknown_084995FE[gUnknown_030033EC],
                     gMap->terrain[gMap->rowOffset[y] + x]))
        return 1;

    if (x > 0)
        total = IsVisibleAllyUnitAtCell(x - 1, y);

    if (y > 0)
        total += IsVisibleAllyUnitAtCell(x, y - 1);

    if (x < gMap->width - 1)
        total += IsVisibleAllyUnitAtCell(x + 1, y);

    if (y < gMap->height - 1)
        total += IsVisibleAllyUnitAtCell(x, y + 1);

    if (total != 0)
        return 1;

    return 0;
}
asm(".global sub_080257C0\n.thumb_set sub_080257C0, IsUnitVisibleToCurrentTeam\n");
