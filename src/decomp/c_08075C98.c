#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075C98.
 * sub_08075C98 @ 0x08075C98
 */

#include "hardware.h"
#include "proc.h"
/* A proc body gated on an on-screen test, then the c_08027B68.c affine shape
 * with a VARIABLE angle and two independently interpolated denominators.
 * Three things worth reading off it:
 *   - the guard's first clause is UNSIGNED (`bhi`), i.e. the one-sided
 *     `(u32)(x + 0x10) <= 0x100` range idiom, while the other two clauses on
 *     +0x30 are signed (`blt` / `bgt`) against -0x10 and 0xa0;
 *   - the constant 0x100 is shared between that guard and the second and third
 *     Interpolate calls, which is why the ROM parks it in r8 before the compare
 *     rather than rebuilding it three times;
 *   - the second and third Interpolate calls are IDENTICAL and both are made.
 *     A call is not CSE-able, so this is two calls in the source, not one
 *     result used twice -- pa/pc take the first denominator and pb/pd the
 *     second, exactly as c_08027B68.c splits unk34 and unk38.
 * +0x4c is s16 and the two access widths disagree on purpose: the increment is
 * `ldrh; adds #1; strh` because its only consumers are a halfword store and the
 * compare, and the sign extension reappears as `lsls #16; asrs #16` at that
 * compare -- the same reading as c_08027B68.c's +0x48.
 */

struct Unk8075C98Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x54);
    /* 54 */ struct Unk0801C210 *unk54;
};

void WorldMapMissionClear_FlashLoop(struct Unk8075C98Proc *proc)
{
    StepBank15WhiteFade(proc->unk4c, 0x20);
    EnablePaletteSync();

    if ((u32)(proc->unk2c + 0x10) <= 0x100
        && proc->unk30 >= -0x10
        && proc->unk30 <= 0xa0)
    {
        int angle = Interpolate(0, 0, 0x100, proc->unk4c, 0x20);
        int sx = Interpolate(0, 0x100, 8, proc->unk4c, 0x20);
        int sy = Interpolate(0, 0x100, 8, proc->unk4c, 0x20);

        SetObjAffine(0,
                     Div(COS_Q12(angle) * 16, sx),
                     Div(-SIN_Q12(angle) * 16, sy),
                     Div(SIN_Q12(angle) * 16, sx),
                     Div(COS_Q12(angle) * 16, sy));

        AP_Update(proc->unk54, (proc->unk2c - 8) & 0x1ff,
                     ((proc->unk30 - 8) & 0xff) | 0x300);
    }

    proc->unk4c++;

    if (proc->unk4c > 0x20)
        Proc_Break(proc);
}
asm(".global sub_08075C98\n.thumb_set sub_08075C98, WorldMapMissionClear_FlashLoop\n");
