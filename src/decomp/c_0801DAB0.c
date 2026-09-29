#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DAB0.
 * sub_0801DAB0 @ 0x0801DAB0
 */

/* The claim half of the affine-matrix slot allocator whose release half is
 * ClearObjAffineSlots: scan gUnknown_03001430 downwards for a free slot, mark it and
 * return its index, or -1 when all 0x20 are taken.
 */
int AllocObjAffineSlot()
{
    s16 i;

    for (i = 0x1F; i >= 0; i--)
    {
        if (gUnknown_03001430[i] == 0)
        {
            gUnknown_03001430[i] = 1;
            return i;
        }
    }
    return -1;
}
asm(".global sub_0801DAB0\n.thumb_set sub_0801DAB0, AllocObjAffineSlot\n");
