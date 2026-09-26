#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E108.
 * SetObjAffine @ 0x0801E108
 */

void SetObjAffine(int index, s16 pa, s16 pb, s16 pc, s16 pd)
{
    gUnknown_03002520[index * 16 + 3] = pa;
    gUnknown_03002520[index * 16 + 7] = pb;
    gUnknown_03002520[index * 16 + 11] = pc;
    gUnknown_03002520[index * 16 + 15] = pd;
}
