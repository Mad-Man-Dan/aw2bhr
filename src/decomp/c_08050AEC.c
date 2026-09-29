#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050AEC.
 * sub_08050AEC @ 0x08050AEC
 */

void EndProjectileEffect(u16 a1, u16 a2, s16 a3)
{
    if (gUnknown_02029A10[a1].entries[a2].unk1a == 0)
    {
        if (a1 == gUnknown_0300450C)
            gUnknown_02029664 |= 1;
        else
            gUnknown_02029664 |= 8;
    }

    if (a3 != -1 && IsSlotScriptActiveAt(a3) != 0)
        EndSlotScriptAt(a3);
}
asm(".global sub_08050AEC\n.thumb_set sub_08050AEC, EndProjectileEffect\n");
