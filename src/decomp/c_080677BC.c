#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080677BC.
 * sub_080677BC @ 0x080677BC
 */

#include "proc.h"
/* r1 is never written before the `bl`, so the parent is this starter's own
 * fourth parameter, passed straight through. `pop {r1}` rather than `pop {r0}`
 * says the proc pointer is returned.
 * Same script as ResumeIntroBgScroll, which finds it and writes the same +0x5c = 1. */
struct Unk677BCProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
    /* 30 */ s32 unk30;
    /* 34 */ s32 unk34;
    /* 38 */ s32 unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x58);
    /* 58 */ s32 unk58;
    /* 5c */ s32 unk5c;
};

ProcPtr StartIntroBgScroll(s32 a, s32 b, s32 c, ProcPtr parent)
{
    struct Unk677BCProc * proc = Proc_Start(gUnknown_08580FE4, parent);

    proc->unk58 = a;
    proc->unk5c = 1;
    proc->unk30 = 0;
    proc->unk2c = 0;
    proc->unk34 = b;
    proc->unk38 = c;

    return proc;
}
asm(".global sub_080677BC\n.thumb_set sub_080677BC, StartIntroBgScroll\n");
