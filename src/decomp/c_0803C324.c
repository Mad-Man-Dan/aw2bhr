#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C324.
 * sub_0803C324 @ 0x0803C324, sub_0803C33C @ 0x0803C33C
 */

/* One of the 0x0803C324-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). This one has no "not yet" arm, so it is the
 * two-block form -- byte-identical to the already-matched ShopAvail_MapNotOwned in
 * src/decomp/c_0803C354.c, and to its own neighbour sub_0803C33C.
 *
 * `lsls r0, r0, #0x18` after the `bl` is IsCampaignMapUnlockedByMapData's declared u8 return, and
 * the `bne` skipping to the `movs #1; rsbs` block is why the -1 is written
 * second in the source. */

int sub_0803C324(u32 id)
{
    if (IsCampaignMapUnlockedByMapData(id))
        return -1;
    return 1;
}

/* Byte-identical to sub_0803C324 next door and to ShopAvail_MapNotOwned in
 * src/decomp/c_0803C354.c -- same callee, same constants. One of the
 * 0x0803C324-0x0803C670 tri-state predicates. */

int sub_0803C33C(u32 id)
{
    if (IsCampaignMapUnlockedByMapData(id))
        return -1;
    return 1;
}
