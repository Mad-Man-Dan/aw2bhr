#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080404A0.
 * sub_080404A0 @ 0x080404A0
 */

#include "proc.h"
/* The 0x0849FB8C proc's init: it converts the cell pair at unk2c/unk30 IN PLACE
 * into the pixel pair `cell * 16 + halfTile` -- the same conversion the matched
 * SiloMissileFall_Init writes into the separate unk54/unk58 -- then installs the
 * animation and plays a sound.
 *
 * Because the `+ 8` and `+ 0x10` are stored back into unk2c/unk30 BEFORE the
 * screen origin is subtracted, no `A - (B - 8)` association arises here and the
 * ROM simply reuses the stored values in r1/r2 rather than reloading them.
 * That is the same reason the parked ExplosionEffect_Loop next door is hard: it does
 * the same arithmetic without the intervening store.
 *
 * APProc_Create takes SIX arguments, the last two on the stack (`str` to sp+0
 * and sp+4 with 0 and 1); unk4a is the packed `tile | pal << 12` halfword that
 * StartSiloMissileLaunch builds and that is copied into the sprite's +0x22 below. */
struct Unk404A0Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x16];
    /* 4a */ u16 unk4a;
    /* 4c */ u8 filler_4c[0x04];
    /* 50 */ struct Unk0801C210 *unk50;
};

void SiloMissileLaunch_Init(struct Unk404A0Proc *proc)
{
    proc->unk2c = (proc->unk2c << 4) + 8;
    proc->unk30 = (proc->unk30 << 4) + 0x10;

    APProc_Create((void *)gUnknown_08111D94,
                 proc->unk2c - gMap->scrollX,
                 proc->unk30 - gMap->scrollY,
                 proc->unk4a, 0, 1);

    proc->unk50 = AP_Create((void *)gUnknown_08111D94, 1, 1);
    AP_SwitchAnimation(proc->unk50, 1);
    proc->unk50->unk22 = proc->unk4a;

    PlayMusicOrSfx2(0x1C6);
}
asm(".global sub_080404A0\n.thumb_set sub_080404A0, SiloMissileLaunch_Init\n");
