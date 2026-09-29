#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D458.
 * sub_0802D458 @ 0x0802D458
 */

/* Byte-identical duplicate of sub_0802D558 (gUnknown_0849AE28), and the
 * four-argument sibling of OpenOptionsMenu / OpenIntelMenu. */
void OpenMapMenu(void)
{
    u16 v;

    v = 1;

    if (gUnknown_030033E4.unk00 - gMap->scrollX / 16 <= 6)
        v = 0x14;

    RebuildMapUnitLayers2();
    CreateRootMenuWithSfx(gUnknown_0849AAC0, v, 1, 1);
    IncrementMapLock();
}
asm(".global sub_0802D458\n.thumb_set sub_0802D458, OpenMapMenu\n");
