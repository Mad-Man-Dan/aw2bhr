#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806775C.
 * sub_0806775C @ 0x0806775C
 */

#include "proc.h"
/* r1 is never written before the `bl`, so the parent is this starter's own
 * second parameter passed straight through. The argument is saved with a bare
 * `adds r4, r0, #0` across the call, so it is an `int` rather than the u8 the
 * `lsls #2` index might suggest.
 *
 * +0x30 is read back through the register the table element is still sitting
 * in (`movs r1, #0; ldrsb r1, [r2, r1]`), not through a reload of +0x2c: the
 * intervening `str` to +0x38 is SImode and a word store does not invalidate a
 * cached load, so `proc->unk2c[0]` costs no second `ldr`.
 */
struct Unk6775CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s8 *unk2c;
    /* 30 */ s32 unk30;
    /* 34 */ s32 unk34;
    /* 38 */ s32 unk38;
};

void StartIntroBgJitter(int a, ProcPtr parent)
{
    struct Unk6775CProc *proc = Proc_Start(ProcScr_IntroT3Child, parent);

    proc->unk2c = gUnknown_08580FC0[a];
    proc->unk38 = 0;
    proc->unk30 = proc->unk2c[0];
    proc->unk34 = 2;
}
asm(".global sub_0806775C\n.thumb_set sub_0806775C, StartIntroBgJitter\n");
