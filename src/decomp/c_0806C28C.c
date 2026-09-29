#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C28C.
 * sub_0806C28C @ 0x0806C28C
 */

#include "proc.h"
#include "hardware.h"
struct Unk806C28C
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};

void CreditsRank_StampLoop(struct Unk806C28C *proc)
{
    s32 t;
    int y;

    t = Interpolate(0, 0x200, 0x100, proc->unk5c, 0x10);

    SetObjAffine(0,
        Div(COS_Q12(0) << 4, t),
        Div(-SIN_Q12(0) << 4, t),
        Div(SIN_Q12(0) << 4, t),
        Div(COS_Q12(0) << 4, t));

    y = gUnknown_085816F0[proc->unk58].unk0c + 0x54;

    PutSpriteExt(0, 0x84, ((y - proc->unk60) & 0xff) | 0x700,
        gUnknown_085816F0[proc->unk58].unk08, 0x1090);

    if (proc->unk5c == 8)
        PlayMusicOrSfx2(0x79);

    if (proc->unk5c > 0xf)
    {
        proc->unk5c = 0;
        gUnknown_0202F2C0 = 1;
        Proc_Break(proc);
    }
    else
    {
        proc->unk5c++;
    }
}
asm(".global sub_0806C28C\n.thumb_set sub_0806C28C, CreditsRank_StampLoop\n");
