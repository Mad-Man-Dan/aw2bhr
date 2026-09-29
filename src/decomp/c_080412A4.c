#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080412A4.
 * sub_080412A4 @ 0x080412A4
 */

#include "proc.h"
/* The gUnknown_0849FD44 proc, viewed through only the member this function
 * writes. Tagged per-function on purpose: promote.py may merge several of these
 * drafts into one linker unit, and two files defining the same tag would not
 * compile. */
struct Unk412A4Owner
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x42);
    /* 42 */ u16 unk42;
};
/* The proc these handlers run on. +0x4c is SIGNED: the incremented value is
 * sign-extended with `lsls #0x10; asrs #0x10` before the compare. */
struct Unk412A4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ s16 unk4c;
};

/* `adds r1, r4, #0; adds r1, #0x4c` is not an address being taken -- 0x4c is
 * past strh's 62-byte displacement limit, so agbcc has to materialise the
 * address. Same for +0x42 on the other proc. */
void CaptureAnimShake_Loop(struct Unk412A4Proc *proc)
{
    struct Unk412A4Owner *p = Proc_Find(gUnknown_0849FD44);

    if (++proc->unk4c > 8)
    {
        p->unk42 = 0;
        Proc_Break(proc);
    }
    else
    {
        p->unk42 = proc->unk4c & 1;
    }
}
asm(".global sub_080412A4\n.thumb_set sub_080412A4, CaptureAnimShake_Loop\n");
