#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806377C.
 * sub_0806377C @ 0x0806377C, sub_080637AC @ 0x080637AC, sub_080637D8 @ 0x080637D8
 */

/* The descending twin of sub_0801537C (src/decomp/c_0801537C.c): the same
 * gUnknown_03001470[] scan keyed on `.unk00` holding the blob the slot was
 * created for, walked 29..0 instead of 0..29 and acting on EVERY hit rather
 * than returning at the first. The cursor's `base + 0xAE0` init and the
 * `subs #0x60` step are strength_reduce's, not source: 0x1D * 0x60 == 0xAE0
 * exactly, so this is a plain `gUnknown_03001470[i]` subscript. */
void ClearCallbackOfSlotsRunningScript(const void *a)
{
    int i;

    for (i = 0x1d; i >= 0; i--)
    {
        if (gUnknown_03001470[i].unk00 == (u32)a)
            ClearSlotScriptCallback(i);
    }
}
asm(".global sub_0806377C\n.thumb_set sub_0806377C, ClearCallbackOfSlotsRunningScript\n");

/* The same descending gUnknown_03001470[] scan as ClearCallbackOfSlotsRunningScript, but it hands
 * back the slot instead of acting on it -- so it returns at the first hit and
 * falls off the bottom of the loop to NULL. */
struct Unk03001470 *FindSlotRunningScript(const void *a)
{
    int i;

    for (i = 0x1d; i >= 0; i--)
    {
        if (gUnknown_03001470[i].unk00 == (u32)a)
            return &gUnknown_03001470[i];
    }

    return NULL;
}
asm(".global sub_080637AC\n.thumb_set sub_080637AC, FindSlotRunningScript\n");

/* ClearCallbackOfSlotsRunningScript with StepSlotScript in place of ClearSlotScriptCallback -- the two differ in
 * that one instruction and nothing else. */
void StepSlotsRunningScript(const void *a)
{
    int i;

    for (i = 0x1d; i >= 0; i--)
    {
        if (gUnknown_03001470[i].unk00 == (u32)a)
            StepSlotScript(i);
    }
}
asm(".global sub_080637D8\n.thumb_set sub_080637D8, StepSlotsRunningScript\n");
