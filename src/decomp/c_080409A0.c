#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080409A0.
 * sub_080409A0 @ 0x080409A0, sub_080409B4 @ 0x080409B4, sub_080409D0 @ 0x080409D0
 */

/* The twin of ExplosionEffect_ScrollToCell above, calling ScrollCameraToCenterCell instead of
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
 * proc itself, which StartSiloLaunch passes on as StartSiloMissileLaunch's fifth argument
 * -- the Proc_StartBlocking parent. `adds r2,r0,#0` before either coordinate
 * load is the parameter copy that argument setup groups first, not a spill.
 * Same s16 field pair as the matched SiloFire_ScrollToSilo at this address. */
struct Unk409B4Proc
{
    /* 00 */ u8 filler_00[0x64];
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
};
/* Restarts the StartSiloMissileFall proc from this proc's own stored state, passing
 * itself as the parent. unk4a is the packed `pal << 12 | tile` halfword
 * StartSiloMissileLaunch and StartSiloMissileFall both build. */
struct Unk409D0Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x16];
    /* 4a */ u16 unk4a;
};

void SiloFire_ScrollToTarget(struct Unk409A0Proc *proc)
{
    ScrollCameraToCenterCell(proc->unk2c, proc->unk30);
}
asm(".global sub_080409A0\n.thumb_set sub_080409A0, SiloFire_ScrollToTarget\n");

void SiloFire_StartLaunch(struct Unk409B4Proc *proc)
{
    StartSiloLaunch(proc->unk64, proc->unk66, proc);
}
asm(".global sub_080409B4\n.thumb_set sub_080409B4, SiloFire_StartLaunch\n");

void SiloFire_StartStrike(struct Unk409D0Proc *proc)
{
    StartSiloMissileStrike(proc->unk2c, proc->unk30, proc->unk4a, proc);
}
asm(".global sub_080409D0\n.thumb_set sub_080409D0, SiloFire_StartStrike\n");
