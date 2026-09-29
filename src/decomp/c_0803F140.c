#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F140.
 * sub_0803F140 @ 0x0803F140
 */

struct UnkF140Proc
{
    /* 00 */ u8 filler_00[0x54];
    /* 54 */ int unk54;
    /* 58 */ int unk58;
    /* 5c */ u8 filler_5c[0x0c];
    /* 68 */ s16 unk68;
    /* 6a */ s16 unk6a;
};

void CannonFire_StartMuzzleEffect(struct UnkF140Proc *proc)
{
    int x;
    int y;

    x = 0;
    y = 0;
    sub_0803F29C(&x, &y, proc->unk6a);
    Decompress(GetCannonFireTileGraphic(proc->unk6a), (void *)0x06013940);
    ApplyPaletteExt(gUnknown_08109564, 0x260, 0x40);
    APProc_Create(GetCannonFireSpriteData(proc->unk6a),
                 proc->unk54 * 16 - gMap->scrollX + x,
                 proc->unk58 * 16 - gMap->scrollY + y,
                 0x31CA,
                 sub_0803F27C(proc->unk68),
                 0);
    PlayMusicOrSfx2(0x1C4);
}
asm(".global sub_0803F140\n.thumb_set sub_0803F140, CannonFire_StartMuzzleEffect\n");
