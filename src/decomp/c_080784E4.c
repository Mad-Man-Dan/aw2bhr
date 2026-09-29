#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080784E4.
 * sub_080784E4 @ 0x080784E4
 */

#include "proc.h"
struct Unk80784E4
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ s32 unk_58;
    /* 0x5c */ s32 unk_5c;
    /* 0x60 */ s32 unk_60;
};

void StartBlockingCoSpeech(s32 a, s32 b, s32 c, ProcPtr parent)
{
    struct Unk80784E4 * proc = Proc_StartBlocking(gUnknown_08615A8C, parent);

    proc->unk_58 = a;
    proc->unk_5c = b;
    proc->unk_60 = c;
}
asm(".global sub_080784E4\n.thumb_set sub_080784E4, StartBlockingCoSpeech\n");
