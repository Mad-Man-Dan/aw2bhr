#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CBA0.
 * sub_0803CBA0 @ 0x0803CBA0
 */

/* The three-range bit-id dispatch. Each `bhi` is agbcc's biased unsigned form
 * of the two-sided range test, so the source reads as the pair of bounds.
 *
 * The prototype `void (int, int)` was already fixed by the promoted caller
 * EventOp_SetCampaignFlag and this definition agrees with it: neither parameter is
 * narrowed here. The `lsls #0x18; lsrs #0x18` on the value before the first
 * two calls is SetCampaignFlagBank2's and SetCampaignFlagBank1's `u8` showing through, and its
 * absence before the third is SetCampaignFlagBank0's `int`. */
void SetCampaignCompletionFlag(int id, int value)
{
    if (id >= 0x60 && id <= 0x9f)
        SetCampaignFlagBank2(id - 0x60, value);
    else if (id >= 0x20 && id <= 0x5f)
        SetCampaignFlagBank1(id - 0x20, value);
    else if ((u32)id <= 0x1f)
        SetCampaignFlagBank0(id, value);
}
asm(".global sub_0803CBA0\n.thumb_set sub_0803CBA0, SetCampaignCompletionFlag\n");
