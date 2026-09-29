#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804EE08.
 * sub_0804EE08 @ 0x0804EE08
 */

void StepBomberBobOrKnock(u16 a, u16 b, s16 c)
{
    struct Unk56E28 r;
    int v;

    if (gUnknown_02029B80[a][b] == 0)
    {
        gUnknown_03001470[c].unk28++;
        if (gUnknown_03001470[c].unk28 == 0xc0)
            gUnknown_03001470[c].unk28 = 0;

        v = gUnknown_08552500[gUnknown_03001470[c].unk28]
            + gUnknown_02029A10[a].entries[b].y;
        gUnknown_02029A10[a].entries[b].y = v;
    }
    else
    {
        PlayMusicOrSfx(0x23);
        gUnknown_02029B80[a][b] = 0;
        gUnknown_02029B94[a][b] = 1;
        gUnknown_02029A10[a].entries[b].frame = 0;

        r.unk00 = a;
        r.unk02 = b;
        r.unk04 = gUnknown_085534EC[a * 2];
        r.unk06 = gUnknown_085534EC[a * 2 + 1];
        r.unk08 = 0x10;
        r.unk0a = 8;
        r.unk0c = 0x64;
        SetFigureSlide(&r);
    }
}
asm(".global sub_0804EE08\n.thumb_set sub_0804EE08, StepBomberBobOrKnock\n");
