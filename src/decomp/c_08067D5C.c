#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067D5C.
 * sub_08067D5C @ 0x08067D5C, sub_08067D6C @ 0x08067D6C, sub_08067D94 @ 0x08067D94
 */

#include "proc.h"
struct Unk67D5CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x30);
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4c);
    /* 4c */ u16 unk4c;
};
struct Unk67D6CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x30);
    /* 30 */ int unk30;
};
struct Unk67D94Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ s16 unk4c;
};

/* Arms the IntroBgBounceIn_ScrollToRest slide-out: +0x30 starts at 0x140 (one screen width,
 * built as `movs #0xa0; lsls #1` rather than a pool word) and +0x4c is the
 * frame index IntroBgBounceIn_Settle walks. No frame, no calls -- `bx lr` straight out. */
void IntroBgBounceIn_Init(struct Unk67D5CProc *proc)
{
    proc->unk30 = 0x140;
    proc->unk4c = 0;
}
asm(".global sub_08067D5C\n.thumb_set sub_08067D5C, IntroBgBounceIn_Init\n");

/* Slides +0x30 in by 5 a frame and breaks once it reaches zero, publishing the
 * remaining offset each tick. The argument setup is the grouped-by-operand-class
 * order from docs/agbcc-codegen.md: the third argument's narrowing first
 * (it is a value already in a register), then the two `mov #imm8` zeroes. */
void IntroBgBounceIn_ScrollToRest(struct Unk67D6CProc *proc)
{
    proc->unk30 -= 5;
    SetBgScrollShadow(0, 0, proc->unk30);

    if (proc->unk30 <= 0)
        Proc_Break(proc);
}
asm(".global sub_08067D6C\n.thumb_set sub_08067D6C, IntroBgBounceIn_ScrollToRest\n");

/* A 13-frame canned animation: step a frame index 0..0xc and publish that
 * frame's level from the gUnknown_085810D4 table. +0x4c is s16 (`movs r2,#0;
 * ldrsh r0,[r4,r2]`, an s16 object rather than a cast) and its address is held
 * in r4 across the call because all three uses need it.
 *
 * The increment comes out as `ldrh; adds #1; strh` -- unsigned even though the
 * field is signed, which is agbcc's normal read-modify-write on a HImode
 * lvalue and carries no signedness information. */
void IntroBgBounceIn_Settle(struct Unk67D94Proc *proc)
{
    SetBgScrollShadow(0, 0, gUnknown_085810D4[proc->unk4c]);

    if (proc->unk4c == 0xc)
        Proc_Break(proc);

    proc->unk4c++;
}
asm(".global sub_08067D94\n.thumb_set sub_08067D94, IntroBgBounceIn_Settle\n");
