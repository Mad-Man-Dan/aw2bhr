#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804599C.
 * sub_0804599C @ 0x0804599C
 */

/* The gUnknown_08499590 map read in its usual three-step form (see
 * src/decomp/c_08001158.c): row-offset table at +0x417A indexed by y*2, plus
 * x, into a plane base. The plane here is +0x1432, the 5-bit terrain code
 * gUnknown_08551CA0 is keyed by, and the cursor is
 * gUnknown_030040D8->unk02 / ->unk03. */
int MapEventCond_SelectedUnitOnHq(void)
{
    int off;

    off = gMap->rowOffset[gUnknown_030040D8->unk03] + gUnknown_030040D8->unk02;

    if ((gMap->terrain[off] & 0x1F) == 8)
        return 1;

    return 0;
}
asm(".global sub_0804599C\n.thumb_set sub_0804599C, MapEventCond_SelectedUnitOnHq\n");
