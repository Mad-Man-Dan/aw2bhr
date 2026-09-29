#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078DD8.
 * sub_08078DD8 @ 0x08078DD8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* Bumps one counter and drops another, but only on the frames bit 0 of
 * gGameClock selects. MissionTitle_ScrollBackdrop_Loop is the same function on a different
 * pair of counters. */

void ScrollBG3_IDLE_08078DD9(void)
{
    if (gGameClock & 1)
    {
        gUnknown_0300200C++;
        gUnknown_03002000--;
    }
}

asm(".global sub_08078DD8\n.thumb_set sub_08078DD8, ScrollBG3_IDLE_08078DD9\n");
