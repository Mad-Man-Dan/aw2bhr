#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027C8C.
 * sub_08027C8C @ 0x08027C8C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
/* Another callback on StartDayStartGlyph's proc -- same +0x2c / +0x30 / +0x4a / +0x64
 * displacements src/decomp/c_08027B68.c already derived.
 *
 * `--proc->unk64 == 0` and not a reload: the compare is `lsls r0, #0x10;
 * cmp r0, #0` on the value the `strh` just wrote, where re-reading the s16
 * member would have used `ldrsh`. */
struct Unk27B10Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4a);
    /* 4a */ u16 unk4a;
    /* 4c */ STRUCT_PAD(0x4c, 0x64);
    /* 64 */ s16 unk64;
};

void DayStart_IDLE_08027C8D(struct Unk27B10Proc *proc)
{
    PutSprite(0, proc->unk2c, proc->unk30, gUnknown_08499E08, proc->unk4a);

    if (--proc->unk64 == 0)
        Proc_Break(proc);
}

asm(".global sub_08027C8C\n.thumb_set sub_08027C8C, DayStart_IDLE_08027C8D\n");
