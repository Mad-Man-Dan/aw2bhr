#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E6C8.
 * sub_0806E6C8 @ 0x0806E6C8
 */

#include "proc.h"
struct Unk806E6C8
{
    /* 0x00 */ u8 filler_00[0x5c];
    /* 0x5c */ s32 unk_5c;
};

void StartSoundRoomCoSwap(s32 arg, ProcPtr parent)
{
    struct Unk806E6C8 * proc = Proc_StartBlocking(gUnknown_08582BB4, parent);

    proc->unk_5c = arg;
}
asm(".global sub_0806E6C8\n.thumb_set sub_0806E6C8, StartSoundRoomCoSwap\n");
