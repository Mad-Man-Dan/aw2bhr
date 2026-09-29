#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078498.
 * sub_08078498 @ 0x08078498
 */

#include "proc.h"
/* Reuses src/decomp/c_080784E4.c's proc layout exactly -- the three `s32`s at
 * +0x58/+0x5c/+0x60 that StartBlockingCoSpeech stores are what this method reads back.
 * The three shift pairs at the StartCoSpeechScript call are the narrowing that
 * callee's own prologue demands (u16, u8, u8); they are NOT casts in this
 * source, and they are what proves the proc fields are wider than the
 * parameters rather than narrow themselves. */

struct Unk80784E4
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ s32 unk_58;
    /* 0x5c */ s32 unk_5c;
    /* 0x60 */ s32 unk_60;
};

void BlockingCoSpeech_Init(struct Unk80784E4 *proc)
{
    gUnknown_03002F08.unk00 = 0;
    StartDialogueBlock(proc);
    StartCoSpeechScript(proc->unk_58, proc->unk_5c, proc->unk_60);
}
asm(".global sub_08078498\n.thumb_set sub_08078498, BlockingCoSpeech_Init\n");
