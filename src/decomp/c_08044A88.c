#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044A88.
 * sub_08044A88 @ 0x08044A88, sub_08044AA0 @ 0x08044AA0
 */

#include "proc.h"

struct Unk44A88Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
#include "proc.h"

struct Unk44AA0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

void CoPowerSturmMeteorStrike(ProcPtr parent)
{
    struct Unk44A88Proc *proc = Proc_StartBlocking(gUnknown_084A0858, parent);

    proc->unk64 = 0x50;
}
asm(".global sub_08044A88\n.thumb_set sub_08044A88, CoPowerSturmMeteorStrike\n");

void sub_08044AA0(ProcPtr parent)
{
    struct Unk44AA0Proc *proc = Proc_StartBlocking(gUnknown_084A0858, parent);

    proc->unk64 = 0x28;
}
