#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073F90.
 * sub_08073F90 @ 0x08073F90
 */

void GetSoundScopeLevels(u16 * x, u16 * y)
{
    *x = gUnknown_0202FDE8;
    *y = gUnknown_0202FDEA;
}
asm(".global sub_08073F90\n.thumb_set sub_08073F90, GetSoundScopeLevels\n");
