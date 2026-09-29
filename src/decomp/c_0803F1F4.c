#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F1F4.
 * sub_0803F1F4 @ 0x0803F1F4
 */

struct UnkF1F4Proc
{
    /* 00 */ u8 filler_00[0x64];
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
    /* 68 */ s16 unk68;
    /* 6a */ s16 unk6a;
};

void CannonFire_StartImpact(struct UnkF1F4Proc *proc)
{
    APProc_Create(GetCannonFireSpriteData(proc->unk6a),
                 proc->unk64 * 16 - gMap->scrollX + 8,
                 proc->unk66 * 16 - gMap->scrollY + 0x10,
                 0x31CA,
                 sub_0803F27C(proc->unk68) + 2,
                 0);
    PlayMusicOrSfx2(0x1C3);
    StartScreenShake(1, 0x14, proc);
    StartWhiteFlash(2, 0, 1, proc);
}
asm(".global sub_0803F1F4\n.thumb_set sub_0803F1F4, CannonFire_StartImpact\n");
