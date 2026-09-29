#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C8BC.
 * sub_0802C8BC @ 0x0802C8BC
 */

/* "Is the cell under the cursor occupied": the gUnknown_08499590 map read in
 * its usual three-step form (src/decomp/c_08001158.c), against the +0x12 unit
 * plane and the gUnknown_03003100 cursor. Read UNSIGNED -- both halves come
 * back with a plain `ldrh`, which is the union's `pos` view and not `spos`. */
bool8 UnitMenu_WaitUsability(void)
{
    int off;

    off = gMap->rowOffset[gUnknown_03003100.pos.unk02] + gUnknown_03003100.pos.unk00;

    if (gMap->unit[off] == 0)
        return FALSE;

    return TRUE;
}
asm(".global sub_0802C8BC\n.thumb_set sub_0802C8BC, UnitMenu_WaitUsability\n");
