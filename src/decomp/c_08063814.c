#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063814.
 * sub_08063814 @ 0x08063814
 */

/* The same descending gUnknown_03001470[] scan, inverted: it acts on every
 * slot that is NOT the argument's (`beq` skips the call). EndSlotScriptAt takes
 * s16, which is where `lsls #0x10; asrs #0x10` comes from -- the u8 narrowing
 * its twins emit is ClearSlotScriptCallback/StepSlotScript's u8 parameter, not a different
 * index type. */
void EndSlotsNotRunningScript(const void *a)
{
    int i;

    for (i = 0x1d; i >= 0; i--)
    {
        if (gUnknown_03001470[i].unk00 != (u32)a)
            EndSlotScriptAt(i);
    }
}
asm(".global sub_08063814\n.thumb_set sub_08063814, EndSlotsNotRunningScript\n");
