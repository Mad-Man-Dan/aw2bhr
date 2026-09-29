#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076494.
 * WorldMapCallout_PopInLoop @ 0x08076494
 */

#include "hardware.h"
#include "proc.h"
struct Unk08076494
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x28];
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};

void WorldMapCallout_PopInLoop(struct Unk08076494 *proc)
{
    int a;
    int b;
    int n;

    a = Interpolate(4, 0x10, 0x100, proc->unk5c, 0x10);
    b = Interpolate(0, 0, 0x10, proc->unk5c, 0x10);
    n = (a << 4) >> 8;
    SetObjAffine(0xC,
                 Div(COS_Q12(0) << 4, 0x100),
                 Div(-SIN_Q12(0) << 4, a),
                 Div(SIN_Q12(0) << 4, 0x100),
                 Div(COS_Q12(0) << 4, a));
    PutSpriteExt(1, proc->unk2c | 0x1800, (proc->unk30 - (n -= 0x10)) | 0x500,
                 gUnknown_086144C0, 0);
    PutSpriteExt(1, (proc->unk2c + proc->unk60) | 0x1800, (proc->unk30 + 0x10) | 0x500,
                 gUnknown_086144D4, 0);
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = b;
    gUnknown_03002B28 = 0x10 - b;
    gUnknown_03001FFC = 0;
    proc->unk5c++;
    if (proc->unk5c > 0xF)
    {
        PlayMusicOrSfx2(0x1CC);
        Proc_Break(proc);
    }
    CycleWorldMapCalloutColor();
}
asm(".global sub_08076494\n.thumb_set sub_08076494, WorldMapCallout_PopInLoop\n");
