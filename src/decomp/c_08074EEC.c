#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074EEC.
 * sub_08074EEC @ 0x08074EEC
 */

/* Pokes one halfword from the gUnknown_0861433C table into all five objects the
 * gUnknown_08614344 proc holds pointers to at +0x40. The proc is the one
 * StartWorldMapScope starts (it writes +0x54 and +0x58 of the same object).
 *
 * The table element is re-`ldrh`d every iteration, which is why
 * gUnknown_0861433C must NOT be declared const: the `strh` goes through a
 * pointer GCC cannot see past, so a non-const global is reloaded, while a const
 * one is hoisted into the preheader and takes a callee-saved register with it.
 * Counter 4..0 with `bge` is the ascending `for (i = 0; i < 5; i++)`. */

#include "proc.h"
struct Unk8074EECSub
{
    /* 0x00 */ u8 filler_00[0x22];
    /* 0x22 */ u16 unk_22;
};
struct Unk8074EECProc
{
    /* 0x00 */ u8 filler_00[0x40];
    /* 0x40 */ struct Unk8074EECSub * unk_40[5];
};

void SetWorldMapScopePalette(int index)
{
    struct Unk8074EECProc * proc = Proc_Find(gUnknown_08614344);
    int i;

    for (i = 0; i < 5; i++)
        proc->unk_40[i]->unk_22 = gUnknown_0861433C[index];
}
asm(".global sub_08074EEC\n.thumb_set sub_08074EEC, SetWorldMapScopePalette\n");
