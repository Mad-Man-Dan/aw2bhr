#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E728.
 * sub_0806E728 @ 0x0806E728
 */

#include "proc.h"
struct Unk806E728
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ ProcPtr unk_54;
};

void StartSoundRoomLevelPulse(ProcPtr parent)
{
    struct Unk806E728 * proc = Proc_Start(gUnknown_08582BE4, parent);

    proc->unk_54 = parent;
}
asm(".global sub_0806E728\n.thumb_set sub_0806E728, StartSoundRoomLevelPulse\n");
