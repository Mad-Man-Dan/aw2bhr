#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D4B0.
 * sub_0802D4B0 @ 0x0802D4B0, sub_0802D504 @ 0x0802D504, sub_0802D558 @ 0x0802D558
 */

/* Byte-identical duplicate of OpenIntelMenu, which names gUnknown_0849ABC0 and
 * gUnknown_030040F0 instead. Both symbols are real objects reached directly by
 * the pool word, not `.LC` reroutes, so this pair is a genuine two-source one.
 *
 * The guard divides a signed halfword by 16 (`cmp #0; bge; adds #0xf; asrs #4`)
 * and the `bgt` skips the assignment, so the source tests `<= 6`. The fifth
 * argument's `ldrh` off a u32 global is the implicit truncation to
 * sub_08019F2C's u16 parameter, folded into a narrower load. */
void OpenOptionsMenu(void)
{
    u16 v;

    v = 1;

    if (gUnknown_030033E4.unk00 - gMap->scrollX / 16 <= 6)
        v = 0x14;

    RebuildMapUnitLayers2();
    sub_08019F2C(gUnknown_0849AC60, v, 1, 1, gUnknown_030044A0);
    IncrementMapLock();
}
asm(".global sub_0802D4B0\n.thumb_set sub_0802D4B0, OpenOptionsMenu\n");

/* Byte-identical duplicate of OpenOptionsMenu -- see there for the shape. Only
 * the blob and the fifth argument's global differ. */
void OpenIntelMenu(void)
{
    u16 v;

    v = 1;

    if (gUnknown_030033E4.unk00 - gMap->scrollX / 16 <= 6)
        v = 0x14;

    RebuildMapUnitLayers2();
    sub_08019F2C(gUnknown_0849ABC0, v, 1, 1, gUnknown_030040F0);
    IncrementMapLock();
}
asm(".global sub_0802D504\n.thumb_set sub_0802D504, OpenIntelMenu\n");

/* Byte-identical duplicate of OpenMapMenu -- only the blob differs. */
void sub_0802D558(void)
{
    u16 v;

    v = 1;

    if (gUnknown_030033E4.unk00 - gMap->scrollX / 16 <= 6)
        v = 0x14;

    RebuildMapUnitLayers2();
    CreateRootMenuWithSfx(gUnknown_0849AE28, v, 1, 1);
    IncrementMapLock();
}
