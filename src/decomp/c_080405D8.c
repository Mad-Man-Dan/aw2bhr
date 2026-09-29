#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080405D8.
 * sub_080405D8 @ 0x080405D8
 */

#include "proc.h"
/* The vertical twin of the matched SiloMissileLaunch_Loop next door: same
 * AP_Update / AP_Delete / Proc_Break shape, but scrolling the animation
 * DOWN eight pixels a frame and stopping once it has passed the proc's own
 * target row rather than the top of the screen.
 *
 * unk66 is s16 and not the u16 SiloMissileFall_Init stores it as: every read here is
 * `ldrsh`, including the re-read after `unk66 += 8` -- the stored value is not
 * reused because the comparison needs it sign-extended. The `+= 8` itself is
 * `ldrh; adds; strh`, which is the same for either signedness.
 *
 * unk54 is the pixel x SiloMissileFall_Init precomputed as `(unk2c << 4) + 8`, which is
 * why no `- 8` appears here and one does in ExplosionEffect_Loop beside it. */
struct Unk405D8Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x1c];
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
    /* 58 */ u8 filler_58[0x0e];
    /* 66 */ s16 unk66;
};

void SiloMissileFall_Loop(struct Unk405D8Proc *proc)
{
    AP_Update(proc->unk50,
                 proc->unk54 - gMap->scrollX,
                 proc->unk66 - gMap->scrollY);

    proc->unk66 += 8;

    if (proc->unk66 > (proc->unk30 << 4))
    {
        AP_Delete(proc->unk50);
        Proc_Break(proc);
    }
}
asm(".global sub_080405D8\n.thumb_set sub_080405D8, SiloMissileFall_Loop\n");
