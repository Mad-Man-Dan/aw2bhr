#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015C00.
 * sub_08015C00 @ 0x08015C00
 */

/* The predicate twin of FindSlotScript, which scans the same 30 slots for the
 * same key and returns the index instead of a flag.
 */
bool8 sub_08015C00(s32 a)
{
    u8 i;

    for (i = 0; i < 30; i++)
    {
        if (gUnknown_03001470[i].unk00 == a)
            return TRUE;
    }

    return FALSE;
}
