#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080392C8.
 * sub_080392C8 @ 0x080392C8, sub_080392F4 @ 0x080392F4
 */

#include "proc.h"
struct Unk392C8Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ int unk2c;
    /* 0x30 */ STRUCT_PAD(0x30, 0x64);
    /* 0x64 */ s16 unk64;
};
struct Unk392F4Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ int unk2c;
    /* 0x30 */ STRUCT_PAD(0x30, 0x64);
    /* 0x64 */ s16 unk64; /* signed: read `ldrsh` into Interpolate's s32
                           * fourth argument, while the increment beside it is
                           * a width-only `ldrh; adds; strh` */
};

/* The slide-in step: x walks left by 8 a frame until it reaches 0xB0, where it
 * is clamped, the frame counter reset and the script unblocked. The compare is
 * on the value left in the register by the store, so the decrement and the
 * test are one expression.
 *
 * The parameter is the record type itself, not a ProcPtr aliased to a local:
 * an alias costs an extra `adds r4, r2, #0` because it has to survive
 * Proc_Break separately from the parameter. */

void CoPowerPortrait_SlideInLoop(struct Unk392C8Proc *proc)
{
    proc->unk2c -= 8;

    if (proc->unk2c <= 0xaf)
    {
        proc->unk2c = 0xb0;
        proc->unk64 = 0;

        Proc_Break(proc);
    }

    CoPowerPortrait_Draw(proc);
}
asm(".global sub_080392C8\n.thumb_set sub_080392C8, CoPowerPortrait_SlideInLoop\n");

/* The slide-out step: 60 frames of Interpolate from 0xB0 to -64, unblocking
 * the script on the frame the end value is reached. -64 is CSEd into one
 * register and used both as the argument and as the compare, which is agbcc
 * reusing a register it already knows -- the literal is written twice here. */

void CoPowerPortrait_SlideOutLoop(struct Unk392F4Proc *proc)
{
    proc->unk2c = Interpolate(1, 0xb0, -64, proc->unk64, 60);

    proc->unk64++;

    if (proc->unk2c == -64)
        Proc_Break(proc);

    CoPowerPortrait_Draw(proc);
}
asm(".global sub_080392F4\n.thumb_set sub_080392F4, CoPowerPortrait_SlideOutLoop\n");
