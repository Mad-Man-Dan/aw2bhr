#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CB40.
 * sub_0803CB40 @ 0x0803CB40
 */

/* The setter for the gUnknown_030033F4 bit block that the promoted
 * IsCampaignFlagBank0Set reads, and it shares that function's two oddities: the index is
 * `id >> 3` with an ARITHMETIC shift (so `int`, not unsigned), and the shift
 * count is the WHOLE id rather than `id & 7`.
 *
 * The address expression belongs inside each arm, not before the `if`: the ROM
 * carries TWO pool words for gUnknown_030033F4 and recomputes `asrs r2, r3, #3`
 * in both branches. Hoisting it to a local computes it once and drops the
 * second pool word (measured). The shared `strb r0, [r2]` tail is gcc's
 * cross-jumping of the two identical stores, not a ternary. */
void SetCampaignFlagBank0(int id, int set)
{
    if (set)
        gUnknown_030033F4[id >> 3] |= 1 << id;
    else
        gUnknown_030033F4[id >> 3] &= ~(1 << id);
}
asm(".global sub_0803CB40\n.thumb_set sub_0803CB40, SetCampaignFlagBank0\n");
