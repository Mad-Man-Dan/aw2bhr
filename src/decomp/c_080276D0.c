#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080276D0.
 * sub_080276D0 @ 0x080276D0, sub_080276F0 @ 0x080276F0
 */

/* The 0 half of a pair with SnapInfoBoxToSide1, which is the same three lines with 1
 * and gUnknown_08090A98[1]. The `ldrsh` element is named twice in the source --
 * once into gUnknown_03003130.unk04 and once as DrawCoPanelWithDaysRemaining's argument -- and
 * CSE leaves it in r0 across the store. */
void SnapInfoBoxToSide0(void)
{
    gUnknown_03003130.unk00 = 0;
    gUnknown_03003130.unk04 = gUnknown_08090A98[0];
    DrawCoPanelWithDaysRemaining(gUnknown_08090A98[0]);
}
asm(".global sub_080276D0\n.thumb_set sub_080276D0, SnapInfoBoxToSide0\n");

/* The 1 half of the SnapInfoBoxToSide0 pair -- see there. */
void SnapInfoBoxToSide1(void)
{
    gUnknown_03003130.unk00 = 1;
    gUnknown_03003130.unk04 = gUnknown_08090A98[1];
    DrawCoPanelWithDaysRemaining(gUnknown_08090A98[1]);
}
asm(".global sub_080276F0\n.thumb_set sub_080276F0, SnapInfoBoxToSide1\n");
