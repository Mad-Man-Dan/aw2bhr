#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803628C.
 * sub_0803628C @ 0x0803628C, sub_080362A8 @ 0x080362A8
 */

#include "proc.h"
/* One of a pair of state-setters on the proc's +0x35 byte (MoveSlideState_WaitForCameraScroll is the
 * other). `adds r1, r4, #0; adds r1, #0x35` is the THUMB `strb` displacement
 * limit of 31, not an address being taken -- the c_08035CF4.c note. */
struct Unk3628CProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x35);
    /* 0x35 */ u8 unk35;
};
/* sub_0803628C's twin. The `(s32)` cast and the `== -1` are the spelling every
 * other FindSlotScript caller in src/decomp already uses (c_08017C4C.c); the
 * `lsls #0x18; asrs #0x18` before the compare is the s8 return being
 * re-narrowed at the call site. */
struct Unk362A8Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x35);
    /* 0x35 */ u8 unk35;
};

void sub_0803628C(ProcPtr procArg)
{
    struct Unk3628CProc *proc = procArg;

    if (!sub_0802759C())
        proc->unk35 = 3;
}

void MoveSlideState_WaitForCameraScroll(ProcPtr procArg)
{
    struct Unk362A8Proc *proc = procArg;

    if (FindSlotScript((s32)gUnknown_0849A00C) == -1)
        proc->unk35 = 2;
}
asm(".global sub_080362A8\n.thumb_set sub_080362A8, MoveSlideState_WaitForCameraScroll\n");
