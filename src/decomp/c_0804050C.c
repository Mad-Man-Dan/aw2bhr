#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804050C.
 * sub_0804050C @ 0x0804050C, sub_08040554 @ 0x08040554, sub_08040590 @ 0x08040590
 */

#include "proc.h"
/* Scrolls the proc's animation up five pixels a frame and breaks the proc once
 * it has passed the top of the gUnknown_08499590 screen. The AP_Update
 * result is discarded here -- no `lsls #0x18` follows the `bl`, which is what
 * distinguishes this from ExplosionEffect_Loop beside it, where the same call is
 * tested.
 *
 * `proc->unk30 -= 5` then re-reading unk30 in the comparison is one statement
 * each: agbcc keeps the stored value in r0 rather than reloading. */
struct Unk4050CProc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x1c];
    /* 50 */ struct Unk0801C210 *unk50;
};
/* SIX parameters, the last two on the stack; the sixth is the
 * Proc_StartBlocking parent. The wider sibling of StartSiloMissileLaunch: same three
 * stores plus a fifth argument into unk44.
 *
 * The fifth parameter is `int` and not `u16`: its prologue is a bare
 * `ldr r7,[sp,#0x18]` with no PROMOTE_MODE narrowing, and the only narrowing
 * anywhere is the `strh` itself, which is a conversion at the store.
 *
 * The unk4a store comes out as `adds r1,r0,#0; adds r1,#0x4a` where
 * StartSiloMissileLaunch's is `adds r0,#0x4a`: r0 still has to survive for the unk44
 * store here, so the address goes to a scratch. That is register allocation,
 * not a source difference. */
struct Unk40554Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x10];
    /* 44 */ s16 unk44;
    /* 46 */ u8 filler_46[0x04];
    /* 4a */ u16 unk4a;
};
/* The proc's init: it converts the cell coordinates at unk2c/unk30 into the
 * pixel pair at unk54/unk58 (the same `cell * 16 + halfTile` the matched
 * ApplySiloDamage then reads back against gUnknown_08499590), snapshots the
 * screen's y scroll into unk66, and installs the animation.
 *
 * `proc->unk50 = AP_Create(...)` followed by `AP_SwitchAnimation(proc->unk50, 2)`
 * is one `str` and a reuse of r0, not a reload -- exactly as the promoted
 * CaptureAnimSprites_Init spells the same idiom three times. */
struct Unk40590Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x16];
    /* 4a */ u16 unk4a;
    /* 4c */ u8 filler_4c[0x04];
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
    /* 58 */ int unk58;
    /* 5c */ u8 filler_5c[0x0a];
    /* 66 */ u16 unk66;
};

void SiloMissileLaunch_Loop(struct Unk4050CProc *proc)
{
    AP_Update(proc->unk50,
                 proc->unk2c - gMap->scrollX,
                 proc->unk30 - gMap->scrollY);

    proc->unk30 -= 5;

    if (proc->unk30 - gMap->scrollY < 0)
    {
        AP_Delete(proc->unk50);
        Proc_Break(proc);
    }
}
asm(".global sub_0804050C\n.thumb_set sub_0804050C, SiloMissileLaunch_Loop\n");

void StartSiloMissileFall(int a, int b, int c, int d, int e, ProcPtr parent)
{
    struct Unk40554Proc *proc = Proc_StartBlocking(gUnknown_0849FBBC, parent);

    proc->unk2c = a;
    proc->unk30 = b;
    proc->unk4a = (d << 12) | c;
    proc->unk44 = e;
}
asm(".global sub_08040554\n.thumb_set sub_08040554, StartSiloMissileFall\n");

void SiloMissileFall_Init(struct Unk40590Proc *proc)
{
    proc->unk54 = (proc->unk2c << 4) + 8;
    proc->unk58 = (proc->unk30 << 4) + 0x10;
    proc->unk66 = gMap->scrollY;

    proc->unk50 = AP_Create((void *)gUnknown_08111D94, 1, 1);
    AP_SwitchAnimation(proc->unk50, 2);
    proc->unk50->unk22 = proc->unk4a;
}
asm(".global sub_08040590\n.thumb_set sub_08040590, SiloMissileFall_Init\n");
