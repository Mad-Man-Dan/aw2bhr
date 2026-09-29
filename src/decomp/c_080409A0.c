#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080409A0.
 * sub_080409A0 @ 0x080409A0, sub_080409B4 @ 0x080409B4, sub_080409D0 @ 0x080409D0
 */

/* The twin of sub_08040088 above, calling ScrollCameraToCenterCell instead of
 * ScrollCameraToKeepCellInView -- the same pair of s16 fields at 0x2c/0x30. */
struct Unk409A0Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ s16 unk2c;
    /* 2e */ u8 filler_2e[0x02];
    /* 30 */ s16 unk30;
};
#include "proc.h"
/* Forwards the proc's own cell coordinates to StartSiloLaunch together with the
 * proc itself, which StartSiloLaunch passes on as sub_0804046C's fifth argument
 * -- the Proc_StartBlocking parent. `adds r2,r0,#0` before either coordinate
 * load is the parameter copy that argument setup groups first, not a spill.
 * Same s16 field pair as the matched sub_08040984 at this address. */
struct Unk409B4Proc
{
    /* 00 */ u8 filler_00[0x64];
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
};
/* Restarts the sub_08040554 proc from this proc's own stored state, passing
 * itself as the parent. unk4a is the packed `pal << 12 | tile` halfword
 * sub_0804046C and sub_08040554 both build. */
struct Unk409D0Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x16];
    /* 4a */ u16 unk4a;
};

void sub_080409A0(struct Unk409A0Proc *proc)
{
    ScrollCameraToCenterCell(proc->unk2c, proc->unk30);
}

void sub_080409B4(struct Unk409B4Proc *proc)
{
    StartSiloLaunch(proc->unk64, proc->unk66, proc);
}

void sub_080409D0(struct Unk409D0Proc *proc)
{
    sub_08040624(proc->unk2c, proc->unk30, proc->unk4a, proc);
}
