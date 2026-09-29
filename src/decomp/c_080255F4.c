#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080255F4.
 * sub_080255F4 @ 0x080255F4
 */

#include "hardware.h"

/* "Is the unit at (ax, ay) boxed in?" -- returns 1 unless the unit is a live,
 * friendly, non-hostile-terrain one with at least one blocked neighbour.
 *
 * `id` has to be a u16 LOCAL and the shift has to be written `>> 6`, not as a
 * mask. The ROM computes `lsls r2, r0, #0x10` where the pointer difference is
 * produced -- two basic blocks before its only use -- and then `lsrs r1, r2,
 * #0x16` at the use. That is the u16 truncation left behind at the assignment
 * after combine merged its `lsr #16` with the `>> 6` at the use; spelling the
 * whole thing inline puts both shifts at the use. gUnits is grouped
 * 64 entries per army, so `id >> 6` is the army and the `+ 1` is the usual
 * 1-based army id, which agbcc folds into `adds r0, #0x58` (0x3c + 0x1c).
 *
 * ax and ay are s16 PARAMETERS, not int: the prologue narrows both with
 * `lsls #0x10; lsrs #0x10` (PROMOTE_MODE) and every use re-narrows with
 * `lsls #0x10; asrs #0x10`. The re-narrowing survives on `ax - 1` / `ay - 1`
 * and is dropped on the bare values, which is num_sign_bit_copies and not a
 * source difference -- do not add casts to chase it.
 *
 * The four neighbour tests are family F061's shape (see src/decomp/c_0800B5C0.c)
 * with a u16 accumulator: the first is a plain assignment and the rest are
 * `+=`, which is what puts the lone `lsls #0x18; lsrs #0x18` on the first call
 * and the `(u16)` re-truncation on the other three. */
u8 IsUnitVisibleToViewer(struct Unit *unit, s16 ax, s16 ay)
{
    u16 total = 0;
    u16 id = unit - gUnits;
    int idx;

    if (unit == NULL)
        return 1;

    if (!(unit->flags & 0x20))
        return 1;

    if (gPlayers[(id >> 6) + 1].turnState & 2)
        return 1;

    idx = gMap->rowOffset[ay] + ax;

    if (gPlayers[gMap->terrain[idx] >> 5].turnState & 2)
        return 1;

    if (ax > 0)
        total = IsViewerUnitAtCell(ax - 1, ay);

    if (ay > 0)
        total += IsViewerUnitAtCell(ax, ay - 1);

    if (ax < gMap->width - 1)
        total += IsViewerUnitAtCell(ax + 1, ay);

    if (ay < gMap->height - 1)
        total += IsViewerUnitAtCell(ax, ay + 1);

    if (total != 0)
        return 1;

    return 0;
}
asm(".global sub_080255F4\n.thumb_set sub_080255F4, IsUnitVisibleToViewer\n");
