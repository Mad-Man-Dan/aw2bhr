#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804FCA4.
 * sub_0804FCA4 @ 0x0804FCA4
 */

void sub_0804FCA4(void)
{
    struct Unk02029A10 *entry;
    u16 w;
    u16 *p1, *p2;
    u16 c, e, t;

    c = gUnknown_03001470[gUnknown_03001FBC].unk30;
    e = gUnknown_03001470[gUnknown_03001FBC].unk34;
    t = sub_080156C4(gUnknown_03001FBC);
    p1 = gUnknown_084C3F70[c];
    p2 = gUnknown_084C3F78[c];
    StepFigureSlide(c, e);
    w = StepFigureHitFlash(c, e, gUnknown_03001FBC);
    if (t == 0x1B)
    {
        PlayMusicOrSfx(gUnknown_085D6C88[gUnknown_03004580[c][1]].unk0c[1]
                                      [gUnknown_020296B0[c].unk1a & 1]);
        gUnknown_020296B0[c].unk1a++;
    }
    if (t == 0x1C)
    {
        PlayMusicOrSfx(gUnknown_085D6C88[gUnknown_03004580[c][1]]
                                      .unk0c[gUnknown_03004580[c][2] - 1]
                                      [gUnknown_020296B0[c].unk1a & 1]);
        gUnknown_020296B0[c].unk1a++;
    }
    entry = (struct Unk02029A10 *)(e * sizeof(struct Unk02029A10)
                                   + c * sizeof(struct Unk02029A10Group)
                                   + (u8 *)gUnknown_02029A10);
    entry->x += gUnknown_08553B28[c][w];
    SetSlotSpritePosition(gUnknown_03001FBC, entry->x - *p1, entry->y - *p2);
}
