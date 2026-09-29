#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067358.
 * sub_08067358 @ 0x08067358
 */

#include "hardware.h"
#include "proc.h"
struct Unk67358Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ int unk3c;
};

/* MATCHED. Byte-for-byte the same function as BlendRamp_Loop -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void sub_08067358(struct Unk67358Proc *proc)
{
    s32 v;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0)
        | ((proc->unk3c << 4) | 0xF);

    v = Interpolate(0, proc->unk30, proc->unk34, proc->unk38, proc->unk2c);
    gUnknown_03001FFC = v;

    if (proc->unk2c == proc->unk38)
        Proc_Break(proc);

    proc->unk38++;
}
