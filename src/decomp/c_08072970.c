#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072970.
 * sub_08072970 @ 0x08072970, sub_0807298C @ 0x0807298C
 */

#include "proc.h"
struct Unk8613F34Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ u32 unk2c;
    /* 30 */ STRUCT_PAD(0x30, 0x34);
    /* 34 */ u32 unk34;
};
struct Unk8613F44Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ u32 unk2c;
    /* 30 */ u32 unk30;
    /* 34 */ u32 unk34;
};

void CallDelayed(u32 a, u32 b)
{
    struct Unk8613F34Proc * proc = Proc_Start(gUnknown_08613F34, PROC_TREE_3);

    proc->unk2c = a;
    proc->unk34 = b;
}
asm(".global sub_08072970\n.thumb_set sub_08072970, CallDelayed\n");

void CallDelayedArg(u32 a, u32 b, u32 c)
{
    struct Unk8613F44Proc * proc = Proc_Start(gUnknown_08613F44, PROC_TREE_3);

    proc->unk2c = a;
    proc->unk30 = b;
    proc->unk34 = c;
}
asm(".global sub_0807298C\n.thumb_set sub_0807298C, CallDelayedArg\n");
