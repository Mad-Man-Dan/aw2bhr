#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802B6C8.
 * sub_0802B6C8 @ 0x0802B6C8
 */

/* Classifies the cell at (x, y). The `* 0x55555555; rsbs; asrs #8` run is ONE
 * expression, not two: agbcc folds the exact division by the 0x0c element
 * stride that `unit - gUnits` needs into the `>> 6` that recovers
 * the army, so the whole thing lands as a single shift-and-add chain.
 *
 * `army` has to be a BINDING LOCAL. Written inline as the third argument the
 * instructions are identical but agbcc emits it AFTER the two simple
 * arguments, where the ROM computes it first; the local also moves the
 * gUnits base from r3 to the r1 the ROM uses.
 *
 * The (u8) is real, not a tidy-up: ShouldDrawTransportMarker's third parameter is u16 and
 * the ROM truncates with `lsls #0x18; lsrs #0x18`. The prototype is right --
 * the promoted DrawCursorInfoUnitIcon passes an s16 there with no truncation at all. */
/* WAVE 35: CANONICAL `struct Map`. Eight drafts across blocks 0x08029-0x0802B
 * each invented their own body for this tag, with 2 to 7 named fields. Every
 * one compiles and byte-matches ALONE, so trymatch cannot see the problem;
 * merged into a promoted unit it is a hard `redefinition of struct Map`.
 * All seven distinct bodies describe the SAME layout at different
 * resolutions -- the offsets reconcile exactly -- so this is their union, the
 * most refined reading of each region. Byte-neutral: only a field's START
 * OFFSET enters the address arithmetic, never its declared length, and no
 * draft referenced any filler. Keep the drafts in sync; sync_work.py
 * reintroduces whatever the drafts say. */

u8 GetCursorInfoPanelSizeClass(u8 x, u8 y)
{
    struct Unit *unit;
    u8 army;
    int idx;

    idx = x + gMap->rowOffset[y];

    if (gMap->unit[idx] == 0)
        return 0;

    unit = &gUnits[gMap->unit[idx]];
    army = ((unit - gUnits) >> 6) + 1;

    if (ShouldDrawTransportMarker(unit->type, gUnknown_030033EC, army))
        return 2;

    if (unit->unk07 | unit->unk08)
        return 2;

    return 1;
}
asm(".global sub_0802B6C8\n.thumb_set sub_0802B6C8, GetCursorInfoPanelSizeClass\n");
