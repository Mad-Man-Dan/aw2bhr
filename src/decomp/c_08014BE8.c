#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014BE8.
 * sub_08014BE8 @ 0x08014BE8
 */

#include "proc.h"

bool8 IsTextSkipAllowed(void)
{
    if (Proc_Find(ProcScr_DialogueOnEnd))
        return TRUE;

    if (Proc_Find(gUnknown_0848A150))
        return FALSE;

    if (FindSlotScript((s32)gUnknown_0848A130) != -1)
        return TRUE;

    if (FindSlotScript((s32)gUnknown_0848A120) != -1)
        return FALSE;

    if (FindEventScriptSlot(gUnknown_0849A520) != -1)
        return FALSE;

    if (FindEventScriptSlot(gUnknown_0849A5E0) != -1)
        return FALSE;

    if (FindEventScriptSlot(gUnknown_0849A8F0) != -1)
        return FALSE;

    if (FindSlotScript((s32)gUnknown_084C1824) != -1)
        return FALSE;

    if (FindSlotScript((s32)gUnknown_0849E240) != -1)
        return FALSE;

    if (sub_080366DC() == MapMainLoopCallback)
        return TRUE;

    return FALSE;
}
asm(".global sub_08014BE8\n.thumb_set sub_08014BE8, IsTextSkipAllowed\n");
