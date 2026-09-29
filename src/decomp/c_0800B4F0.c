#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800B4F0.
 * sub_0800B4F0 @ 0x0800B4F0
 */

/* The `movs r4, #0` ahead of the base `ldr` is the initialiser hoisting to the
 * top of the function; `return cells[idx] == 0xD;` would materialise the zero
 * after the load instead (that is what sub_080094EC has). */
int IsShoalAt(int x, int y)
{
    int result = 0;
    int idx;

    idx = gMap->rowOffset[y] + x;

    if (gMap->terrain[idx] == 0xD)
        result = 1;

    return result;
}
asm(".global sub_0800B4F0\n.thumb_set sub_0800B4F0, IsShoalAt\n");
