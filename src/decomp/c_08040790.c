#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040790.
 * sub_08040790 @ 0x08040790, sub_080407E4 @ 0x080407E4
 */

#include "proc.h"
/* The other arm of the matched InventionAttack_StartHit: where StartInventionHit starts the
 * 0x0849FBEC proc from an existing entry, this one starts it from a bare
 * (x, y) with no entry -- unk4c is NULL, unk64 is the literal 6, and unk66
 * comes from the cell plane instead of the entry's unk04. Store order is
 * StartInventionHit's: unk2c, unk30, unk64, unk66, unk4c.
 *
 * gUnknown_020288B4 is named directly here with a CLEAN pool word (a single
 * `ldr`), which is what proves it a real global rather than a -fforce-addr
 * artefact -- StartAttackOnPipeSeamAt reaches the same array through agbcc's word at
 * 0x08091338, whose ROM content is 0x020288B4.
 *
 * The row table MUST be reached as a member of a struct laid over the map
 * header. The ROM computes `(map + 0x417A) + y * 2`, and that association only
 * survives through a COMPONENT_REF: `((u16 *)(map + 0x417A))[y]` reassociates
 * to `(map + y * 2) + 0x417A`, and binding a `u16 *` local fixes the
 * association but reorders the pool (gUnknown_08499590 ahead of
 * gUnknown_020288B4) and drops the r6 the ROM spends on the constant. */
struct Unk40790Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x18];
    /* 4c */ void *unk4c;
    /* 50 */ u8 filler_50[0x14];
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
};
/* The 0x0849FBEC proc's dispatcher: while unk66 is still positive it just
 * reports the cell, and once it runs out it fires the one-shot keyed by unk64.
 *
 * The switch operand is an s16 LOCAL and not `proc->unk64` directly: the ROM
 * reads unk64 with `ldrh`, subtracts 1, and only THEN sign-extends
 * (`lsls #0x10; asrs #0x10`). That is shorten_binary_op narrowing the load
 * because the difference is stored back into a 16-bit object, followed by the
 * switch's own sign extension -- `switch (proc->unk64)` on the s16 member would
 * have emitted `ldrsh` and no shift pair.
 *
 * StartExplosionEffect's fourth argument and StartExplosionEffectDefault's third are both invisible:
 * r3 and r2 already hold the proc. Case bodies are emitted in source order,
 * which is why the unk64 == 6 arm comes first in the ROM. */
struct Unk407E4Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x18];
    /* 4c */ struct Unk02028360 *unk4c;
    /* 50 */ u8 filler_50[0x14];
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
};

void StartPipeSeamHit(int a1, int a2, ProcPtr a3)
{
    struct Unk40790Proc *proc = Proc_StartBlocking(gUnknown_0849FBEC, a3);

    proc->unk2c = a1;
    proc->unk30 = a2;
    proc->unk64 = 6;
    proc->unk66 = gUnknown_020288B4[
        gMap->rowOffset[a2] + a1];
    proc->unk4c = NULL;
}
asm(".global sub_08040790\n.thumb_set sub_08040790, StartPipeSeamHit\n");

void InventionHit_Run(struct Unk407E4Proc *proc)
{
    s16 k;

    if (proc->unk66 > 0)
    {
        if (proc->unk64 == 6)
            StartExplosionEffect(proc->unk2c, proc->unk30, -3, proc);
        else
            StartExplosionEffectDefault(proc->unk2c, proc->unk30, proc);
    }
    else
    {
        k = proc->unk64 - 1;

        switch (k)
        {
        case 5:
            DestroyPipeSeam(proc->unk2c, proc->unk30, proc);
            break;
        case 0:
        case 3:
            DestroyLaserOrMinicannon(proc->unk4c, proc);
            break;
        case 2:
            StartBlackCannonDestroyedEffect(proc->unk4c, proc);
            break;
        case 4:
            StartDeathRayDestroyedEffect(proc->unk4c, proc);
            break;
        }
    }
}
asm(".global sub_080407E4\n.thumb_set sub_080407E4, InventionHit_Run\n");
