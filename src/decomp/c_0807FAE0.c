#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807FAE0.
 * sub_0807FAE0 @ 0x0807FAE0
 */

#include "hardware.h"
#include "proc.h"
struct Unk0807FAE0
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x1c];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[0xa];
    /* 0x58 */ int unk58;
};

void CoPowerNameBanner_PopInLoop(struct Unk0807FAE0 *p)
{
    int i;

    p->unk2c = Interpolate(4, 0x80, 0, p->unk4c, p->unk58 * 4 + 8);
    sub_08043C28((p->unk2c + 0xa8) & 0x1ff, 0xa0, 0x41ca, 2, 1);

    for (i = 0; i < p->unk58; i++)
    {
        if (p->unk4c >= i * 4 && p->unk4c < i * 4 + 8)
        {
            p->unk2c = Interpolate(4, 0x200, 0x100, p->unk4c - i * 4, 8);
            SetObjAffine(i,
                         Div(gSinLut[0x40] << 4, p->unk2c ? p->unk2c : 2),
                         Div(-gSinLut[0] << 4, p->unk2c ? p->unk2c : 2),
                         Div(gSinLut[0] << 4, p->unk2c ? p->unk2c : 2),
                         Div(gSinLut[0x40] << 4, p->unk2c ? p->unk2c : 2));
            PutSpriteExt(0, (i << 9) | gUnknown_030059A0[i], 0x330,
                         gUnknown_0848B6E6, 0x3000 | (0x30a + i * 8));
        }
        else if (p->unk4c >= i * 4 + 8)
        {
            PutSprite(0, gUnknown_030059A0[i] + 8, 0x40,
                      gUnknown_0848B6E6, 0x3000 | (0x30a + i * 8));
        }
    }

    if (p->unk4c < p->unk58 * 4 + 8)
    {
        p->unk4c++;
    }
    else
    {
        p->unk4c = 0;
        Proc_Break(p);
    }
}
asm(".global sub_0807FAE0\n.thumb_set sub_0807FAE0, CoPowerNameBanner_PopInLoop\n");
