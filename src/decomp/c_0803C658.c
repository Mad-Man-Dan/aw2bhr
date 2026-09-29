#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C658.
 * sub_0803C658 @ 0x0803C658
 */

/* `if (IsCampaignFlagBank2Set(id)) return -1; return 1;`
 *
 * The `lsls r0, r0, #0x18` between the `bl` and the `cmp` is the whole type
 * signal here: IsCampaignFlagBank2Set is one of the gUnknown_02028030 single-bit readers
 * and its declared return type is eight bits wide, so testing the result
 * re-narrows it. An `int` return emits no such shift.
 *
 * Branch polarity: the `bne` jumps to the -1 block and the fallthrough is 1,
 * which is the `if (C) return A; return B;` layout with A = -1. Writing the
 * inverse (`if (!f(id)) return 1; return -1;`) swaps the two blocks and misses.
 */

int sub_0803C658(u32 id)
{
    if (IsCampaignFlagBank2Set(id))
        return -1;

    return 1;
}
