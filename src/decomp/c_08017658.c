#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017658.
 * sub_08017658 @ 0x08017658
 */

/* The parameter is `struct Unk03001470 *`: a bare `adds r4, r0, #0` prologue
 * with no narrowing, then `ldrh` at +0x1e, which is that struct's `s16 unk1e`
 * and is exactly the field StartResumeScript next door writes with `strh`.
 * gUnknown_030033EC is a u16 read `ldrb` -- the truncation PlayArmyCoMusic's `u8`
 * parameter folds into the load. */
void ResumeScript_LoadSuspendSave(struct Unk03001470 *proc)
{
    InitGameSettings();
    LoadSuspendSave(proc->unk1e);
    ResetUnitCycleAndVisionMasks();
    sub_08023348();
    InstallMapFrameCallbacks();
    PlayArmyCoMusic(gUnknown_030033EC);
}
asm(".global sub_08017658\n.thumb_set sub_08017658, ResumeScript_LoadSuspendSave\n");
