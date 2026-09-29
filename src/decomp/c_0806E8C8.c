#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E8C8.
 * sub_0806E8C8 @ 0x0806E8C8
 */

#include "proc.h"
struct Unk806E8C8
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ s32 unk_58;
    /* 0x5c */ s32 unk_5c;
    /* 0x60 */ s32 unk_60;
};

void StartSoundRoomArrows(s32 arg, ProcPtr parent)
{
    struct Unk806E8C8 * proc = Proc_Start(gUnknown_08582C24, parent);

    proc->unk_5c = 0;
    proc->unk_58 = 0;
    proc->unk_60 = arg;
}
asm(".global sub_0806E8C8\n.thumb_set sub_0806E8C8, StartSoundRoomArrows\n");
