#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027CC8.
 * sub_08027CC8 @ 0x08027CC8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* The ProcScr_DayStart proc, started by StartDayStartGlyph: a spinning, shrinking
 * affine sprite. StartDayStartGlyph pins every field used here -- +0x2c and +0x30
 * are its first two `int` parameters, +0x34 and +0x38 are both seeded 0x200
 * as words, +0x48 is `strh 0x20` (the angle), +0x4a is `strh (arg2 + 0x1ca) |
 * 0x3000` (the OAM2 word) and +0x6a is `strh arg3` (the OBJ affine slot,
 * which SetObjAffine takes as `s32` and PutSpriteExt's x word takes `<< 9`).
 *
 * +0x48 is s16, not u16, and the two accesses disagree on purpose: the
 * decrement in sub_08027B68 loads `ldrh` because force_to_mode drops the
 * sign extension when the only consumers are a halfword store and a `& 0xff`,
 * while the `== 0` test that follows keeps `ldrsh` because a SImode compare
 * against 0 has no such mask. Declaring it u16 breaks the compare. */
struct Unk27B10Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x48);
    /* 48 */ s16 unk48;
    /* 4a */ u16 unk4a;
    /* 4c */ STRUCT_PAD(0x4c, 0x64);
    /* 64 */ s16 unk64;
    /* 66 */ STRUCT_PAD(0x66, 0x6a);
    /* 6a */ s16 unk6a;
};

void DayStart_IDLE_08027CC9(struct Unk27B10Proc *proc)
{
    proc->unk34 -= Div(proc->unk34, 2);

    if (proc->unk34 <= 4)
    {
        Proc_End(proc);
        return;
    }

    SetObjAffine(proc->unk6a,
                 Div(COS_Q12(proc->unk48) * 16, proc->unk34 != 0 ? proc->unk34 : 2),
                 Div(-SIN_Q12(proc->unk48) * 16, proc->unk38 != 0 ? proc->unk38 : 2),
                 Div(SIN_Q12(proc->unk48) * 16, proc->unk34 != 0 ? proc->unk34 : 2),
                 Div(COS_Q12(proc->unk48) * 16, proc->unk38 != 0 ? proc->unk38 : 2));

    PutSpriteExt(0, (proc->unk2c & 0x1ff) | (proc->unk6a << 9),
                 (proc->unk30 & 0xff) | 0x300, gUnknown_08499E10, proc->unk4a);
}

asm(".global sub_08027CC8\n.thumb_set sub_08027CC8, DayStart_IDLE_08027CC9\n");
