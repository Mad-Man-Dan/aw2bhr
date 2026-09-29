#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DAE8.
 * sub_0801DAE8 @ 0x0801DAE8
 */

void FreeObjAffineSlot(s16 a)
{
    if (a != -1)
        gUnknown_03001430[a] = 0;
}
asm(".global sub_0801DAE8\n.thumb_set sub_0801DAE8, FreeObjAffineSlot\n");
