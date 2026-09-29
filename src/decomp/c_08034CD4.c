#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034CD4.
 * sub_08034CD4 @ 0x08034CD4
 */

#include "proc.h"

/* A `switch` and not an `if` chain: the ROM's `cmp #0x8d beq / cmp #0x8d bgt /
 * cmp #0x8b bgt / cmp #0x8a blt` is agbcc's binary decision tree over the case
 * set {0x8a, 0x8b, 0x8d, 0x8f}, with 0x8a and 0x8b collapsed into one range
 * test. Written as `||` it would be four independent compares in source order.
 *
 * `lsls r0, #0x18; cmp r0, #0` with no `lsrs` is the test-the-low-byte form
 * agbcc uses for a bool8/u8-returning callee under `if (f())`, which agrees
 * with sub_08019260's declared `bool8`. */
void MapState_StartInventionTurnScript(void)
{
    switch (gPlaySt.mapID)
    {
    case 0x8a:
    case 0x8b:
    case 0x8d:
    case 0x8f:
        break;
    default:
        if (sub_08019260())
            return;
        break;
    }

    Proc_Start(gUnknown_0849F790, PROC_TREE_3);
    gUnknown_030032D8 = 0xc;
}
asm(".global sub_08034CD4\n.thumb_set sub_08034CD4, MapState_StartInventionTurnScript\n");
