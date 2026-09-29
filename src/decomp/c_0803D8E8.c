#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D8E8.
 * sub_0803D8E8 @ 0x0803D8E8, sub_0803D8F8 @ 0x0803D8F8
 */

#include "proc.h"
/* Runs the callback StartSaveScreen parked at +0x4c. `bl _call_via_r0` is a
 * NULLARY indirect call -- gcc puts the pointer in the first free scratch
 * register, so r0 means no arguments. */
struct UnkD8E8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ void (*unk4c)(void);
};

void SaveScreen_RunCallback(struct UnkD8E8Proc *proc)
{
    if (proc->unk4c != NULL)
        proc->unk4c();
}
asm(".global sub_0803D8E8\n.thumb_set sub_0803D8E8, SaveScreen_RunCallback\n");

/* Reads the byte SaveScreen_StartMessage/SaveScreenCampaign_StartMessage stashed at struct Unk0200C528's
 * +0x10 -- signed for the `== 6` test (`ldrsb`), then re-loaded `ldrb` as
 * DeleteSuspendSave's `u8` argument. The two loads are the member's own s8 type and
 * a cast at the second use. */
void SaveMessage_CommitProgress(struct Unk0200C528 *p)
{
    if (p->unk10 == 6)
        WriteProfile();
    else
        DeleteSuspendSave(p->unk10);
}
asm(".global sub_0803D8F8\n.thumb_set sub_0803D8F8, SaveMessage_CommitProgress\n");
