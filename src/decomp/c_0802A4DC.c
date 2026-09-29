#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A4DC.
 * sub_0802A4DC @ 0x0802A4DC
 */

/* Both arguments come off ONE index computation -- the repeated
 * `gPlayers[gUnknown_030033EC]` spelling is what gives that here,
 * because nothing between the two reads can invalidate the cached address (the
 * same reason src/decomp/c_08026588.c keeps the plain spelling). unk2f and
 * unk30 were filler until wave 29; they are named from this call. */

void ScrollCameraToCurrentArmyCursor(void)
{
    ScrollCameraToKeepCellInView(gPlayers[gUnknown_030033EC].cursorX,
                 gPlayers[gUnknown_030033EC].cursorY);
}
asm(".global sub_0802A4DC\n.thumb_set sub_0802A4DC, ScrollCameraToCurrentArmyCursor\n");
