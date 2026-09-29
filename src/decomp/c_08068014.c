#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08068014.
 * sub_08068014 @ 0x08068014
 */

#include "proc.h"
/* r1 is never written before the `bl`, so the parent is this starter's own
 * fourth parameter, passed straight through. */
struct Unk68014Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
    /* 30 */ s32 unk30;
    /* 34 */ s32 unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x40);
    /* 40 */ s32 unk40;
};

void StartIntroBlendFade(s32 a, s32 b, s32 c, ProcPtr parent)
{
    struct Unk68014Proc * proc = Proc_Start(gUnknown_08581138, parent);

    proc->unk2c = a;
    proc->unk30 = b;
    proc->unk34 = c;
    proc->unk40 = 0;
}
asm(".global sub_08068014\n.thumb_set sub_08068014, StartIntroBlendFade\n");
