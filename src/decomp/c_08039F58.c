#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039F58.
 * sub_08039F58 @ 0x08039F58
 */

/* The single `movs r1, #0` serves both scroll shadows: agbcc reuses a register
 * it already knows holds zero rather than reloading it, so the literal is
 * written twice here and emitted once. gUnknown_03001FF8 being volatile costs
 * nothing on a plain store. */

void ApplyCoPowerStatus(void)
{
    gUnknown_03001FF8 = 0;
    gUnknown_03001418 = 0;

    SetPlayerCoPowerStatus(gUnknown_030033EC);
    RebuildMapUnitLayers2();
}
asm(".global sub_08039F58\n.thumb_set sub_08039F58, ApplyCoPowerStatus\n");
